/*****************************************************************************
 *
 * File Name : main.c
 *
 * Description: main
 *
 * Date : 2024-05-07
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include <unistd.h>
// #include <sys/stat.h>
// #include <sys/mman.h>
#include "mod1/mad.h"

#include "wm_type_def.h"

#include "wm_cpu.h"
// #include "wm_uart.h"
#include "wm_gpio.h"
// #include "wm_pwm.h"
// #include "wm_params.h"
#include "wm_osal.h"
// #include "wm_efuse.h"
#include "wm_mem.h"
// #include "wm_regs.h"
#include "wm_gpio_afsel.h"
#include "wm_i2s.h"

#include "BabyElephantWalk60_mp3.h"
#include "w806_def.h"

#define DEMO_TASK_SIZE 2048
static OS_STK DemoTaskStk[DEMO_TASK_SIZE];
#define DEMO_TASK_PRIO 32

#define SERIAL_DEBUG

static int decode (unsigned char const *, unsigned long);

static u16 i2s_DMABUF_SIZE = 0;
static u8 *i2s_DMABUF = NULL;
static u8 *mp3_BUF = NULL;

static u8 info_channels = 2;
static u8 info_bits_per_sample = 16;

typedef enum dma_status
{
  D_NONE = 0,
  D_HW_INIT = 1,
  D_PLAY = 2,
  D_END_FILE = 3,
  D_STOP = 4,
  D_QUERY_TO_STOP = 5,
} _dma_status;

static volatile _dma_status stat_dma = D_NONE;

#include <FreeRTOS.h>
#include <semphr.h>
#define LONG_TIME 0xffff
SemaphoreHandle_t xSemaphore = NULL;
StaticSemaphore_t xSemaphoreBuffer;

static wm_dma_handler_type hdma_tx;
static void
i2sDmaSendCpltCallback (wm_dma_handler_type *hdma)
{
  memcpy (i2s_DMABUF + i2s_DMABUF_SIZE, mp3_BUF, i2s_DMABUF_SIZE);
  /* Unblock the task by releasing the semaphore. */
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR (xSemaphore, &xHigherPriorityTaskWoken);
}
static void
i2sDmaSendHalfCpltCallback (wm_dma_handler_type *hdma)
{
  memcpy (i2s_DMABUF, mp3_BUF, i2s_DMABUF_SIZE);
  /* Unblock the task by releasing the semaphore. */
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR (xSemaphore, &xHigherPriorityTaskWoken);
}

void
user_task (void *sdata)
{
  printf ("user task\n");

  tls_sys_clk_set (CPU_CLK_240M); // нам нужно 240MHz

  tls_gpio_cfg (WM_IO_PB_00, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);
  tls_gpio_cfg (WM_IO_PB_01, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);
  tls_gpio_write (WM_IO_PB_00, 1);
  tls_gpio_write (WM_IO_PB_01, 1);

  wm_i2s_ck_config (WM_IO_PA_08); // clock line         i2s Bclk
  wm_i2s_ws_config (WM_IO_PA_09); // word select line   i2s LRclk
  wm_i2s_do_config (WM_IO_PA_10); // Dout
  wm_i2s_di_config (WM_IO_PA_11); // Din
                                  // no need
  // wm_i2s_mclk_config (WM_IO_PA_00);// only PA0  ,The clock is still the
  // internal 160MHz clock, whether the MCLK clock is turned on, and the
  // operating frequencies of MCLK and BCLK

  // no need
  // wm_i2s_extclk_config (WM_IO_PA_07);
  // only PA7,The working clock configuration of the I2S module can be
  // configured by setting the 0x40000718 register in the clock and reset
  // module to select the external clock source.
  stat_dma = D_NONE;
  xSemaphore = xSemaphoreCreateBinaryStatic (&xSemaphoreBuffer);
  if (xSemaphoreGive (xSemaphore) != pdTRUE) // Сделать иначе не
                                             // инициализирован
    {
      // We would expect this call to fail because we cannot give
      // a semaphore without first "taking" it!
    }

  decode (BabyElephantWalk60_mp3, BabyElephantWalk60_mp3_len);

  wm_i2s_tx_rx_stop ();
  if (i2s_DMABUF != NULL)
    {
      free (i2s_DMABUF);
      i2s_DMABUF = NULL;
    }
  if (mp3_BUF != NULL)
    {
      free (mp3_BUF);
      mp3_BUF = NULL;
    }

  printf ("end \n");

  tls_gpio_cfg (BUILDIN_LED_D1, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);
  u8 u8_led_state = 0;
  tls_gpio_write (BUILDIN_LED_D1, u8_led_state);
  for (;;)
    {
      tls_gpio_write (BUILDIN_LED_D1, u8_led_state);
      tls_os_time_delay (HZ);
      u8_led_state = ~u8_led_state;
    }
}

void
UserMain (void)
{

  tls_os_task_create (NULL, NULL, user_task, NULL,
                      (void *)DemoTaskStk, /* task's stack start address */
                      DEMO_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      DEMO_TASK_PRIO, 0);

  //	while(1)
  //	{
  //	}
}

