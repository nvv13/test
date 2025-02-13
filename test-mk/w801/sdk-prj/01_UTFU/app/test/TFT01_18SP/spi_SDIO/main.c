

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
#include "wm_rtc.h"

#include "UTFT.h"
#include "u_jpeg.h"

#define USER_APP1_TASK_SIZE 2048
static OS_STK UserApp1TaskStk[USER_APP1_TASK_SIZE];
#define USER_APP1_TASK_PRIO 32

extern uint8_t SmallFont[]; // подключаем маленький шрифт
extern uint8_t BigFont[];   // подключаем большой шрифт
extern uint8_t
    SevenSegNumFont[]; // подключаем шрифт имитирующий семисегментный индикатор
extern uint8_t SmallSymbolFont[];


void
user_app1_task (void *sdata)
{
  printf ("user_app1_task start TFT01_18SP 128x160 HW SPI st7735S\n");

  // подключаем библиотеку UTFT
  UTFT_UTFT (TFT01_18SP
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
3.3v bl    подсветка 
3.3v vcc
gnd  gnd
PB21 res   RESET сброс           
PB23 cs    CS выбор чипа       
PB22 dc    комманда/данные  
PB07 sda   данные           
PB06 sck   синхросигнал     
---- ------
W801 LCD   
---- ------


*/


  UTFT_InitLCD (LANDSCAPE); // инициируем дисплей
  // UTFT_InitLCD (PORTRAIT);

  while (1)
    {                 //
      UTFT_clrScr (); // стираем всю информацию с дисплея

      UTFT_setColor2 (VGA_WHITE); // 240x280
      for (int i = 2; i < 32; i++)
        {
          UTFT_drawRect (2, 2, i * 5, i * 4);
        }

      tls_os_time_delay (HZ * 3); //

      UTFT_fillScr2 (VGA_BLACK);

      UTFT_setColor2 (VGA_BLUE); // Устанавливаем синий цвет
      UTFT_drawRoundRect (
          10, 10, 150,
          118); // Рисуем прямоугольник со скруглёнными углами (с
                // противоположными углами в координатах 10x110 - 170x210)
      tls_os_time_delay (HZ * 3); //
                                  //
      UTFT_setColor2 (VGA_LIME); // Устанавливаем лаймовый цвет
      UTFT_fillRect (11          //по горизонтали?
                     ,
                     11 // по вертикали?
                     ,
                     149 //длинна?
                     ,
                     117 //высота?
      ); // Рисуем закрашенный прямоугольник (с противоположными углами
         // в координатах 10x220 - 170x310)
      tls_os_time_delay (HZ * 3); //
                                  //
      UTFT_setColor2 (VGA_PURPLE); // Устанавливаем фиолетовый цвет
      UTFT_drawCircle (
          80, 64,
          50); // Рисуем окружность (с центром в точке x y  и радиусом r)

      UTFT_fillCircle (80, 64, 40); // Рисуем закрашенную окружность (с
                                     // центром в точке x y и радиусом r)
      tls_os_time_delay (HZ * 3);

      UTFT_fillScr2 (VGA_RED);
      tls_os_time_delay (HZ); // заливаем
      //   дисплей красным,     ждём 1 секунду

      UTFT_fillScr2 (VGA_GREEN);
      tls_os_time_delay (HZ); // заливаем
      //   дисплей зелёным,     ждём 1 секунду

      UTFT_fillScr2 (VGA_BLUE);
      tls_os_time_delay (HZ); // заливаем дисплей синим,       ждём 1   секунду

      UTFT_fillScr2 (VGA_SILVER);
      tls_os_time_delay (HZ); //   заливаем дисплей серебристым, ждём 1 секунду

      UTFT_fillScr2 (VGA_MAROON);
      tls_os_time_delay (HZ); // заливаем  дисплей бордовым,    ждём 1 секунду

      UTFT_fillScr2 (VGA_NAVY);
      tls_os_time_delay (HZ); // заливаем дисплей тем. синим,  ждём 1  секунду

      UTFT_clrScr (); // стираем всю информацию с дисплея
      UTFT_setFont (BigFont); // устанавливаем большой шрифт
      UTFT_setColor2 (VGA_BLUE); // устанавливаем синий цвет текста
      UTFT_print ("BigFont", CENTER, 64,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 100 точек)
      UTFT_print ("12345678", CENTER, 64,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 115 точек)
      tls_os_time_delay (HZ * 3);
      //

      UTFT_setFont (SmallFont); // устанавливаем большой шрифт
      UTFT_print ("SmallFont", CENTER, 80,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 100 точек)
      UTFT_print ("12345678", CENTER, 64,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 115 точек)
      tls_os_time_delay (HZ * 3);
      //
      UTFT_setFont (SevenSegNumFont); // устанавливаем шрифт имитирующий
                                      // семисегментный индикатор
      UTFT_setColor2 (VGA_FUCHSIA); // устанавливаем пурпурный цвет текста
      UTFT_print ("1234567890", CENTER, 64,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 150 точек)
      tls_os_time_delay (HZ * 3);


      UTFT_setFont (SmallSymbolFont); // устанавливаем шрифт имитирующий
      UTFT_print ("\x20\x21\x22\x23\x24\x25", CENTER, 10,
                  0); // выводим текст на дисплей (выравнивание по ширине -
      tls_os_time_delay (HZ * 3);



      unsigned int t=0; // used to save time relative to 1970
      struct tm *tblock;
      tblock = localtime ((const time_t *)&t); // switch to local time
      tls_set_rtc (tblock);
      struct tm tstart;
      struct tm tstop;
      tls_get_rtc (&tstart);
      u32 current_tick = tls_os_get_time();
      u32 count=0;
      while( (tls_os_get_time() - current_tick) <= (HZ*10) )
       {
       UTFT_fillScr2 (count);
       count++;
       }
      tls_get_rtc (&tstop);
      int sec=(tstop.tm_hour*3600 + tstop.tm_min*60 + tstop.tm_sec) - (tstart.tm_hour*3600 + tstart.tm_min*60 + tstart.tm_sec);
      UTFT_clrScr ();
      UTFT_setFont (SmallFont);
      UTFT_setColor2 (VGA_FUCHSIA); // устанавливаем пурпурный цвет текста
      char mesg[50];
      sprintf (mesg, "run %d sec" , sec);
      UTFT_print (mesg, CENTER, 20, 0);       
      sprintf (mesg, "=%d FPS=%d", count, count/sec);
      UTFT_print (mesg, CENTER, 50, 0);       
      tls_os_time_delay (HZ * 10);





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
