

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

//#include "wm_gpio_afsel.h"

void
user_app1_task (void *sdata)
{
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
5v   2
3.3v 1
gnd  6,25
PB21 22    RESET сброс           
PB23 24    CS выбор чипа       
PB22 18    комманда/данные  
PB07 19    данные           
PB06 23    синхросигнал     
---- ------
W801 LCD   
---- ------


*/

  UTFT_InitLCD (LANDSCAPE); // инициируем дисплей
  // UTFT_InitLCD (PORTRAIT);

  while (1)
    {                 //
      UTFT_clrScr (); // стираем всю информацию с дисплея
      tls_os_time_delay (HZ); //

      extern unsigned short j001_240x160[0x9600];
      extern unsigned short j002_240x160[0x9600];
      extern unsigned short j011_240x160[0x9600];
      extern unsigned short j013_240x160[0x9600];
      u8 p=0;
      for (int i = 0; i < 100; i++)
        {
        switch(p)
         {
         case 3: { 
         UTFT_drawBitmap (0, 0, 240, 160,     j001_240x160, 1);
         UTFT_drawBitmap (0, 160, 240, 160,   j002_240x160, 1);
         UTFT_drawBitmap (240, 160, 240, 160, j011_240x160, 1);
         UTFT_drawBitmap (240, 0, 240, 160,   j013_240x160, 1);
                  };break;
         case 2: { 
         UTFT_drawBitmap (0, 0, 240, 160,     j013_240x160, 1);
         UTFT_drawBitmap (0, 160, 240, 160,   j001_240x160, 1);
         UTFT_drawBitmap (240, 160, 240, 160, j002_240x160, 1);
         UTFT_drawBitmap (240, 0, 240, 160,   j011_240x160, 1);
                  };break;
         case 1: { 
         UTFT_drawBitmap (0, 0, 240, 160,     j011_240x160, 1);
         UTFT_drawBitmap (0, 160, 240, 160,   j013_240x160, 1);
         UTFT_drawBitmap (240, 160, 240, 160, j001_240x160, 1);
         UTFT_drawBitmap (240, 0, 240, 160,   j002_240x160, 1);
                  };break;
         case 0: { 
         UTFT_drawBitmap (0, 0, 240, 160,     j002_240x160, 1);
         UTFT_drawBitmap (0, 160, 240, 160,   j011_240x160, 1);
         UTFT_drawBitmap (240, 160, 240, 160, j013_240x160, 1);
         UTFT_drawBitmap (240, 0, 240, 160,   j001_240x160, 1);
                  };break;
         }
         p++;
         p&=0x3;
         if(i<5)tls_os_time_delay (400);
         if(i<20)tls_os_time_delay (100);
        }
                                     


      UTFT_fillScr2 (VGA_BLACK);

      UTFT_setColor2 (VGA_GREEN); // Устанавливаем зелёный цвет
      UTFT_drawRect (
          5, 5, 480 - 5,
          320 - 5); // Рисуем прямоугольник (с противоположными углами)

      UTFT_setColor2 (VGA_RED); // Устанавливаем красный цвет
      UTFT_drawLine (1, 1, 150,
                     70);     // Рисуем линию

      UTFT_setColor2 (VGA_BLUE); // Устанавливаем синий цвет
      UTFT_drawRoundRect (
          10, 10, 470,
          310); // Рисуем прямоугольник со скруглёнными углами (с
                // противоположными углами в координатах 10x110 - 170x210)
                                  //
      UTFT_setColor2 (VGA_LIME); // Устанавливаем лаймовый цвет
      UTFT_fillRect (11          //по горизонтали?
                     ,
                     11 // по вертикали?
                     ,
                     471 //длинна?
                     ,
                     311 //высота?
      ); // Рисуем закрашенный прямоугольник (с противоположными углами
         // в координатах 10x220 - 170x310)
                                  //
      UTFT_setColor2 (VGA_PURPLE); // Устанавливаем фиолетовый цвет
      UTFT_drawCircle (
          240, 160,
          70); // Рисуем окружность (с центром в точке x y  и радиусом r)

      UTFT_fillCircle (240, 160, 50); // Рисуем закрашенную окружность (с
                                    // центром в точке x y и радиусом r)
      UTFT_fillScr2 (VGA_RED);
      UTFT_fillScr2 (VGA_GREEN);
      UTFT_fillScr2 (VGA_BLUE);
      UTFT_fillScr2 (VGA_SILVER);
      UTFT_fillScr2 (VGA_MAROON);
      UTFT_fillScr2 (VGA_NAVY);
      UTFT_clrScr (); // стираем всю информацию с дисплея
      UTFT_setFont (BigFont); // устанавливаем большой шрифт
      UTFT_setColor2 (VGA_BLUE); // устанавливаем синий цвет текста
      UTFT_print ("BigFont", CENTER, 40,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 100 точек)
      UTFT_clrScr (); // стираем всю информацию с дисплея
      UTFT_setColor2 (VGA_RED); // устанавливаем
      UTFT_setBackColor2 (VGA_TRANSPARENT);
      UTFT_print ("12:35", CENTER, 40,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 115 точек)
      //

      UTFT_clrScr (); // стираем всю информацию с дисплея
      UTFT_setFont (SmallFont); // устанавливаем большой шрифт
      UTFT_print ("SmallFontTest", CENTER, 10,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 100 точек)
      UTFT_print ("12345678", CENTER, 50,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 115 точек)
      //
      UTFT_setFont (SevenSegNumFont); // устанавливаем шрифт имитирующий
                                      // семисегментный индикатор
      UTFT_clrScr (); // стираем всю информацию с дисплея
      UTFT_setColor2 (VGA_FUCHSIA); // устанавливаем пурпурный цвет текста
      UTFT_print ("12345", CENTER, 10,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 150 точек)

      UTFT_setFont (SmallFont); // устанавливаем большой шрифт
      UTFT_print ("SmallFont", CENTER, 80,
                  0); // выводим текст на дисплей (выравнивание по ширине -

      UTFT_setFont (SmallSymbolFont); // устанавливаем шрифт имитирующий
      UTFT_print ("\x20\x21\x22\x23\x24\x25", CENTER, 100,
                  0); // выводим текст на дисплей (выравнивание по ширине -

      unsigned int t = 0; // used to save time relative to 1970
      struct tm *tblock;
      tblock = localtime ((const time_t *)&t); // switch to local time
      tls_set_rtc (tblock);
      struct tm tstart;
      struct tm tstop;
      tls_get_rtc (&tstart);
      u32 current_tick = tls_os_get_time ();
      u32 count = 0;
      while ((tls_os_get_time () - current_tick) <= (HZ * 10))
        {
          UTFT_fillScr2 (count);
          count++;
        }
      tls_get_rtc (&tstop);
      int sec = (tstop.tm_hour * 3600 + tstop.tm_min * 60 + tstop.tm_sec)
                - (tstart.tm_hour * 3600 + tstart.tm_min * 60 + tstart.tm_sec);
      UTFT_clrScr ();
      UTFT_setFont (BigFont);
      UTFT_setColor2 (VGA_FUCHSIA); // устанавливаем пурпурный цвет текста
      char mesg[50];
      sprintf (mesg, "run %d sec", sec);
      UTFT_print (mesg, CENTER, 20, 0);
      sprintf (mesg, "=%d FPS=%d", count, count / sec);
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