/*
 * This is a private message structure. A generic pointer to this structure
 * is passed to each of the callback functions. Put here any data you need
 * to access from within the callbacks.
 */

struct buffer
{
  unsigned char const *start;
  unsigned long length;
};

/*
 * This is the input callback. The purpose of this callback is to (re)fill
 * the stream buffer which is to be decoded. In this example, an entire file
 * has been mapped into memory, so we just call mad_stream_buffer() with the
 * address and length of the mapping. When this callback is called a second
 * time, we are finished decoding.
 */

static enum mad_flow
input (void *data, struct mad_stream *stream)
{
  struct buffer *buffer = data;

  if (!buffer->length)
    return MAD_FLOW_STOP;

  mad_stream_buffer (stream, buffer->start, buffer->length);

  buffer->length = 0;

  return MAD_FLOW_CONTINUE;
}

/*
 * The following utility routine performs simple rounding, clipping, and
 * scaling of MAD's high-resolution samples down to 16 bits. It does not
 * perform any dithering or noise shaping, which would be recommended to
 * obtain any exceptional audio quality. It is therefore not recommended to
 * use this routine if high-quality output is desired.
 */

static inline signed int
scale (mad_fixed_t sample)
{
  /* round */
  sample += (1L << (MAD_F_FRACBITS - 16));

  /* clip */
  if (sample >= MAD_F_ONE)
    sample = MAD_F_ONE - 1;
  else if (sample < -MAD_F_ONE)
    sample = -MAD_F_ONE;

  /* quantize */
  return sample >> (MAD_F_FRACBITS + 1 - 16);
}

/*
 * This is the output callback function. It is called after each frame of
 * MPEG audio data has been completely decoded. The purpose of this callback
 * is to output (or play) the decoded PCM audio.
 */

