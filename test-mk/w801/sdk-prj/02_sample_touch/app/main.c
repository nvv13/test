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
#include "mod1/UTFT_Buttons.h"

//#define TOUCH_ORIENTATION PORTRAIT
#define TOUCH_ORIENTATION  LANDSCAPE


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
  printf ("user_app1_task start 3.5 TFT 320x480 HW SPI \n");

  // подключаем библиотеку UTFT
  UTFT_UTFT (TFT3_5SP_9488
             ,(u8)NO_GPIO_PIN // WM_IO_PB_17  //RS  SDA
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

//URTouch_URTouch(sensor_type, byte tclk_scl, byte tcs_sda, byte tdin_thresh, byte dout_none, byte irq_irq);
  URTouch_URTouch (TS_FT6236  // sensor_type
                   ,
                   WM_IO_PA_01 // byte tclk_scl
                   ,
                   WM_IO_PA_04 // byte tcs_sda
                   ,
                   FT6236_DEFAULT_THRESHOLD // byte din_thresh
                   ,
                   NO_GPIO_PIN // dout_none
                   ,
                   WM_IO_PA_09 // byte irq_irq
  );


  URTouch_InitTouch (TOUCH_ORIENTATION);
  URTouch_setPrecision (PREC_MEDIUM);

  UTFT_Buttons_UTFT_Buttons();
  UTFT_Buttons_setTextFont(BigFont);
  UTFT_Buttons_setSymbolFont(Dingbats1_XL);

  //

  UTFT_clrScr ();
  UTFT_setFont (BigFont);
  UTFT_setColor2 (VGA_FUCHSIA); // устанавливаем пурпурный цвет текста

  int but1, but2, pressed_button;
  
  but1 = UTFT_Buttons_addButton( 10 ,  240, 100,  50, "RED",0);
  but2 = UTFT_Buttons_addButton( 140,  240, 110,  50, "GREEN",0);
  UTFT_Buttons_disableButton(but2, true);

  UTFT_Buttons_drawButtons();

  while (1)
    { //


    if (URTouch_dataAvailable() == true)
    {
      pressed_button = UTFT_Buttons_checkButtons();

      if (pressed_button==but1)
        {
          UTFT_Buttons_disableButton(but1, true);
          UTFT_Buttons_enableButton(but2, true);
          //UTFT_fillScr2 (VGA_RED);
          //UTFT_Buttons_drawButtons();
          UTFT_setColor2 (VGA_RED); // Устанавливаем лаймовый цвет
          UTFT_fillRect (
             10, 10, 100,
             200); // Рисуем закрашенный прямоугольник (с противоположными углами

        }
      if (pressed_button==but2)
        {
          UTFT_Buttons_disableButton(but2, true);
          UTFT_Buttons_enableButton(but1, true);
          //UTFT_fillScr2 (VGA_GREEN);
          //UTFT_Buttons_drawButtons();
          UTFT_setColor2 (VGA_GREEN); // Устанавливаем лаймовый цвет
          UTFT_fillRect (
             10, 10, 100,
             200); // Рисуем закрашенный прямоугольник (с противоположными углами
        }

    }

    } //
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
