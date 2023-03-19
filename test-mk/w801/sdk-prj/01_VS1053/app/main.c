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
#include "wm_timer.h"

#include "wm_gpio.h"
#include "wm_gpio_afsel.h"
#include "wm_osal.h"

#include "mod1/u8g2.h"
#include "u8x8_riotos.h"

static u8g2_t u8g2;


#include "VS1053/VS1053.h"

#define USER_APP1_TASK_SIZE 2048
static OS_STK UserApp1TaskStk[USER_APP1_TASK_SIZE];
#define USER_APP1_TASK_PRIO 32

#include "ff.h"


static void
demo_timer_irq (u8 *arg) // здесь будет смена режима
{
if(VS1053_status_get_status()==VS1053_PLAY)
 {
 VS1053_stop_PlayMP3();
 };
}



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
              // printf ("%s/%s\n", path, fno.fname);
              char FileName[256];
              if (strlen (path) != 0)
                sprintf (FileName, "0:%s/%s", path, fno.fname);
              else
                sprintf (FileName, "0:%s", fno.fname);
              if (strstr (FileName, ".mp3") != NULL
                  || strstr (FileName, ".MP3") != NULL)
                {
                  printf ("FileName = %s \n", FileName);

                  u8g2_FirstPage (&u8g2);
                  do
                    {
                      u8g2_SetDrawColor (&u8g2, 1);
                      u8g2_SetFont (&u8g2, u8g2_font_courR08_tr);
                      u8g2_DrawStr (&u8g2, 0, 20, fno.fname);
                      if(strlen(fno.fname)>20)u8g2_DrawStr (&u8g2, 0, 35, fno.fname+20);
                      if(strlen(fno.fname)>40)u8g2_DrawStr (&u8g2, 0, 50, fno.fname+40);
                    }
                  while (u8g2_NextPage (&u8g2));

                  VS1053_PlayMp3 (FileName);
                  tls_os_time_delay (HZ);
                }
            }
        }
      f_closedir (&dir);
    }

  return res;
}

#define VOLUME 100 // volume level 0-100

void
user_app1_task (void *sdata)
{
  printf ("user_app1_task start\n");

  u8 timer_id;
  struct tls_timer_cfg timer_cfg;
  timer_cfg.unit = TLS_TIMER_UNIT_MS;
  // timer_cfg.unit = TLS_TIMER_UNIT_US; // чтобы небыло мерцания на
  // минимальной яркости, пришлось сделать время таймера поменьше
  // timer_cfg.timeout = 100; // 0 * 30;
  timer_cfg.timeout = 1000 * 10;
  timer_cfg.is_repeat = 1;
  timer_cfg.callback = (tls_timer_irq_callback)demo_timer_irq;
  timer_cfg.arg = NULL;
  timer_id = tls_timer_create (&timer_cfg);
  if (true)
    {
      tls_timer_start (timer_id);
      printf ("timer start\n");
    }

  /* initialize to SPI */
  puts ("Initializing to I2C oled Display.");

  u8g2_Setup_sh1106_i2c_128x64_noname_f (
      &u8g2, U8G2_R0, u8x8_byte_hw_i2c_riotos, u8x8_gpio_and_delay_riotos);

  if (u8g2.u8x8.i2c_address == 255) // заменяем default на настоящий адрес
    u8g2.u8x8.i2c_address = 0x3C;

  u8x8_riotos_t d_data = {
    .pin_cs = GPIO_UNDEF,    //
    .pin_dc = GPIO_UNDEF,    //
    .pin_reset = GPIO_UNDEF, //

    .i2c_scl = WM_IO_PA_01, /* */
    .i2c_sda = WM_IO_PA_04, /* */
    .i2c_freq = 100000      /* частота i2c в герцах */

  };

  u8g2_SetUserPtr (&u8g2, &d_data);

  /* initialize the display */
  puts ("Initializing display.");

  u8g2_InitDisplay (&u8g2);
  u8g2_SetPowerSave (&u8g2, 0);

  /* vs1053 */
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
  VS1053_begin ();
  VS1053_setVolume (VOLUME);

  FATFS fs;
  FRESULT res_sd;
  char buff[256]; // буффер для названия директории при сканировании файловой
                  // системы
  wm_sdio_host_config (0);

  while (1)
    { //

      // mount SD card
      res_sd = f_mount (&fs, "0:", 1);

      //***********************formatting    test****************************
      if (res_sd == FR_NO_FILESYSTEM)
        {
          printf ("FR_NO_FILESYSTEM:Failed to mount file system! Probably "
                  "because the file initialization failed! error code:%d\r\n",
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

  tls_os_task_create (NULL, NULL, user_app1_task, NULL,
                      (void *)UserApp1TaskStk, /* task's stack start address */
                      USER_APP1_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      USER_APP1_TASK_PRIO, 0);
}
