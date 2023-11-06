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
//#include "wm_netif.h"
//#include "wm_efuse.h"
//#include "wm_mem.h"
//#include "wm_regs.h"

#include "mod1/URTouch.h"
#include "mod1/UTFT.h"
#include "mod1/psram.h"
#include "mod1/u_jpeg.h"

#include "ff.h"
#include "wm_gpio_afsel.h"
#include "utils.h"

#include "lfile_http.h"

#include "../../../../../../w_wifi_pass.h"
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

extern uint8_t SmallFont[]; // подключаем маленький шрифт
extern uint8_t BigFont[];   // подключаем большой шрифт
extern uint8_t
    SevenSegNumFont[]; // подключаем шрифт имитирующий семисегментный индикатор
extern uint8_t SmallSymbolFont[];
extern uint8_t Dingbats1_XL[];


void
user_app1_task (void *sdata)
{
  printf ("user_app1_task start 3.5 TFT 480x320 HW SDIO SPI MSP3526 \n");

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
                40000000
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


  UTFT_InitLCD (TOUCH_ORIENTATION); // инициируем дисплей

  // URTouch_URTouch(sensor_type, byte tclk_scl, byte tcs_sda, byte
  // tdin_thresh, byte dout_none, byte irq_irq);
  /*
    URTouch_URTouch (TS_FT6236 // sensor_type
                     ,
                     WM_IO_PA_01 // byte tclk_scl
                     ,
                     WM_IO_PA_04 // byte tcs_sda
                     ,
                     50//FT6236_DEFAULT_THRESHOLD // byte din_thresh
                     ,
                     NO_GPIO_PIN // dout_none
                     ,
                     NO_GPIO_PIN //WM_IO_PA_09 // byte irq_irq
    );

    URTouch_InitTouch (TOUCH_ORIENTATION);
    URTouch_setPrecision (PREC_MEDIUM);
  */
  UTFT_clrScr ();
  UTFT_setFont (SmallFont);

  FATFS fs;
  FIL fnew;
  FRESULT res_sd;
  //UINT fnum;
  //char WriteBuffer[100];
  //BYTE ReadBuffer[256] = { 0 };

  wm_psram_config (1);
  d_psram_init (PSRAM_SPI, 2, 2, 1, 2);
  tls_os_time_delay (HZ / 10);
  // mount psram disk !
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

              char s_Url[256];
              sprintf (s_Url, "http://192.168.1.1:8088/jpg/j%.3d-480x320.jpg", ind_file);

              printf ("s_Url = %s\n", s_Url);
              //sprintf (WriteBuffer, "white txt %s file name", FileName);

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
                      //printf ("fnum = %d\r\n", fnum);
                      //printf ("WriteBuffer = %s \r\n", WriteBuffer);
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

              if (UTFT_ADD_lcd_draw_jpeg (FileName, 0, 0) < 0)
                    {
                      ind_file = -1;
                    }
              ind_file++;

              /*
              res_sd = f_open (&fnew, FileName, FA_OPEN_EXISTING | FA_READ);
              if (res_sd == FR_OK)
                {
                  printf ("f_open ok\r\n");
                  res_sd
                      = f_read (&fnew, ReadBuffer, sizeof (ReadBuffer), &fnum);

                  if (res_sd == FR_OK)
                    {
                      printf ("fnum = %d\r\n", fnum);
                      //printf ("ReadBuffer = %s\r\n", ReadBuffer);
                      dumpBuffer("jpg", (char*)ReadBuffer, fnum);
                    }
                  else
                    {
                      printf ("f_read failed! error code:%d\r\n", res_sd);
                      tls_os_time_delay (HZ);
                    }

                  f_close (&fnew);
                }
              else
                {
                  printf ("f_open failed! error code:%d\r\n", res_sd);
                  tls_os_time_delay (HZ);
                }
              */

              tls_os_time_delay (HZ * 10);
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
  // tls_sys_clk_set (CPU_CLK_2M);
  // tls_sys_clk_set (CPU_CLK_40M);
  // tls_sys_clk_set (CPU_CLK_80M);
  // tls_sys_clk_set (CPU_CLK_160M);

  tls_os_task_create (NULL, NULL, user_app1_task, NULL,
                      (void *)UserApp1TaskStk, /* task's stack start address */
                      USER_APP1_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      USER_APP1_TASK_PRIO, 0);
}
