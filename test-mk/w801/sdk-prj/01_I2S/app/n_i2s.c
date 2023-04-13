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

//#define SERIAL_DEBUG
//#define SERIAL_DEBUG_ALL

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

static u8 file_buffer[DEMO_DATA_SIZE * 2] = { 0 };
static u8 info_channels = 2;
static u8 info_bits_per_sample = 16;

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
i2sDmaSendHalfCpltCallback (wm_dma_handler_type *hdma)
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
  hdma_tx.XferCpltCallback = i2sDmaSendCpltCallback;
  hdma_tx.XferHalfCpltCallback = i2sDmaSendHalfCpltCallback;
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

  // s8 mode = 1; // 1: dma
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

#include "mp3_decode/minimp3.h"
#include "wm_mem.h"
#define MP3_FRAME_LEN 4 * 1152

static void *mp3_decoder = NULL;

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

  if (sizeof (file_buffer) < MP3_FRAME_LEN)
    {
      printf ("Error %d file_buffer < MP3_FRAME_LEN %d\r\n",
              sizeof (file_buffer), MP3_FRAME_LEN);
      return -55;
    }

  if (mp3_decoder == NULL)
    mp3_decoder = tls_mem_alloc (sizeof (mp3dec_t));

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

              res_sd = f_read (&fnew, file_buffer, MP3_FRAME_LEN, &fnum);
              
              u8 channel;
              s32 freq;/*    sample rate  */
              int bitrate_kbps;
              //    int frame_bytes, frame_offset, channels, hz, layer, bitrate_kbps;
              mp3dec_frame_info_t info;
              if (mp3dec_decode_frame (mp3_decoder, file_buffer, fnum, NULL,
                                       &info)
                  > 0)
                {
                  freq = info.hz;
                  channel = info.channels;
                  bitrate_kbps = info.bitrate_kbps;
                  printf ("mp3dec frame info frame_bytes=%d, frame_offset=%d, channels=%d, hz=%d, layer=%d, bitrate_kbps=%d\r\n",
                           info.frame_bytes, info.frame_offset,
                           info.channels, info.hz, info.layer, info.bitrate_kbps);
                }
              else
                {
                  printf ("error mp3dec_decode_frame\r\n");
                  f_close (&fnew);
                  return -7;
                }

              memset (mp3_decoder, 0, sizeof (mp3dec_t));
              mp3dec_init (mp3_decoder);

              if (channel != 2)
                {
                  printf ("Wrong `channelsNum` value, 2 expected\r\n");
                  f_close (&fnew);
                  return -8;
                }


              if ((freq != 44100)
                  || (bitrate_kbps != 44100 * 2 * 2)
                       )
                {
                  printf ("Wrong file format, 16 bit file with sample "
                          "rate 44100 expected\r\n");
                  f_close (&fnew);
                  return -9;
                }

              s8 datawidth = 8 * bitrate_kbps / channel / freq;
              /*    datawidth
               *    - \ref 8: 8 bit
               *    - \ref 16: 16 bit
               *    - \ref 24: 24 bit
               *    - \ref 32: 32 bit
               */

              s8 stereo = (2 - channel);
              /* 	 stereo
               *    - \ref 0: stereo
               *	  - \ref 1: mono
               */


              /*
                            fnum = MP3_FRAME_LEN;
                            my_sost = VS1053_PLAY;
                            VS1053_data_mode_on ();
                            while (fnum == MP3_FRAME_LEN
                                   && my_sost == VS1053_PLAY) // More to do?
                              {
                                VS1053_await_data_request (); // Wait for space
                 available res_sd = f_read (&fnew, file_buffer, MP3_FRAME_LEN,
                 &fnum); SPI_writeBytes (file_buffer, fnum);
                              }
                            VS1053_data_mode_off ();
                            my_sost = VS1053_STOP;

	mp3dec_frame_info_t info;

                           const uint8_t *input, uint32_t len,
                            int16_t *pcm, uint32_t *out_len,
                            uint32_t *hz, uint32_t *used)

	int result = mp3dec_decode_frame(decoder, input, len, pcm, &info);
	*hz = info.hz;
	*out_len = (result * info.channels * 2);
	*used = info.frame_bytes;
	return result;



				result = mp3_decoder_get_data(coder->mp3_decoder, coder->buff.addr + pos, coder->buff.used - pos, out_buff->addr + out_buff->used, &out_len, &hz, &used);
				if (result > 0)
				{
					out_buff->used += out_len;
				}

//				if (!result) {
//					LLOGD("jump %dbyte", info.frame_bytes);
//				}
				pos += used;
				if ((out_buff->len - out_buff->used) < (MINIMP3_MAX_SAMPLES_PER_FRAME * 2))
				{
					break;
				}


              res_sd
                  = f_read (&fnew, file_buffer, sizeof (file_buffer), &fnum);
              applyVolume (file_buffer, sizeof (file_buffer));
#ifdef SERIAL_DEBUG
              printf ("load fnum:%d, ", fnum);
#endif
              tls_i2s_send (freq, datawidth, stereo);

              */
            }
        }
      // close file
      f_close (&fnew);
    }
  return res_sd;
}
