/*****************************************************************************
 *
 * File Name : main.c
 *
 * Description: main
 *
 * Date : 2022-12-21
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wm_type_def.h"

#include "wm_cpu.h"

#include "wm_gpio.h"
#include "wm_gpio_afsel.h"
#include "wm_osal.h"

#include "VS1053/VS1053.h"

#define USER_APP1_TASK_SIZE 2048
static OS_STK UserApp1TaskStk[USER_APP1_TASK_SIZE];
#define USER_APP1_TASK_PRIO 32

#include "ff.h"

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
              if (strstr (FileName, ".mp3") != NULL
                  || strstr (FileName, ".MP3") != NULL)
                {
                  // n_i2s_PlayWav (FileName);
                }
            }
        }
      f_closedir (&dir);
    }

  return res;
}

#define VOLUME 100 // volume level 0-100

#include "SampleMp3.h"

void
user_app1_task (void *sdata)
{
  printf ("user_app1_task start\n");

  libVS1053_t user_data = {

    .rst_pin = WM_IO_PB_17,  /* HW reset pin */
    .cs_pin = WM_IO_PB_21,   /* ->xcs  SCI Chip Select pin */
    .dcs_pin = WM_IO_PB_22,  /* ->xdcs SDI Chip Select pin */
    .dreq_pin = WM_IO_PB_18, /* <-dreq Data Request pin */

    .spi_cs = WM_IO_PB_23, /* */
    .spi_ck = WM_IO_PB_24, /*      ck -> sck Clock pin */
    .spi_di = WM_IO_PB_03, /* master miso di <- miso slave, на макетке board
                              HLK-W801-KIT-V1.1 работает только WM_IO_PB_03  */
    .spi_do = WM_IO_PB_26, /* master mosi do -> mosi slave */
  };

  /**
   * config the pins used for spi di
   * WM_IO_PB_00 - не работает,
   * WM_IO_PB_03 - работает!
   * WM_IO_PB_16 only for 56pin - не работает, мешает светодиод подключенный к
   * данному контакту на макетке
   * WM_IO_PB_25 only for 56pin - не работает, мешает светодиод подключенный к
   * данному контакту на макетке
   */

  VS1053_VS1053 (&user_data);
  // initialize SPI
  //    SPI.begin();
  VS1053_begin ();
  VS1053_printDetails ("reg value read ");
  VS1053_sineTest (N_Hz_3000,
                   5000); // Make a tone to indicate VS1053 is working
  if (VS1053_getChipVersion () == 4)
    { // Only perform an update if we really are using a VS1053, not. eg.
      // VS1003
      VS1053_loadDefaultVs1053Patches ();
    }
  VS1053_switchToMp3Mode (); // optional, some boards require this
  VS1053_setVolume (VOLUME);

  FATFS fs;
  FRESULT res_sd;
  char buff[256]; // буффер для названия директории при сканировании файловой
                  // системы
  // wm_sdio_host_config (0);

  while (1)
    { //
      VS1053_printDetails ("reg value read ");
      tls_os_time_delay (2);

      VS1053_playChunk (sampleMp3, sizeof (sampleMp3));

      /*
            // mount SD card
            res_sd = f_mount (&fs, "0:", 1);

            //***********************formatting
         test**************************** if (res_sd == FR_NO_FILESYSTEM)
              {
                printf ("FR_NO_FILESYSTEM:Failed to mount file system! Probably
         " "because the file " "initialization failed! error code:%d\r\n",
                        res_sd);
              }
            else if (res_sd != FR_OK)
              {
                printf ("Failed to mount file system! Probably because the file
         " "initialization failed! error code:%d\r\n", res_sd);
              }
            else
              {
                printf ("The file system is successfully mounted, and the read
         and " "write test can be performed!\r\n");
              }

            if (res_sd == FR_OK)
              {
                memset (buff, 0, sizeof (buff));
                strcpy (buff, "/");
                res_sd = scan_files (buff);
              }

            // unmount file system
            f_mount (NULL, "0:", 1);
      */
      tls_os_time_delay (HZ * 1);

    } //
}

void
UserMain (void)
{
  printf ("UserMain start");
  tls_sys_clk_set (CPU_CLK_240M);

  tls_os_task_create (NULL, NULL, user_app1_task, NULL,
                      (void *)UserApp1TaskStk, /* task's stack start address */
                      USER_APP1_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      USER_APP1_TASK_PRIO, 0);
}
