/*****************************************************************************
 *
 * File Name : main.c
 *
 * Description: main
 *
 * Date : 2023-04-11
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

#include "URTouch.h"
#include "mod1/UTFT.h"
#include "UTFT_Buttons.h"

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

  UTFT_clrScr (); // стираем всю информацию с дисплея

  UTFT_setColor2 (VGA_WHITE); // 800x480
  for (int i = 2; i < 80; i++)
    {
      UTFT_drawRect (2, 2, i * 6, i * 4);
    }
  tls_os_time_delay (HZ); //

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

  int but1, but2, but3, but4, butX, butY, pressed_button;
  boolean default_colors = true;
  
  but1 = UTFT_Buttons_addButton( 10,  20, 300,  30, "Button 1",0);
  but2 = UTFT_Buttons_addButton( 10,  60, 300,  30, "Button 2",0);
  but3 = UTFT_Buttons_addButton( 10, 100, 300,  30, "Button 3",0);
  but4 = UTFT_Buttons_addButton( 10, 140, 300,  30, "Button 4", BUTTON_DISABLED);
  butX = UTFT_Buttons_addButton(279, 199,  40,  40, "a", BUTTON_SYMBOL);
  butY = UTFT_Buttons_addButton(  0, 199, 100,  40, "I", BUTTON_SYMBOL | BUTTON_SYMBOL_REP_3X);
  UTFT_Buttons_drawButtons();

  UTFT_print("You pressed:", 110, 205,0);
  UTFT_setColor2(VGA_BLACK);
  UTFT_setBackColor2(VGA_WHITE);
  UTFT_print("None    ", 110, 220,0);

//  int x = 0, y = 0;

  while (1)
    { //


    if (URTouch_dataAvailable() == true)
    {
      pressed_button = UTFT_Buttons_checkButtons();

      if (pressed_button==butX)
      {
        if (UTFT_Buttons_buttonEnabled(but4))
          UTFT_Buttons_disableButton(but4, true);
        else
          UTFT_Buttons_enableButton(but4, true);
      }
      else if (pressed_button==butY)
      {
        if (default_colors)
        {
          UTFT_Buttons_setButtonColors(VGA_YELLOW, VGA_RED, VGA_YELLOW, VGA_BLUE, VGA_GRAY);
          UTFT_Buttons_relabelButton(butY, "_",false);
          UTFT_Buttons_drawButtons();
          default_colors=false;
        }
        else
        {
          UTFT_Buttons_setButtonColors(VGA_WHITE, VGA_GRAY, VGA_WHITE, VGA_RED, VGA_BLUE);
          UTFT_Buttons_relabelButton(butY, "I",false);
          UTFT_Buttons_drawButtons();
          default_colors=true;
        }
      }
      if (pressed_button==but1)
        UTFT_print("Button 1", 110, 220, 0);
      if (pressed_button==but2)
        UTFT_print("Button 2", 110, 220, 0);
      if (pressed_button==but3)
        UTFT_print("Button 3", 110, 220, 0);
      if (pressed_button==but4)
        UTFT_print("Button 4", 110, 220, 0);
      if (pressed_button==-1)
        UTFT_print("None    ", 110, 220, 0);
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