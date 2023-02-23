#include <stdio.h>
#include <string.h>

#include "wm_gpio.h"
#include "wm_gpio_afsel.h"
#include "wm_i2c.h"
#include "wm_pmu.h"
#include "wm_regs.h"

#include "wm_type_def.h"

#include "n_i2s.h"

#define SERIAL_DEBUG
#define SERIAL_DEBUG_ALL

#define DEMO_DATA_SIZE (8192)

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

u8 file_buffer[DEMO_DATA_SIZE * 2] = { 0 };

#include "wm_i2s.h"

static wm_dma_handler_type hdma_tx;
static void
i2sDmaSendCpltCallback (wm_dma_handler_type *hdma)
{
  if (my_sost == N_I2S_END_FILE)
    {
      memset ((file_buffer + DEMO_DATA_SIZE), 0, DEMO_DATA_SIZE);
    }
  else
    {
      res_sd = f_read (&fnew, (file_buffer + DEMO_DATA_SIZE), DEMO_DATA_SIZE,
                       &fnum);
      if (fnum < DEMO_DATA_SIZE)
        {
          my_sost = N_I2S_END_FILE;
          memset ((file_buffer + DEMO_DATA_SIZE + fnum), 0,
                  (DEMO_DATA_SIZE - fnum));
        }
    }
}
static void
i2sDmaSendHalfCpltCallback (wm_dma_handler_type *hdma)
{

  if (my_sost == N_I2S_END_FILE)
    {
      memset (file_buffer, 0, DEMO_DATA_SIZE);
    }
  else
    {
      res_sd = f_read (&fnew, file_buffer, DEMO_DATA_SIZE, &fnum);
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
  hdma_tx.XferCpltCallback = i2sDmaSendCpltCallback;
  hdma_tx.XferHalfCpltCallback = i2sDmaSendHalfCpltCallback;
  wm_i2s_transmit_dma (&hdma_tx, (u16 *)file_buffer, DEMO_DATA_SIZE);
  printf ("dma transmit start\n");
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

  printf ("\r\n");
  printf ("format:%d, tx_en:%d, freq:%d, ", format, tx_rx, freq);
  printf ("datawidth:%d, stereo:%d, mode:%d\r\n", datawidth, stereo, mode);
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
#ifdef SERIAL_DEBUG
              printf ("wav header ok\r\n");
#endif

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

              if (aHead->channels != 2)
                {
                  printf ("Wrong `channelsNum` value, 2 expected\r\n");
                  f_close (&fnew);
                  return -8;
                }

              if ((aHead->samples_per_sec != 44100)
                  || (aHead->block_align != 4)
                  || (aHead->bits_per_sample != 16)
                  || (aHead->avg_bytes_per_sec != 44100 * 2 * 2)
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
