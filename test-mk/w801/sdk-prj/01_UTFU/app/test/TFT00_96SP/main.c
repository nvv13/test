!!!В РАБОТЕ!!!
!!!underway!!!



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


#include "UTFT.h"

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
  printf ("user_app1_task start\n");

  //Цветной графический дисплей 3.2 TFT 480x320 MEGA
  // http://iarduino.ru/shop/Displei/cvetnoy-graficheskiy-displey-3-2-tft-480x320.html
  // подключаем библиотеку UTFT

  // TFT00_96SP - для ST7735S
  UTFT_UTFT (TFT00_96SP, (u8)WM_IO_PA_01, (u8)WM_IO_PA_02, (u8)WM_IO_PA_03,
             (u8)WM_IO_PA_04, (u8)WM_IO_PA_05);

  //UTFT_UTFT(byte model, byte RS, byte WR, byte CS, byte RST, byte SER);
  //                               byte RS,         byte WR,         byte CS,
  //                               byte RST, byte SER
  // объявляем объект myGLCD класса библиотеки UTFT указывая тип дисплея
  // TFT32MEGA и номера выводов Arduino к которым подключён дисплей: RS, WR,
  // CS, RST. Выводы параллельной шины данных не указываются

  //UTFT_UTFT (TFT32MEGA_2, (u8)WM_IO_PA_01, (u8)WM_IO_PA_02, (u8)WM_IO_PA_03,
  //           (u8)WM_IO_PA_04, 0);
  //          TFT32MEGA_2                        // если изображение на дисплее
  //          отображается зеркально, значит для инициализации Вашего дисплея,
  //          нужно указать не TFT32MEGA, а TFT32MEGA_2.
  // (тип TFT32MEGA - для дисплеев на базе чипа ILI9481, а тип TFT32MEGA_2 -
  // для дисплеев на базе чипа HX8357С).
  //
  UTFT_InitLCD (LANDSCAPE); // инициируем дисплей
  //UTFT_InitLCD (PORTRAIT);
  // UTFT_clrScr();                            // стираем всю информацию с
  // дисплея
  //

  while (1)
    { //

      UTFT_clrScr (); // стираем всю информацию с дисплея
      tls_os_time_delay (HZ); // 
/*
      UTFT_setColor2 (VGA_GREEN); // Устанавливаем зелёный цвет
      UTFT_drawRect (1, 2, 17,
                     10); // Рисуем прямоугольник (с противоположными углами в
                           // координатах 10x20 - 170x100)
      tls_os_time_delay (HZ); // 

      UTFT_setColor2 (VGA_RED); // Устанавливаем красный цвет
      UTFT_drawLine (
          1, 1, 150,
          10); // Рисуем линию (через точки с координатами 10x10 - 170x10)
      tls_os_time_delay (HZ); // 

      UTFT_setColor2 (VGA_BLUE); // Устанавливаем синий цвет
      UTFT_drawRoundRect (
          1, 5, 30,
          15); // Рисуем прямоугольник со скруглёнными углами (с
                // противоположными углами в координатах 10x110 - 170x210)
      tls_os_time_delay (HZ); // 
*/
                //
      UTFT_setColor2 (VGA_LIME); // Устанавливаем лаймовый цвет
      UTFT_fillRect (
           5     //по горизонтали?
         , 5     // по вертикали?
         , 160-5 //длинна?
         , 80-5  //высота?
        ); // Рисуем закрашенный прямоугольник (с противоположными углами
                // в координатах 10x220 - 170x310)
      tls_os_time_delay (HZ*3); // 
                //
/*
      UTFT_setColor2 (VGA_PURPLE); // Устанавливаем фиолетовый цвет
      UTFT_drawCircle (
          80, 40,
          30); // Рисуем окружность (с центром в точке 350x90 и радиусом 70)

      UTFT_fillCircle (80, 40, 30); // Рисуем закрашенную окружность (с
                                      // центром в точке 350x240 и радиусом 70)

      tls_os_time_delay (HZ * 3);
*/
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
/*
      UTFT_clrScr (); // стираем всю информацию с дисплея
      UTFT_setFont (BigFont); // устанавливаем большой шрифт
      UTFT_setColor2 (VGA_BLUE); // устанавливаем синий цвет текста
      UTFT_print ("BigFont", CENTER, 10,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 100 точек)
      UTFT_print ("12345678", CENTER, 10,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 115 точек)
      tls_os_time_delay (HZ * 3);
      //

      UTFT_setFont (SmallFont); // устанавливаем большой шрифт
      UTFT_print ("SmallFont", CENTER, 20,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 100 точек)
      UTFT_print ("12345678", CENTER, 15,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 115 точек)
      tls_os_time_delay (HZ * 3);
      //
      UTFT_setFont (SevenSegNumFont); // устанавливаем шрифт имитирующий
                                      // семисегментный индикатор
      UTFT_setColor2 (VGA_FUCHSIA); // устанавливаем пурпурный цвет текста
      UTFT_print ("1234567890", CENTER, 10,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 150 точек)
      tls_os_time_delay (HZ * 3);

      UTFT_setFont (BigFont);
      UTFT_setColor2 (VGA_WHITE);
      UTFT_setBackColor2 (VGA_TRANSPARENT);
      UTFT_print ("BigF(c \xA3"
                  "o"
                  "\x99\x99"
                  "ep"
                  "\x9b\x9f"
                  "o"
                  "\x9e"
                  " pycc"
                  "\x9f"
                  "o"
                  "\x98"
                  "o):",
                  CENTER, 5, 5);

      //      UTFT_print ("АБВГДЕЁЖЗИЙКЛМН", CENTER, 150, 0);
      tls_os_time_delay (HZ * 3);
*/
      UTFT_setFont (SmallFont); // устанавливаем большой шрифт
      UTFT_print ("SmallFont", CENTER, 40,
                  0); // выводим текст на дисплей (выравнивание по ширине -
/*
      UTFT_setFont (SmallSymbolFont); // устанавливаем шрифт имитирующий
      UTFT_print ("\x20\x21\x22\x23\x24\x25", CENTER, 10,
                  0); // выводим текст на дисплей (выравнивание по ширине -
*/
      tls_os_time_delay (HZ * 3);
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