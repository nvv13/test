/*****************************************************************************
 *
 * File Name : main.c
 *
 * Description: main
 *
 * Date : 2023-08-12
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
#include "wm_rtc.h"

#include "mod1/UTFT.h"

#include "FT6236.h"

#define USER_APP1_TASK_SIZE 2048
static OS_STK UserApp1TaskStk[USER_APP1_TASK_SIZE];
#define USER_APP1_TASK_PRIO 32

extern uint8_t SmallFont[]; // подключаем маленький шрифт
extern uint8_t BigFont[];   // подключаем большой шрифт
extern uint8_t
    SevenSegNumFont[]; // подключаем шрифт имитирующий семисегментный индикатор
extern uint8_t SmallSymbolFont[];

#include "wm_gpio_afsel.h"

void
user_app1_task (void *sdata)
{
  printf ("user_app1_task start 3.5 TFT 320x480 HW SPI \n");

  // подключаем библиотеку UTFT
  UTFT_UTFT (TFT3_5SP_9488 //
             ,
             (u8)NO_GPIO_PIN // WM_IO_PB_17  //RS  SDA
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

  UTFT_InitLCD (LANDSCAPE); // инициируем дисплей
  // UTFT_InitLCD (PORTRAIT);

  FT6236_begin (FT6236_DEFAULT_THRESHOLD, WM_IO_PA_04, WM_IO_PA_01);

  UTFT_clrScr (); // стираем всю информацию с дисплея
  tls_os_time_delay (HZ); //

  UTFT_setColor2 (VGA_WHITE); //
  for (int i = 2; i < 80; i++)
    {
      UTFT_drawRect (2, 2, i * 6, i * 4);
    }

  tls_os_time_delay (HZ / 100); //

  UTFT_clrScr (); // стираем всю информацию с дисплея
  UTFT_setFont (BigFont); // устанавливаем большой шрифт
  char char_buff[100];

  while (1)
    { //

      if (FT6236_touched ())
        {
          // Retrieve a point
          TS_Point p = FT6236_getPoint (0);
          p.y=UTFT_getDisplayXSize () - p.y;
          // Print coordinates to the serial output
          sprintf (char_buff, "     X=%d, Y=%d     ", p.x, p.y);
          //printf ("%s\n", char_buff);
          UTFT_setColor2 (VGA_WHITE);
          UTFT_print (char_buff, CENTER, 250, 0); // выводим текст на дисплей
          if (p.x >= 0 && p.y >= 0)
            {
              UTFT_setColor2 (VGA_BLUE); 
              UTFT_fillCircle ( p.y, p.x, 2); // Рисуем закрашенную окружность
            }
        }

      // Debouncing. To avoid returning the same touch multiple times you can
      // play with this delay.
      tls_os_time_delay (HZ / 200); // ~50 ms

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
