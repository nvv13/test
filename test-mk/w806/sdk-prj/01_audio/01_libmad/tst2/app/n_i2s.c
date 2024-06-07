/*

Volume Algoritm used from
https://github.com/pschatzmann/arduino-audio-tools.git

*/

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "wm_gpio.h"
#include "wm_gpio_afsel.h"
#include "wm_i2c.h"
#include "wm_pmu.h"
#include "wm_regs.h"

#include "wm_type_def.h"

#include "n_i2s.h"

// #define SERIAL_DEBUG
// #define SERIAL_DEBUG_ALL

// #define DEMO_DATA_SIZE (4096+2048)
// #define MP3_FRAME_LEN 4 * 1152
#define DEMO_DATA_SIZE (6 * 1152)


static enum n_i2s_status my_sost = N_I2S_NONE;

enum n_i2s_status
n_i2s_get_status (void)
{
  return my_sost;
};
void
n_i2s_stop (void)
{
  if (my_sost != N_I2S_STOP)
    my_sost = N_I2S_QUERY_TO_STOP;
};

static FIL fnew;       // file object
static FRESULT res_sd; // file operation results
static UINT fnum;      // The number of files successfully read and written

static u8 file_buffer[DEMO_DATA_SIZE * 2] = { 0 };
static u8 info_channels = 2;
static u8 info_bits_per_sample = 16;

#include "mod1/mad.h"
#include "wm_mem.h"
//#define MP3_FRAME_LEN 4 * 1152
//static u8 file_buf_mp3[MP3_FRAME_LEN] = { 0 };
//static u16 mp3_hz;
//static u16 mp3_out_len = 0;
//static u16 mp3_used = 0;

#include "wm_i2s.h"

static s32_t max_value;

/// provides the biggest number for the indicated number of bits
static s32_t
maxValue (int value_bits_per_sample)
{
  switch (value_bits_per_sample)
    {
    case 8:
      return 127;
    case 16:
      return 32767;
    case 24:
      return 8388607;
    case 32:
      return 2147483647;
    }
  return 32767;
}

float factor_for_channel[] = { 1.0, 1.0 };

static void
applyVolume16 (s16_t *data, size_t size)
{
  for (size_t j = 0; j < size; j++)
    {
      s16_t result = factor_for_channel[(j % info_channels ? 0 : 1)] * data[j];
      if (true) //! info.allow_boost)
        {
          if (result > max_value)
            result = max_value;
          if (result < -max_value)
            result = -max_value;
        }
      data[j] = result;
    }
}

/*static void
applyVolume24 (int24_t *data, size_t size)
{
  for (size_t j = 0; j < size; j++)
    {
      float result = factorForChannel (j % info.channels) * data[j];
      if (1)//!info.allow_boost)
        {
          if (result > max_value)
            result = max_value;
          if (result < -max_value)
            result = -max_value;
        }
      int32_t result1 = result;
      data[j] = static_cast<int24_t> (result1);
    }
}
*/

static void
applyVolume32 (s32_t *data, size_t size)
{
  for (size_t j = 0; j < size; j++)
    {
      s32_t result = factor_for_channel[(j % info_channels ? 0 : 1)] * data[j];
      if (true) //! info.allow_boost)
        {
          if (result > max_value)
            result = max_value;
          if (result < -max_value)
            result = -max_value;
        }
      data[j] = result;
    }
}

static void
applyVolume (const uint8_t *buffer, size_t size)
{
  switch (info_bits_per_sample)
    {
    case 16:
      applyVolume16 ((int16_t *)buffer, size / 2);
      break;
    case 24:
      // applyVolume24 ((int24_t *)buffer, size / 3);
      break;
    case 32:
      applyVolume32 ((int32_t *)buffer, size / 4);
      break;
      // default:
      // printf("Unsupported bits_per_sample: %d", info.bits_per_sample);
    }
}

/**************************************************/

#define SERIAL_DEBUG

static int decode (unsigned char const *, unsigned long);

static u16 i2s_DMABUF_SIZE = 0;
static u8 *i2s_DMABUF = NULL;
static u8 *mp3_BUF = NULL;

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



