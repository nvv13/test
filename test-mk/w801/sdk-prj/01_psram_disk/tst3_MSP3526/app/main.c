/*****************************************************************************
 *
 * File Name : main.c
 *
 * Description: main
 *
 * Date : 2023-11-02
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wm_type_def.h"

#include "wm_cpu.h"

#include "wm_watchdog.h"
//#include "wm_uart.h"
#include "wm_gpio.h"
//#include "wm_hostspi.h"
//#include "wm_socket.h"
//#include "wm_sockets.h"
//#include "wm_wifi.h"
//#include "wm_hspi.h"
//#include "wm_pwm.h"
//#include "wm_params.h"
#include "wm_osal.h"
#include "wm_timer.h"
//#include "wm_netif.h"
//#include "wm_efuse.h"
//#include "wm_mem.h"
//#include "wm_regs.h"

#include "mod1/UTFT.h"
#include "mod1/psram.h"
#include "mod1/u_jpeg.h"

#include "ff.h"
#include "utils.h"
#include "wm_gpio_afsel.h"

#include "mod1/lfile_http.h"

#include "../../../../../../../w_wifi_pass.h"
//#define MY_WIFI_AP "bred8"
//#define MY_WIFI_PASS "123123123"
//#define MY_WIFI_AP "bred1"
//#define MY_WIFI_PASS "9115676369"

#include "w_wifi.h"
u8 volatile u8_wifi_state = 0;

//#define TOUCH_ORIENTATION PORTRAIT
#define TOUCH_ORIENTATION LANDSCAPE

#define USER_APP1_TASK_SIZE 2048
static OS_STK UserApp1TaskStk[USER_APP1_TASK_SIZE];
#define USER_APP1_TASK_PRIO 32

#define URL_ARR_SIZE 14
static const char *aUrl[URL_ARR_SIZE] = {
  "http://188.9.157.80:81/axis-cgi/jpg/image.cgi?resolution=640x480",
  "https://stihi.ru/pics/2020/05/13/6749.jpg",
  "https://add-groups.com/uploads/mini/short/67/1596098197_1528559713160437186.jpg",
  "https://stihi.ru/pics/2016/04/03/2978.jpg",
    "https://ciprogress.files.wordpress.com/2013/09/cielo-de-colores-imposibles-4.jpg?w=480",
  "http://mobiliv.ru/_ph/138/2/81395433.jpg",
  "http://mobiliv.ru/_ph/138/2/184736894.jpg",
    "http://mobiliv.ru/_ph/138/2/672145609.jpg",
    "http://mobiliv.ru/_ph/138/2/749000498.jpg",
    "http://mobiliv.ru/_ph/138/2/334097223.jpg",
    "http://mobiliv.ru/_ph/138/2/721252628.jpg",
    "http://mobiliv.ru/_ph/138/2/753682620.jpg",
    "http://mobiliv.ru/_ph/138/2/411781451.jpg" //,
    "http://mobiliv.ru/_ph/138/2/763402510.jpg"
};

extern uint8_t SmallFont[]; // подключаем маленький шрифт
extern uint8_t BigFont[];   // подключаем большой шрифт
extern uint8_t
    SevenSegNumFont[]; // подключаем шрифт имитирующий семисегментный индикатор
extern uint8_t SmallSymbolFont[];
extern uint8_t Dingbats1_XL[];

static void
my_timer_irq (u8 *arg) // здесь будет смена режима
{
}

void
user_app1_task (void *sdata)
{

  // подключаем библиотеку UTFT

  printf ("user_app1_task start 3.5 TFT 320x480 HW SDIO SPI MSP3526 \n");

  // подключаем библиотеку UTFT
  UTFT_UTFT (MSP3526 //
             ,
             (u8)NO_GPIO_PIN // WM_IO_PB_17  //RS  SDA
             ,
             (u8)NO_GPIO_PIN // WM_IO_PB_15  //WR  SCL
             ,
             WM_IO_PB_23 //(u8)NO_GPIO_PIN // WM_IO_PB_14  //CS  CS
             ,
             (u8)WM_IO_PB_21 // RST reset RES
             ,
             (u8)WM_IO_PB_22 // SER => DC !
             ,
             //120000000
                60000000
             /* spi_freq(Герц) для 5 контактных SPI дисплеев
                (где отдельно ножка комманда/данные)
             програмируеться HW SPI на ножки (предопред)
                 wm_spi_cs_config (WM_IO_PB_14);
                 wm_spi_ck_config (WM_IO_PB_15);
                 wm_spi_di_config (WM_IO_PB_16);
                 wm_spi_do_config (WM_IO_PB_17);
             но, можно отказаться от HW SPI в пользу Soft SPI
             установив spi_freq=0
             эмуляции SPI, это удобно для разных ножек

    максимально, частота spi_freq = 20000000 (20MHz)
        но!      если spi_freq > 20000000 тогда работает spi SDIO
        частоту можно ставить от 21000000 до 120000000 герц (работает при
    240Mhz тактовой) контакты: WM_IO_PB_06 CK   -> SCL 
                               WM_IO_PB_07 CMD  -> MOSI
           */
  );
