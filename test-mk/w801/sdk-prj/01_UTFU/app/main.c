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

#include "n_delay.h"

#include "UTFT.h"

#define USER_APP1_TASK_SIZE 2048
static OS_STK UserApp1TaskStk[USER_APP1_TASK_SIZE];
#define USER_APP1_TASK_PRIO 32

extern uint8_t SmallFont[]; // подключаем маленький шрифт
extern uint8_t BigFont[];   // подключаем большой шрифт
extern uint8_t
    SevenSegNumFont[]; // подключаем шрифт имитирующий семисегментный индикатор

void
user_app1_task (void *sdata)
{
  printf ("user_app1_task start\n");

  //Цветной графический дисплей 3.2 TFT 480x320 MEGA
  // http://iarduino.ru/shop/Displei/cvetnoy-graficheskiy-displey-3-2-tft-480x320.html
  // подключаем библиотеку UTFT
  UTFT_UTFT (TFT32MEGA_2, (u8)WM_IO_PA_01, (u8)WM_IO_PA_02, (u8)WM_IO_PA_03,
             (u8)WM_IO_PA_04, 0);
  //                               byte RS,         byte WR,         byte CS,
  //                               byte RST, byte SER
  // объявляем объект myGLCD класса библиотеки UTFT указывая тип дисплея
  // TFT32MEGA и номера выводов Arduino к которым подключён дисплей: RS, WR,
  // CS, RST. Выводы параллельной шины данных не указываются
  //          TFT32MEGA_2                        // если изображение на дисплее
  //          отображается зеркально, значит для инициализации Вашего дисплея,
  //          нужно указать не TFT32MEGA, а TFT32MEGA_2.
  // (тип TFT32MEGA - для дисплеев на базе чипа ILI9481, а тип TFT32MEGA_2 -
  // для дисплеев на базе чипа HX8357С).
  //
  UTFT_InitLCD (LANDSCAPE); // инициируем дисплей
  // UTFT_clrScr();                            // стираем всю информацию с
  // дисплея
  //
  while (1)
    { //

      UTFT_clrScr (); // стираем всю информацию с дисплея

      UTFT_setColor2 (VGA_GREEN); // Устанавливаем зелёный цвет
      UTFT_drawRect (10, 20, 170,
                     100); // Рисуем прямоугольник (с противоположными углами в
                           // координатах 10x20 - 170x100)

      UTFT_setColor2 (VGA_RED); // Устанавливаем красный цвет
      UTFT_drawLine (
          10, 10, 170,
          10); // Рисуем линию (через точки с координатами 10x10 - 170x10)

      UTFT_setColor2 (VGA_BLUE); // Устанавливаем синий цвет
      UTFT_drawRoundRect (
          10, 110, 170,
          210); // Рисуем прямоугольник со скруглёнными углами (с
                // противоположными углами в координатах 10x110 - 170x210)
                //
      UTFT_setColor2 (VGA_LIME); // Устанавливаем лаймовый цвет
      UTFT_fillRect (
          10, 220, 170,
          310); // Рисуем закрашенный прямоугольник (с противоположными углами
                // в координатах 10x220 - 170x310)
                //
      UTFT_setColor2 (VGA_PURPLE); // Устанавливаем фиолетовый цвет
      UTFT_drawCircle (
          350, 90,
          70); // Рисуем окружность (с центром в точке 350x90 и радиусом 70)

      UTFT_fillCircle (350, 240, 70); // Рисуем закрашенную окружность (с
                                      // центром в точке 350x240 и радиусом 70)

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
      UTFT_print ("BigFont", CENTER, 100,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 100 точек)
      UTFT_print ("12345678", CENTER, 115,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 115 точек)
      tls_os_time_delay (HZ * 3);
      //

      UTFT_setFont (SmallFont); // устанавливаем большой шрифт
      UTFT_print ("SmallFont", CENTER, 130,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 100 точек)
      UTFT_print ("12345678", CENTER, 145,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 115 точек)
      tls_os_time_delay (HZ * 3);
      //
      UTFT_setFont (SevenSegNumFont); // устанавливаем шрифт имитирующий
                                      // семисегментный индикатор
      UTFT_setColor2 (VGA_FUCHSIA); // устанавливаем пурпурный цвет текста
      UTFT_print ("1234567890", CENTER, 150,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 150 точек)
      tls_os_time_delay (HZ * 3);

      //UTFT_loadBitmap(0,0,480,320,"0:0-0.raw"); // выводим на дисплей картинку начиная с координаты 0,0 размером 480,320 из файла image_480x320_a.raw
      //tls_os_time_delay (HZ * 3);


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
