#include "wm_type_def.h"

#include "wm_i2c.h"
#include "wm_regs.h"

#include "wm_gpio.h"
#include "wm_pmu.h"

#include <stdio.h>
#include <string.h>

#include "wm_gpio_afsel.h"

#include "mod1/UTFT.h"
#include "n_delay.h" //нужна для UTFT

#include "ff.h"

#define SERIAL_DEBUG
#define SERIAL_DEBUG_ALL

#define USER_APP1_TASK_SIZE 2048
static OS_STK UserApp1TaskStk[USER_APP1_TASK_SIZE];
#define USER_APP1_TASK_PRIO 32

//#define BEF_LEN_3X 510 // 170*3
//#define BEF_LEN_2X 512 // 256*2
// u8 file_buffer[65536] = { 0 };
u8 file_buffer[32768] = { 0 };

#include "wm_i2s.h"

#define DEMO_DATA_SIZE (1024)

// extern int wm_i2s_tranceive_dma (uint32_t i2s_mode,
//                                 wm_dma_handler_type *hdma_tx,
//                                 wm_dma_handler_type *hdma_rx,
//                                 uint16_t *data_tx, uint16_t *data_rx,
//                                 uint16_t len);

// enum
//{
//  WM_I2S_MODE_INT,
//  WM_I2S_MODE_DMA
//};

// enum
//{
//  WM_I2S_TX = 1,
//  WM_I2S_RX = 2,
//  WM_I2S_TR = 3,
//  WM_I2S_TR_SLAVE = 4,
//};

u16 i2s_demo_tx[DEMO_DATA_SIZE] = { 0 };

static uint32_t g_tx_buff_val = 0;
static wm_dma_handler_type hdma_tx;
static void
i2sDmaSendCpltCallback (wm_dma_handler_type *hdma)
{
  int i = DEMO_DATA_SIZE / 2;
  for (; i < DEMO_DATA_SIZE; i++)
    {
      i2s_demo_tx[i] = ((u16 *)file_buffer)[g_tx_buff_val++];
    }
}
static void
i2sDmaSendHalfCpltCallback (wm_dma_handler_type *hdma)
{
  int i = 0;
  for (; i < DEMO_DATA_SIZE / 2; i++)
    {
      i2s_demo_tx[i] = ((u16 *)file_buffer)[g_tx_buff_val++];
    }
}
static void
tls_i2s_tx_dma_demo ()
{
  g_tx_buff_val = 0;
  for (u16 len = 0; len < DEMO_DATA_SIZE; len++)
    {
      i2s_demo_tx[len] = ((u16 *)file_buffer)[g_tx_buff_val++];
    }
#if 0
	wm_i2s_tx_dma((int16_t *)i2s_demo_tx, DEMO_DATA_SIZE, NULL);
	printf("send %d\r\n", DEMO_DATA_SIZE);
#else
  memset (&hdma_tx, 0, sizeof (wm_dma_handler_type));
  hdma_tx.XferCpltCallback = i2sDmaSendCpltCallback;
  hdma_tx.XferHalfCpltCallback = i2sDmaSendHalfCpltCallback;
  wm_i2s_transmit_dma (&hdma_tx, i2s_demo_tx, DEMO_DATA_SIZE);
  printf ("dma transmit start\n");
#endif
}

/**
 * @brief
 *
 * @param[in]  freq
 *    sample rate
 *
 * @param[in]  datawidth
 *    - \ref 8: 8 bit
 *    - \ref 16: 16 bit
 *    - \ref 24: 24 bit
 *    - \ref 32: 32 bit
 *
 * @param[in]  stereo
 *    - \ref 0: stereo
 *	  - \ref 1: mono
 *
 */
int
tls_i2s_send (s32 freq, s8 datawidth, s8 stereo)
{
  s8 format = 0; // 0: i2s
  s8 tx_rx = 1;  // 1: transmit
  s8 mode = 1;   // 1: dma

  I2S_InitDef opts = { I2S_MODE_MASTER, I2S_CTRL_STEREO,   I2S_RIGHT_CHANNEL,
                       I2S_Standard,    I2S_DataFormat_16, 8000,
                       5000000 };

  opts.I2S_Mode_MS = (tx_rx - 1);      // master or slave mode
  opts.I2S_Mode_SS = (stereo << 22);   // stereo or single channel
  opts.I2S_Mode_LR = I2S_LEFT_CHANNEL; // left or right channel
  opts.I2S_Trans_STD = (format * 0x1000000);
  opts.I2S_DataFormat = (datawidth / 8 - 1) * 0x10;
  opts.I2S_AudioFreq = freq;
  opts.I2S_MclkFreq = 80000000;

  printf ("\r\n");
  printf ("format:%d, tx_en:%d, freq:%d, ", format, tx_rx, freq);
  printf ("datawidth:%d, stereo:%d, mode:%d\r\n", datawidth, stereo, mode);
  wm_i2s_port_init (&opts);
  wm_i2s_register_callback (NULL);
  tls_i2s_tx_dma_demo ();

  tls_os_time_delay (HZ * 10);
  wm_i2s_tx_rx_stop ();

  return WM_SUCCESS;
}