static enum mad_flow
output (void *data, struct mad_header const *header, struct mad_pcm *pcm)
{
  unsigned int nchannels, nsamples;
  mad_fixed_t const *left_ch, *right_ch;

  /* pcm->samplerate contains the sampling frequency */

  nchannels = pcm->channels;
  nsamples = pcm->length;
  left_ch = pcm->samples[0];
  right_ch = pcm->samples[1];

  if (stat_dma == D_NONE)
    {
      i2s_DMABUF_SIZE
          = nsamples * 2
            * nchannels; // по 2 байта на 1 sample - на количество каналов
      i2s_DMABUF = malloc (i2s_DMABUF_SIZE * 2); // удвоенный для работы дма
      if (i2s_DMABUF == NULL)
        {
          printf ("error malloc i2s_DMABUF\r\n");
          return MAD_FLOW_BREAK;
        }
      mp3_BUF = malloc (i2s_DMABUF_SIZE); // для готового mp3
      if (mp3_BUF == NULL)
        {
          free (i2s_DMABUF);
          i2s_DMABUF = NULL;
          printf ("error malloc mp3_BUF\r\n");
          return MAD_FLOW_BREAK;
        }
      int ix = 0;
      while (nsamples--)
        {
          signed int sample;

          sample = scale (*left_ch++); // преобразовать в 16-bit
          i2s_DMABUF[ix++] = (sample >> 0) & 0xff;
          i2s_DMABUF[ix++] = (sample >> 8) & 0xff;

          if (nchannels == 2)
            {
              sample = scale (*right_ch++);
              i2s_DMABUF[ix++] = (sample >> 0) & 0xff;
              i2s_DMABUF[ix++] = (sample >> 8) & 0xff;
            }
        }

      s32 freq = pcm->samplerate; /*sample rate */
      s8 datawidth = 16;
      /*
      datawidth
      *    - \ref 8: 8 bit
      *    - \ref 16: 16 bit
      *    - \ref 24: 24 bit
      *    - \ref 32: 32 bit
      */

      s8 stereo = 2 - nchannels;
      /* *  stereo
       *  - \ref 0: stereo
       *  - \ref 1: mono
       */

      info_bits_per_sample = datawidth;
      if (stereo)
        info_channels = 1;

      s8 format = 0;
      /*  format
       *	- \ref 0: i2s
       *	- \ref 1: msb
       *	- \ref 2: pcma
       *	- \ref 3: pcmb
       */

      s8 tx_rx = 1; // 1: transmit
                    /*    tx_rx
                     *    - \ref 1: transmit
                     *    - \ref 2: receive
                     *    - \ref 3: duplex master
                     *    - \ref 4: duplex slave
                     */

      s8 mode = 1; // 1: dma
      /*   mode
       *    - \ref 0: interrupt
       *    - \ref 1: dma
       */

      // format:0, tx_en:1, freq:44100, datawidth:16, stereo:0, mode:1

      I2S_InitDef opts
          = { I2S_MODE_MASTER, I2S_CTRL_STEREO,   I2S_RIGHT_CHANNEL,
              I2S_Standard,    I2S_DataFormat_16, 8000,
              5000000 };

      opts.I2S_Mode_MS = (tx_rx - 1);      // master or slave mode
      opts.I2S_Mode_SS = (stereo << 22);   // stereo or single channel
      opts.I2S_Mode_LR = I2S_LEFT_CHANNEL; // left or right channel
      opts.I2S_Trans_STD
          = (format * 0x1000000); /* 0:i2s 1:msb 2:pcma 3:pcmb */
      opts.I2S_DataFormat
          = (datawidth / 8 - 1)
            * 0x10; /* I2S_DataFormat_8(8) I2S_DataFormat_16(16)
                       I2S_DataFormat_24(24) I2S_DataFormat_32(32) */
      opts.I2S_AudioFreq = freq;
      opts.I2S_MclkFreq = 80000000;

#ifdef SERIAL_DEBUG
      printf ("\r\n");
      printf ("format:%d, tx_en:%d, freq:%d, ", format, tx_rx, freq);
      printf ("datawidth:%d, stereo:%d, mode:%d\r\n", datawidth, stereo, mode);
      printf ("i2s_DMABUF:%d, mp3_BUF:%d, i2s_DMABUF_SIZE:%d\r\n",
              (int)i2s_DMABUF, (int)mp3_BUF, i2s_DMABUF_SIZE);
#endif
      wm_i2s_port_init (&opts);
      wm_i2s_register_callback (NULL);

      stat_dma = D_HW_INIT;
      return MAD_FLOW_CONTINUE;
    }

  if (stat_dma == D_HW_INIT)
    {
      int ix = i2s_DMABUF_SIZE;
      while (nsamples--)
        {
          signed int sample;

          sample = scale (*left_ch++);
          i2s_DMABUF[ix++] = (sample >> 0) & 0xff;
          i2s_DMABUF[ix++] = (sample >> 8) & 0xff;

          if (nchannels == 2)
            {
              sample = scale (*right_ch++);
              i2s_DMABUF[ix++] = (sample >> 0) & 0xff;
              i2s_DMABUF[ix++] = (sample >> 8) & 0xff;
            }
        }

      memset (&hdma_tx, 0, sizeof (wm_dma_handler_type));
      hdma_tx.XferCpltCallback = i2sDmaSendCpltCallback;
      hdma_tx.XferHalfCpltCallback = i2sDmaSendHalfCpltCallback;
      wm_i2s_transmit_dma (&hdma_tx, (u16 *)i2s_DMABUF, i2s_DMABUF_SIZE);

#ifdef SERIAL_DEBUG
      printf ("  D_PLAY\r\n");
#endif

      stat_dma = D_PLAY;
      return MAD_FLOW_CONTINUE;
    }

  if (stat_dma == D_PLAY)
    {
      /* Block waiting for the semaphore to become available. */
      if (xSemaphoreTake (xSemaphore, LONG_TIME) == pdTRUE)
        {
          int ix = 0;
          while (nsamples--)
            {
              signed int sample;

              sample = scale (*left_ch++);
              mp3_BUF[ix++] = (sample >> 0) & 0xff;
              mp3_BUF[ix++] = (sample >> 8) & 0xff;

              if (nchannels == 2)
                {
                  sample = scale (*right_ch++);
                  mp3_BUF[ix++] = (sample >> 0) & 0xff;
                  mp3_BUF[ix++] = (sample >> 8) & 0xff;
                }
            }
        }
#ifdef SERIAL_DEBUG
//      printf ("MAD_FLOW_CONTINUE\r\n");
#endif
      return MAD_FLOW_CONTINUE;
    }

  /* output sample(s) in 16-bit signed little-endian PCM */

  return MAD_FLOW_STOP;
}

/*
 * This is the error callback function. It is called whenever a decoding
 * error occurs. The error is indicated by stream->error; the list of
 * possible MAD_ERROR_* errors can be found in the mad.h (or stream.h)
 * header file.
 */

static enum mad_flow
error (void *data, struct mad_stream *stream, struct mad_frame *frame)
{
  struct buffer *buffer = data;

  fprintf (stderr, "decoding error 0x%04x (%s) at byte offset %u\n",
           stream->error, mad_stream_errorstr (stream),
           stream->this_frame - buffer->start);

  /* return MAD_FLOW_BREAK here to stop decoding (and propagate an error) */

  return MAD_FLOW_CONTINUE;
}

/*
 * This is the function called by main() above to perform all the decoding.
 * It instantiates a decoder object and configures it with the input,
 * output, and error callback functions above. A single call to
 * mad_decoder_run() continues until a callback function returns
 * MAD_FLOW_STOP (to stop decoding) or MAD_FLOW_BREAK (to stop decoding and
 * signal an error).
 */

static int
decode (unsigned char const *start, unsigned long length)
{
  struct buffer buffer;
  struct mad_decoder decoder;
  int result;

  /* initialize our private message structure */

  buffer.start = start;
  buffer.length = length;

  /* configure input, output, and error functions */

  mad_decoder_init (&decoder, &buffer, input, 0 /* header */, 0 /* filter */,
                    output, error, 0 /* message */);

  /* start decoding */

  result = mad_decoder_run (&decoder, MAD_DECODER_MODE_SYNC);

  /* release the decoder */

  mad_decoder_finish (&decoder);

  return result;
}