/**************************************************/
static void
i2sWavDmaSendCpltCallback (wm_dma_handler_type *hdma)
{
  if (my_sost == N_I2S_END_FILE)
    {
      memset ((file_buffer + DEMO_DATA_SIZE), 0, DEMO_DATA_SIZE);
    }
  else
    {
      res_sd = f_read (&fnew, (file_buffer + DEMO_DATA_SIZE), DEMO_DATA_SIZE,
                       &fnum);
      applyVolume ((file_buffer + DEMO_DATA_SIZE), fnum);
      if (fnum < DEMO_DATA_SIZE)
        {
          my_sost = N_I2S_END_FILE;
          memset ((file_buffer + DEMO_DATA_SIZE + fnum), 0,
                  (DEMO_DATA_SIZE - fnum));
        }
    }
}
static void
i2sWavDmaSendHalfCpltCallback (wm_dma_handler_type *hdma)
{

  if (my_sost == N_I2S_END_FILE)
    {
      memset (file_buffer, 0, DEMO_DATA_SIZE);
    }
  else
    {
      res_sd = f_read (&fnew, file_buffer, DEMO_DATA_SIZE, &fnum);
      applyVolume (file_buffer, fnum);
      if (fnum < DEMO_DATA_SIZE)
        {
          my_sost = N_I2S_END_FILE;
          memset ((file_buffer + fnum), 0, (DEMO_DATA_SIZE - fnum));
        }
    }
}
static void
tls_i2s_tx_dma_demo ()
{
  memset (&hdma_tx, 0, sizeof (wm_dma_handler_type));
  hdma_tx.XferCpltCallback = i2sWavDmaSendCpltCallback;
  hdma_tx.XferHalfCpltCallback = i2sWavDmaSendHalfCpltCallback;
  wm_i2s_transmit_dma (&hdma_tx, (u16 *)file_buffer, DEMO_DATA_SIZE);
#ifdef SERIAL_DEBUG_ALL
  printf ("dma transmit start\n");
#endif
}

static int
tls_i2s_send (s32 freq /*sample rate */
              ,
              s8 datawidth
              /*
             datawidth
              *    - \ref 8: 8 bit
              *    - \ref 16: 16 bit
              *    - \ref 24: 24 bit
              *    - \ref 32: 32 bit
              */
              ,
              s8 stereo
              /* *  stereo
               *  - \ref 0: stereo
               *  - \ref 1: mono
               */

)
{

  if (my_sost == N_I2S_NONE)
    n_i2s_init_hw ();

  max_value = maxValue (datawidth);
  info_bits_per_sample = datawidth;
  if (stereo)
    info_channels = 1;
  else
    info_channels = 2;

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

  I2S_InitDef opts = { I2S_MODE_MASTER, I2S_CTRL_STEREO,   I2S_RIGHT_CHANNEL,
                       I2S_Standard,    I2S_DataFormat_16, 8000,
                       5000000 };

  opts.I2S_Mode_MS = (tx_rx - 1);            // master or slave mode
  opts.I2S_Mode_SS = (stereo << 22);         // stereo or single channel
  opts.I2S_Mode_LR = I2S_LEFT_CHANNEL;       // left or right channel
  opts.I2S_Trans_STD = (format * 0x1000000); /* 0:i2s 1:msb 2:pcma 3:pcmb */
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
#endif
  wm_i2s_port_init (&opts);
  wm_i2s_register_callback (NULL);
  tls_i2s_tx_dma_demo ();
  my_sost = N_I2S_PLAY;
  while (my_sost == N_I2S_PLAY)
    {
      tls_os_time_delay (HZ);
    }
  wm_i2s_tx_rx_stop ();

  return WM_SUCCESS;
}

void
n_i2s_init_hw (void)
{
  wm_i2s_ck_config (WM_IO_PA_08); // clock line         i2s Bclk
  wm_i2s_ws_config (WM_IO_PA_09); // word select line   i2s LRclk
  wm_i2s_do_config (WM_IO_PA_10); // Dout
  wm_i2s_di_config (WM_IO_PA_11); // Din

  my_sost = N_I2S_HW_INIT;

  // no need
  // wm_i2s_mclk_config (WM_IO_PA_00);// only PA0  ,The clock is still the
  // internal 160MHz clock, whether the MCLK clock is turned on, and the
  // operating frequencies of MCLK and BCLK

  // no need
  // wm_i2s_extclk_config (WM_IO_PA_07);
  // only PA7,The working clock configuration of the I2S module can be
  // configured by setting the 0x40000718 register in the clock and reset
  // module to select the external clock source.
}