void
init_i2s (void)
{
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
}

void
init_ff (void)
{
  wm_sdio_host_config (0);
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
WAV_loadWav (char *filename)
{

  FIL fnew;       // file object
  FRESULT res_sd; // file operation results
  UINT fnum;      // The number of files successfully read and written

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
                  || (aHead->data_size < sizeof (i2s_demo_tx)))
                {
                  printf ("Wrong file format, 16 bit file with sample "
                          "rate 44100 expected\r\n");
                  f_close (&fnew);
                  return -9;
                }

              s32 freq = aHead->samples_per_sec;
              s8 datawidth = aHead->bits_per_sample;
              s8 stereo = aHead->channels;

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

extern uint8_t BigFont[]; // подключаем большой шрифт

FRESULT
scan_files (
    char *path /* Start node to be scanned (***also used as work area***) */
)
{ /* http://elm-chan.org/fsw/ff/doc/readdir.html */
  FRESULT res;
  DIR dir;
  UINT i;
  static FILINFO fno;

  res = f_opendir (&dir, path); /* Open the directory */
  if (res == FR_OK)
    {
      for (;;)
        {
          res = f_readdir (&dir, &fno); /* Read a directory item */
          if (res != FR_OK || fno.fname[0] == 0)
            break; /* Break on error or end of dir */
          if (fno.fattrib & AM_DIR)
            { /* It is a directory */
              i = strlen (path);
              sprintf (&path[i], "/%s", fno.fname);
              res = scan_files (path); /* Enter the directory */
              if (res != FR_OK)
                break;
              path[i] = 0;
            }
          else
            { /* It is a file. */
              printf ("%s/%s\n", path, fno.fname);
              char FileName[256];
              if (strlen (path) != 0)
                sprintf (FileName, "0:%s/%s", path, fno.fname);
              else
                sprintf (FileName, "0:%s", fno.fname);
              UTFT_clrScr (); // стираем всю информацию с дисплея
              UTFT_setFont (BigFont);
              UTFT_setColor2 (VGA_FUCHSIA);
              UTFT_print (FileName, CENTER, 300, 0);
              if (strstr (FileName, ".wav") != NULL
                  || strstr (FileName, ".WAV") != NULL)
                {
                  WAV_loadWav (FileName);
                }
            }
        }
      f_closedir (&dir);
    }

  return res;
}

void
user_app1_task (void *sdata)
{
  printf ("user_app1_task start\n");

  init_i2s ();

  init_ff ();

  // подключаем библиотеку UTFT
  UTFT_UTFT (TFT32MEGA_2, (u8)WM_IO_PA_01, (u8)WM_IO_PA_02, (u8)WM_IO_PA_03,
             (u8)WM_IO_PA_04, 0);
  UTFT_InitLCD (LANDSCAPE); // инициируем дисплей
  UTFT_clrScr (); // стираем всю информацию с дисплея

  FATFS fs;
  FRESULT res_sd;
  char buff[256]; // буффер для названия директории при сканировании файловой
                  // системы
  while (1)
    { //
      // mount SD card
      res_sd = f_mount (&fs, "0:", 1);

      //***********************formatting test****************************
      if (res_sd == FR_NO_FILESYSTEM)
        {
          printf ("FR_NO_FILESYSTEM:Failed to mount file system! Probably "
                  "because the file "
                  "initialization failed! error code:%d\r\n",
                  res_sd);
        }
      else if (res_sd != FR_OK)
        {
          printf ("Failed to mount file system! Probably because the file "
                  "initialization failed! error code:%d\r\n",
                  res_sd);
        }
      else
        {
          printf ("The file system is successfully mounted, and the read and "
                  "write test can be performed!\r\n");
        }

      if (res_sd == FR_OK)
        {
          memset (buff, 0, sizeof (buff));
          strcpy (buff, "/");
          res_sd = scan_files (buff);
        }

      // unmount file system
      f_mount (NULL, "0:", 1);
      tls_os_time_delay (HZ * 1);
    } //
}

void
UserMain (void)
{

  printf ("UserMain start");
  tls_sys_clk_set (CPU_CLK_240M);
  //tls_sys_clk_set (CPU_CLK_160M);

  tls_os_task_create (NULL, NULL, user_app1_task, NULL,
                      (void *)UserApp1TaskStk, /* task's stack start address */
                      USER_APP1_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      USER_APP1_TASK_PRIO, 0);
}