/*
---- ------
W801 LCD   
---- ------
gnd  GND  
3.3v VCC
PB06 SCL  синхросигнал
PB07 SDA  данные
PB21 RES  сброс (reset)
PB22 DC   команда/данные
PB23 CS   выбор чипа
---- ------
W801 LCD   
---- ------
*/



  // UTFT_InitLCD (TOUCH_ORIENTATION); // инициируем дисплей
  UTFT_InitLCD (LANDSCAPE); // инициируем дисплей
  //UTFT_InitLCD (PORTRAIT);

  u8 timer_id;
  struct tls_timer_cfg timer_cfg;
  timer_cfg.unit = TLS_TIMER_UNIT_MS;
  timer_cfg.timeout = 100; //
  timer_cfg.is_repeat = 1;
  timer_cfg.callback = (tls_timer_irq_callback)my_timer_irq;
  timer_cfg.arg = NULL;
  timer_id = tls_timer_create (&timer_cfg);
  if (false)
    {
      tls_timer_start (timer_id);
      printf ("timer start\n");
    }

  UTFT_fillScr2 (VGA_NAVY);
  UTFT_clrScr ();
  UTFT_setFont (SmallFont);

  FATFS fs;
  FIL fnew;
  FRESULT res_sd;

  wm_psram_config (1);
  d_psram_init (PSRAM_SPI, 2, 2, 1, 2);
  tls_os_time_delay (HZ / 10);

  // mount psram disk ! не забудь заменить libapp на /mod1/libapp в makefile!
  res_sd = f_mount (&fs, "1:", 1);
  //***********************formatting test****************************
  if (res_sd == FR_NO_FILESYSTEM)
    {
      printf ("FR_NO_FILESYSTEM:Failed to mount file system! Probably "
              "because the file "
              "initialization failed! error code:%d\r\n",
              res_sd);

      BYTE work[FF_MAX_SS];
      res_sd = f_mkfs ("1:", 0, work, sizeof (work));

      if (res_sd == FR_OK)
        {
          printf ("mount again\r\n");
          res_sd = f_mount (NULL, "1:", 1);
          printf ("code:%d\r\n", res_sd);
          res_sd = f_mount (&fs, "1:", 1);
          printf ("code:%d\r\n", res_sd);
        }
      else
        {
          printf ("f_mkfs failed! error code:%d\r\n", res_sd);
        }
    }

  if (res_sd == FR_OK)
    {
      int ind_file = 0;
      tls_watchdog_init (60 * 1000
                         * 1000); // u32 usec microseconds, около 60 сек
      for (;;) // цикл(1) с подсоединением к wifi и запросом времени
        {
          while (u8_wifi_state == 0)
            {
              printf ("trying to connect wifi\n");
              if (u8_wifi_state == 0
                  && demo_connect_net (MY_WIFI_AP, MY_WIFI_PASS) == WM_SUCCESS)
                u8_wifi_state = 1;
              else
                {
                  tls_os_time_delay (HZ * 5);
                }
            }

          tls_os_time_delay (HZ * 5);
          tls_watchdog_clr ();

          while (u8_wifi_state == 1) // основной цикл(2)
            {

              char FileName[20];
              sprintf (FileName, "1:test0.jpg");

              if (ind_file > URL_ARR_SIZE - 1)
                ind_file = 0;
              char s_Url[256];
              sprintf (s_Url, "%s", aUrl[ind_file]);

              printf ("s_Url = %s\n", s_Url);
              // sprintf (WriteBuffer, "white txt %s file name", FileName);

              res_sd = f_open (&fnew, FileName, FA_CREATE_ALWAYS | FA_WRITE);
              if (res_sd == FR_OK)
                {
                  printf ("f_open ok, try load url = %s \r\n", s_Url);
                  // природа -inurl:https
                  res_sd = download_file_http (s_Url, &fnew);

                  //              res_sd
                  //                  = f_write (&fnew, WriteBuffer, sizeof
                  //                  (WriteBuffer), &fnum);
                  if (res_sd == FR_OK)
                    {
                      // printf ("fnum = %d\r\n", fnum);
                      // printf ("WriteBuffer = %s \r\n", WriteBuffer);
                    }
                  else
                    {
                      printf ("f_write failed! error code:%d\r\n", res_sd);
                      tls_os_time_delay (HZ);
                    }

                  f_close (&fnew);
                }
              else
                {
                  printf ("f_open failed! error code:%d\r\n", res_sd);
                  tls_os_time_delay (HZ);
                }

              UTFT_store_to_psram (true);
              if (UTFT_ADD_lcd_draw_jpeg (FileName, 0, 0) >= 0)
                {
                  UTFT_psram_to_drawBitmap ();
                  tls_os_time_delay (HZ * 6); //
                }

              ind_file++;

              // tls_os_time_delay (HZ * 10);
              //uint32_t cur = tls_os_get_time ();
              //while ((tls_os_get_time () - cur) < (HZ * 20))
              // { //
              // }
              tls_watchdog_clr ();
            }
        }

      // unmount file system
      f_mount (NULL, "1:", 1);
    }
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