/*
Volume Algoritm used from
https://github.com/pschatzmann/arduino-audio-tools.git
*/
void
n_i2s_SetVolume (u8 procVol)
{
  if (procVol >= 100)
    {
      factor_for_channel[0] = 1.0;
      factor_for_channel[1] = 1.0;
    }
  else
    {
      if (procVol == 0)
        {
          factor_for_channel[0] = 0.0;
          factor_for_channel[1] = 0.0;
        }
      else
        {
          float input = procVol;
          input = input / 100;
#ifdef SERIAL_DEBUG_ALL
          printf ("0 input %f\n", input);
#endif

          float ym = 0.1;
          float b = pow (((1 / ym) - 1), 2);
          float a = 1.0f / (b - 1.0f);
          float volumeFactor = pow (b, input) * a - a;

          // float volumeFactor = pow (2.0, input) - 1.0;

#ifdef SERIAL_DEBUG_ALL
          printf ("1 volumeFactor %f\n", volumeFactor);
#endif

          if (volumeFactor > 1.0)
            volumeFactor = 1.0;
          if (volumeFactor < 0.0)
            volumeFactor = 0.0;

#ifdef SERIAL_DEBUG_ALL
          printf ("2 volumeFactor %f\n", volumeFactor);
#endif

          factor_for_channel[0] = volumeFactor;
          factor_for_channel[1] = volumeFactor;
        }
    }
}

// pack(push,1) - Byte alignment ?
#pragma pack(push, 1)
// Wav Header
typedef struct tagWAVHDR
{
  char riff[4];  // = "RIFF" = 0x46464952
  u32 size_8;    // = FileSize - 8                 fileSize
  char wave[4];  // = "WAVE" = 0x45564157
  char fmt[4];   // = "fmt " = 0x20746D66
  u32 dwFmtSize; // = size of next structure: 16   headerSizeLeft

  u16 format_tag;        // = PCM : 1  audioFormat, (1 = no compression)
  u16 channels;          // = number of channels: 1 mono, 2 dual channels
  u32 samples_per_sec;   // = sample rate       sampleRate
  u32 avg_bytes_per_sec; // = bytes per second  bytesPerSecond
  u16 block_align;       // = bytes per sample point: wBitsPerSample / 8
                         // bytesPerSample
  u16 bits_per_sample;   // = Quantization bits: 8 | 16 bitsPerSamplePerChannel

  char data[4];  // = "data" = 0x61746164
  u32 data_size; // = file length without file header   dataSize
} WAVHDR, *PWAVHDR;
#pragma pack(pop)

FRESULT
n_i2s_PlayWav (char *filename)
{

  if (sizeof (WAVHDR) != 44)
    {
      printf ("Wrong sizeof `WAVHDR` value, 44 expected\r\n");
      return -5;
    }

  // Open the file
  res_sd = f_open (&fnew, filename, FA_OPEN_EXISTING | FA_READ);
  // file opened successfully?
  if (res_sd == FR_OK)
    {
#ifdef SERIAL_DEBUG_ALL
      printf ("Open file successfully! Start reading data!\r\n");
#endif
      res_sd = f_read (&fnew, file_buffer, sizeof (WAVHDR), &fnum);
      if (res_sd == FR_OK)
        {
          // Parse BMP header to get the information we need
          PWAVHDR aHead = (PWAVHDR)file_buffer;
          if (strstr (aHead->riff, "RIFF") != NULL
              && strstr (aHead->wave, "WAVE") != NULL
              && strstr (aHead->fmt, "fmt ") != NULL
              && strstr (aHead->data, "data") != NULL)
            {
#ifdef SERIAL_DEBUG_ALL
              printf ("wav header ok\r\n");
#endif

              printf ("format_tag:%d, dwFmtSize:%d, channels:%d, ",
                      aHead->format_tag, aHead->dwFmtSize, aHead->channels);
              printf (
                  "samples_per_sec:%d, block_align:%d, bits_per_sample:%d\r\n",
                  aHead->samples_per_sec, aHead->block_align,
                  aHead->bits_per_sample);
              printf ("avg_bytes_per_sec:%d, data_size:%d, ",
                      aHead->avg_bytes_per_sec, aHead->data_size);

              if (aHead->dwFmtSize != 16)
                {
                  printf ("Wrong `headerSizeLeft` value, 16 expected\r\n");
                  f_close (&fnew);
                  return -6;
                }

              if (aHead->format_tag != 1)
                {
                  printf ("Wrong `compression` value, 1 expected\r\n");
                  f_close (&fnew);
                  return -7;
                }

              if (aHead->channels > 2 || aHead->channels < 1)
                {
                  printf ("Wrong `channelsNum` value, 2 expected\r\n");
                  f_close (&fnew);
                  return -8;
                }

              if ((aHead->samples_per_sec != 44100)
                  || (aHead->block_align != 2 * aHead->channels)
                  || (aHead->bits_per_sample != 16)
                  || (aHead->avg_bytes_per_sec != 44100 * 2 * aHead->channels)
                  || (aHead->data_size < sizeof (file_buffer)))
                {
                  printf ("Wrong file format, 16 bit file with sample "
                          "rate 44100 expected\r\n");
                  f_close (&fnew);
                  return -9;
                }

              s32 freq = aHead->samples_per_sec; /*    sample rate  */
              s8 datawidth = aHead->bits_per_sample;
              /*    datawidth
               *    - \ref 8: 8 bit
               *    - \ref 16: 16 bit
               *    - \ref 24: 24 bit
               *    - \ref 32: 32 bit
               */

              s8 stereo = (2 - aHead->channels);
              /* 	 stereo
               *    - \ref 0: stereo
               *	  - \ref 1: mono
               */

              /*
                            uint32_t bmpImageoffset = 1024 * 1024 * 4;
                            res_sd
                                = f_lseek (&fnew,
                                           bmpImageoffset
                                );
              #ifdef SERIAL_DEBUG
                            printf ("f_lseek successfully! %d\r\n",
              bmpImageoffset); #endif
              */

              res_sd
                  = f_read (&fnew, file_buffer, sizeof (file_buffer), &fnum);
              applyVolume (file_buffer, sizeof (file_buffer));
#ifdef SERIAL_DEBUG
              printf ("load fnum:%d, ", fnum);
#endif
              tls_i2s_send (freq, datawidth, stereo);
            }
        }
      // close file
      f_close (&fnew);
    }
  return res_sd;
}

