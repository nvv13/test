/*****************************************************************************
 *
 * File Name : main.c
 *
 * Description: main
 *
 * Date : 2023-08-15
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wm_type_def.h"

#include "wm_cpu.h"

//#include "wm_watchdog.h"
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
#include "mod1/u_jpeg.h"

#include "ff.h"
#include "wm_gpio_afsel.h"

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
  printf ("user_app1_task start 3.2 TFT 240x320 HW SPI MSP2834 \n");

  // подключаем библиотеку UTFT
  UTFT_UTFT (MSP2834, (u8)NO_GPIO_PIN // WM_IO_PB_17  //RS  SDA
             ,
             (u8)NO_GPIO_PIN // WM_IO_PB_15  //WR  SCL
             ,
             (u8)NO_GPIO_PIN // WM_IO_PB_14  //CS  CS
             ,
             (u8)WM_IO_PB_21 // RST reset RES
             ,
             (u8)WM_IO_PB_23 // SER => DC !
             ,
             20000000
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
           */
  );

  UTFT_InitLCD (TOUCH_ORIENTATION); // инициируем дисплей

  // URTouch_URTouch(sensor_type, byte tclk_scl, byte tcs_sda, byte
  // tdin_thresh, byte dout_none, byte irq_irq);
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

  UTFT_clrScr ();
  UTFT_setFont (SmallFont);

  char char_buff[100];

  FATFS fs;
  FRESULT res_sd;
  wm_sdio_host_config (0);
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
  int ind_file = -1;

  UTFT_setColor2 (VGA_FUCHSIA); // устанавливаем пурпурный цвет текста
  //             x      y    x    y
  UTFT_drawRect (1, 1, 40, 239);
  UTFT_drawRect (280, 1, 319, 239);
  UTFT_setColor2 (VGA_WHITE);
  UTFT_drawRect (1, 210, 319, 239);

  while (1)
    { //

      if (URTouch_dataAvailable () == true)
        {

          URTouch_read ();

          if (URTouch_TP_X > 0 && URTouch_TP_Y > 0)
            {
              UTFT_setColor2 (VGA_BLUE);
              UTFT_fillCircle (URTouch_TP_X, URTouch_TP_Y,
                               2); // Рисуем закрашенную окружность
            }
          if (URTouch_TP_X2 > 0 && URTouch_TP_Y2 > 0)
            {
              UTFT_setColor2 (VGA_RED);
              UTFT_fillCircle (URTouch_TP_X2, URTouch_TP_Y2, 2);
            }

          if (URTouch_TP_X > 0 || URTouch_TP_Y > 0 || URTouch_TP_X2 > 0
              || URTouch_TP_Y2 > 0)
            {

              sprintf (char_buff, "  x=%d,y=%d,x2=%d,y2=%d  ", URTouch_TP_X,
                       URTouch_TP_Y, URTouch_TP_X2, URTouch_TP_Y2);
              // printf ("%s\n", char_buff);
              UTFT_setColor2 (VGA_WHITE);
              UTFT_print (char_buff, CENTER, 215,
                          0); // выводим текст на дисплей
            }
          if (URTouch_TP_Y > 210)
            {
              UTFT_clrScr ();
              UTFT_setColor2 (
                  VGA_FUCHSIA); // устанавливаем пурпурный цвет текста
             UTFT_drawRect (1, 1, 40, 239);
             UTFT_drawRect (280, 1, 319, 239);
             UTFT_setColor2 (VGA_WHITE);
             UTFT_drawRect (1, 210, 319, 239);
            }

          if (res_sd == FR_OK)
            {
              bool l_draw = false;
              if (URTouch_TP_X > 280)
                {
                  ind_file++;
                  l_draw = true;
                }
              if (URTouch_TP_X > 0 && URTouch_TP_X < 40 && ind_file > 0)
                {
                  ind_file--;
                  l_draw = true;
                }
              if (l_draw)
                {
                  char FileName[256];
                  sprintf (FileName, "0:320x240/j%.3d-320x240.jpg", ind_file);
                  printf ("%s\n", FileName);
                  if (UTFT_ADD_lcd_draw_jpeg (FileName, 0, 0) < 0)
                    {
                      ind_file = -1;
                    }
                }
            }
        }

    } //

  // unmount file system
  f_mount (NULL, "0:", 1);
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
