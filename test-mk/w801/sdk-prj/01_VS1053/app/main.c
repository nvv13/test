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
#include "mod1/u8x8_riotos.h"
#include "mod1/encoder.h"

static u8g2_t u8g2;

#include "VS1053/VS1053.h"

#define USER_APP1_TASK_SIZE 2048
static OS_STK UserApp1TaskStk[USER_APP1_TASK_SIZE];
#define USER_APP1_TASK_PRIO 32

#include "ff.h"

static int i_type_file=0;

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
              if (((strstr (FileName, ".mp3") != NULL  || strstr (FileName, ".MP3")  != NULL) && i_type_file==0) ||
                  ((strstr (FileName, ".flac") != NULL || strstr (FileName, ".FLAC") != NULL) && i_type_file==1) ||
                  ((strstr (FileName, ".ogg") != NULL  || strstr (FileName, ".OGG")  != NULL) && i_type_file==2) ||
                  (
                   (strstr (FileName, ".m4a") != NULL  || 
                    strstr (FileName, ".M4A") != NULL  || 
                    strstr (FileName, ".3gp") != NULL  || 
                    strstr (FileName, ".3GP") != NULL  || 
                    strstr (FileName, ".3g2") != NULL  || 
                    strstr (FileName, ".3G2") != NULL  || 
                    strstr (FileName, ".mp4") != NULL  || 
                    strstr (FileName, ".MP4") != NULL  || 
                    strstr (FileName, ".aac") != NULL  || 
                    strstr (FileName, ".AAC") != NULL
                  ) && i_type_file==3) 
                 )
                {
                  printf ("FileName = %s \n", FileName);

                  u8g2_FirstPage (&u8g2);
                  do
                    {
                      u8g2_SetDrawColor (&u8g2, 1);
                      u8g2_SetFont (&u8g2, u8g2_font_courR08_tr);
                      // FF_LFN_BUF		255
                      if (strlen (fno.fname) > 80)
                        {
                          fno.fname[100] = 0;
                          u8g2_DrawStr (&u8g2, 0, 8 + 12 + 12 + 12 + 12,
                                        fno.fname + 80);
                        };
                      if (strlen (fno.fname) > 60)
                        {
                          fno.fname[80] = 0;
                          u8g2_DrawStr (&u8g2, 0, 8 + 12 + 12 + 12,
                                        fno.fname + 60);
                        };
                      if (strlen (fno.fname) > 40)
                        {
                          fno.fname[60] = 0;
                          u8g2_DrawStr (&u8g2, 0, 8 + 12 + 12, fno.fname + 40);
                        };
                      if (strlen (fno.fname) > 20)
                        {
                          fno.fname[40] = 0;
                          u8g2_DrawStr (&u8g2, 0, 8 + 12, fno.fname + 20);
                        };
                      fno.fname[20] = 0;
                      u8g2_DrawStr (&u8g2, 0, 8, fno.fname);
                    }
                  while (u8g2_NextPage (&u8g2));

                  if(i_type_file==0)
                    VS1053_PlayMp3 (FileName);
                    else
                    VS1053_PlayFlac (FileName);
                  tls_os_time_delay (HZ);
                }
            }
        }
      f_closedir (&dir);
    }

  return res;
}

#define VOLUME 100 // volume level 0-100

static const u16 i_pos_dreb_SW
    = 500; //кнопка,таймер 300 Мкс, значит будет 150 миллисекунд.
volatile static u8 i_dreb_SW = 0;     // от дребезга кнопки

static void
demo_timer_irq (u8 *arg) // здесь будет смена режима
{

  int i_enc_diff= get_encoder_diff();
  if (i_enc_diff!=0)//i_dreb_CLK != 0)
    {
      if (i_dreb_SW == 0) // защита от ддребезга контактов для кнопки
        {
          i_dreb_SW = 1;
          set_encoder_diff(0);  
          i_type_file++;
          if(i_type_file>3)i_type_file=0;
          if (VS1053_status_get_status () == VS1053_PLAY)
            {
              VS1053_stop_PlayMP3 ();
            };
        }
    }


  if(get_encoder_btn_state()==0) //Нажали
    {
      if (i_dreb_SW == 0) // защита от ддребезга контактов для кнопки
        {
          i_dreb_SW = 1;
          if (VS1053_status_get_status () == VS1053_PLAY)
            {
              VS1053_stop_PlayMP3 ();
            };
        }
    }

  if (i_dreb_SW != 0
      && i_dreb_SW++ > i_pos_dreb_SW) //можно отсчитывать временной интервал
    {
      i_dreb_SW = 0; // от дребезга
    }

}

void
user_app1_task (void *sdata)
{
  printf ("user_app1_task start\n");

  u8 timer_id;
  struct tls_timer_cfg timer_cfg;
  timer_cfg.unit = TLS_TIMER_UNIT_US;
  timer_cfg.timeout = 300;
  timer_cfg.is_repeat = 1;
  timer_cfg.callback = (tls_timer_irq_callback)demo_timer_irq;
  timer_cfg.arg = NULL;
  timer_id = tls_timer_create (&timer_cfg);
  tls_timer_start (timer_id);
  printf ("timer start\n");

  libENCODER_t enc_pin = {
	 .ENCODER_S=WM_IO_PA_11,
	 .ENCODER_A=WM_IO_PA_12, 
	 .ENCODER_B=WM_IO_PA_13, 
    .ADD_BUTTON_1 = NO_GPIO_PIN,
    .ADD_BUTTON_2 = NO_GPIO_PIN,
    .ADD_BUTTON_3 = NO_GPIO_PIN,
   };
  bsp_encoder_init(&enc_pin);



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
    .spi_fastest_speed = 0,
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