// pack(push,1) - Byte alignment ?
#pragma pack(push, 1)
// MP3 Header
typedef struct tagMP3
{
  char ID3[3]; // = "ID3"
  u8 ver;
  u8 sum_ver;
  u8 flag;
  u8 header_size[4];

} MP3HDR, *PMP3HDR;
#pragma pack(pop)

FRESULT
n_i2s_PlayMp3 (char *filename)
{

//  if (sizeof (file_buf_mp3) < MP3_FRAME_LEN)
//    {
//      printf ("Error %d file_buf_mp3 < MP3_FRAME_LEN %d\r\n",
//              sizeof (file_buf_mp3), MP3_FRAME_LEN);
//      return -55;
//    }

  uint32_t start;

  // Open the file
  res_sd = f_open (&fnew, filename, FA_OPEN_EXISTING | FA_READ);
  // file opened successfully?
  if (res_sd == FR_OK)
    {
#ifdef SERIAL_DEBUG_ALL
      printf ("Open file successfully! Start reading data!\r\n");
#endif
      res_sd = f_read (&fnew, file_buffer, sizeof (MP3HDR), &fnum);
      if (res_sd == FR_OK)
        {
          // Parse BMP header to get the information we need
          PMP3HDR aHead = (PMP3HDR)file_buffer;
          if (strstr (aHead->ID3, "ID3") != NULL)
            {
#ifdef SERIAL_DEBUG_ALL
              printf ("MP3 header ok\r\n");
#endif
              if (!(aHead->ver > 0 && aHead->ver < 6))
                {
                  printf ("Wrong `version` value %d\r\n", aHead->ver);
                  f_close (&fnew);
                  return -7;
                }

              start = 0ul;
              start |= (0x7F & aHead->header_size[0]);
              start <<= 7;
              start |= (0x7F & aHead->header_size[1]);
              start <<= 7;
              start |= (0x7F & aHead->header_size[2]);
              start <<= 7;
              start |= (0x7F & aHead->header_size[3]);

              res_sd = f_lseek (&fnew, start);
#ifdef SERIAL_DEBUG
              printf ("f_lseek successfully! %d\r\n", start);
#endif

              res_sd = f_read (&fnew, file_buffer, DEMO_DATA_SIZE*2, &fnum);

              stat_dma = D_NONE;
              xSemaphore = xSemaphoreCreateBinaryStatic (&xSemaphoreBuffer);
              if (xSemaphoreGive (xSemaphore) != pdTRUE) // Сделать иначе не
                                                         // инициализирован
                {
                  // We would expect this call to fail because we cannot give
                  // a semaphore without first "taking" it!
                }

              decode (file_buffer, fnum);

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
            }
        }
      // close file
      f_close (&fnew);
    }
  return res_sd;
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


  res_sd = f_read (&fnew, file_buffer, DEMO_DATA_SIZE * 2, &fnum);
  //res_sd = f_read (&fnew, file_buffer + DEMO_DATA_SIZE, DEMO_DATA_SIZE, &fnum);
  if (res_sd == FR_OK)
    buffer->length = fnum;
  else
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
