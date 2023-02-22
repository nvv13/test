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

#include "n_i2s.h"

void
init_ff (void)
{
  wm_sdio_host_config (0);
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
              if (strstr (FileName, ".wav") != NULL
                  || strstr (FileName, ".WAV") != NULL)
                {
                  UTFT_clrScr (); // стираем всю информацию с дисплея
                  UTFT_setFont (BigFont);
                  UTFT_setColor2 (VGA_FUCHSIA);
                  UTFT_print (FileName, CENTER, 300, 0);
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
  // tls_sys_clk_set (CPU_CLK_160M);

  tls_os_task_create (NULL, NULL, user_app1_task, NULL,
                      (void *)UserApp1TaskStk, /* task's stack start address */
                      USER_APP1_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      USER_APP1_TASK_PRIO, 0);
}
