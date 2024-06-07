#include "wm_type_def.h"

#include "wm_i2c.h"
#include "wm_regs.h"

#include "wm_gpio.h"
#include "wm_pmu.h"
#include "wm_timer.h"

#include <stdio.h>
#include <string.h>

#include "wm_gpio_afsel.h"

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

static u8 u8_volume = 0; //
static char buf_str_ind[10];

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
                  sprintf (buf_str_ind, "%.3d", u8_volume);
                  //n_i2s_PlayWav (FileName);
                }
              if (strstr (FileName, ".mp3") != NULL
                  || strstr (FileName, ".mp3") != NULL)
                {
                  sprintf (buf_str_ind, "%.3d", u8_volume);
                  n_i2s_PlayMp3 (FileName);
                }
            }
        }
      f_closedir (&dir);
    }

  return res;
}

//****************************************************************************************************//

//****************************************************************************************************//

void
user_app1_task (void *sdata)
{
  printf ("user_app1_task start\n");

  n_i2s_init_hw ();

  init_ff ();

  //
  //

  u8_volume = 50;
  n_i2s_SetVolume (u8_volume);

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
