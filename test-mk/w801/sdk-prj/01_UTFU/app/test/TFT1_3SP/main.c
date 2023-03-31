


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
  printf ("user_app1_task start TFT01_3SP 240x240 SPI st7789\n");

  // подключаем библиотеку UTFT

  // TFT01_3SP - для ST7789
  UTFT_UTFT (TFT01_3SP
 , (u8)WM_IO_PB_17  //RS  SDA
 , (u8)WM_IO_PB_15  //WR  SCL
 , (u8)NO_GPIO_PIN //WM_IO_PB_22  //CS  CS
 , (u8)WM_IO_PB_21  //RST reset RES
 , (u8)WM_IO_PB_23 //SER => DC !
 , 2500000
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



  //UTFT_UTFT(byte model, byte RS, byte WR, byte CS, byte RST, byte SER, u32 spi_freq);
  //                               byte RS,         byte WR,         byte CS,
  //                               byte RST, byte SER, u32 spi_freq

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

      UTFT_setColor2 (VGA_WHITE); // 
      for(int i=1;i<40;i++){
      UTFT_drawRect (1, 1, i*4, i*2); 
      }
 
      tls_os_time_delay (HZ*3); // 
  
      UTFT_fillScr2 (VGA_BLACK);

      UTFT_setColor2 (VGA_GREEN); // Устанавливаем зелёный цвет
      UTFT_drawRect (5, 5, 160-5,
                     80-5); // Рисуем прямоугольник (с противоположными углами в
                           // координатах 10x20 - 170x100)
      tls_os_time_delay (HZ); // 

      UTFT_setColor2 (VGA_RED); // Устанавливаем красный цвет
      UTFT_drawLine (
          1, 1, 150,
          70); // Рисуем линию (через точки с координатами 10x10 - 170x10)
      tls_os_time_delay (HZ); // 

      UTFT_setColor2 (VGA_BLUE); // Устанавливаем синий цвет
      UTFT_drawRoundRect (
          5, 5, 130,
          55); // Рисуем прямоугольник со скруглёнными углами (с
                // противоположными углами в координатах 10x110 - 170x210)
      tls_os_time_delay (HZ); // 
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
      UTFT_setColor2 (VGA_PURPLE); // Устанавливаем фиолетовый цвет
      UTFT_drawCircle (
          80, 40,
          30); // Рисуем окружность (с центром в точке 350x90 и радиусом 70)

      UTFT_fillCircle (80, 40, 30); // Рисуем закрашенную окружность (с
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
      UTFT_print ("BigFont", CENTER, 40,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 100 точек)
      tls_os_time_delay (HZ); // заливаем дисплей тем. синим,  ждём 1  секунду
      UTFT_clrScr (); // стираем всю информацию с дисплея
      UTFT_setColor2 (VGA_RED); // устанавливаем 
      UTFT_setBackColor2 (VGA_TRANSPARENT);
      UTFT_print ("12:35", CENTER, 40,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 115 точек)
      tls_os_time_delay (HZ * 3);
      //

      UTFT_clrScr (); // стираем всю информацию с дисплея
      UTFT_setFont (SmallFont); // устанавливаем большой шрифт
      UTFT_print ("SmallFontTest", CENTER, 10,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 100 точек)
      UTFT_print ("12345678", CENTER, 40,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 115 точек)
      tls_os_time_delay (HZ * 3);
      //
      UTFT_setFont (SevenSegNumFont); // устанавливаем шрифт имитирующий
                                      // семисегментный индикатор
      UTFT_clrScr (); // стираем всю информацию с дисплея
      UTFT_setColor2 (VGA_FUCHSIA); // устанавливаем пурпурный цвет текста
      UTFT_print ("12345", CENTER, 10,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 150 точек)
      tls_os_time_delay (HZ * 3);


      UTFT_setFont (SmallFont); // устанавливаем большой шрифт
      UTFT_print ("SmallFont", CENTER, 60,
                  0); // выводим текст на дисплей (выравнивание по ширине -

      UTFT_setFont (SmallSymbolFont); // устанавливаем шрифт имитирующий
      UTFT_print ("\x20\x21\x22\x23\x24\x25", CENTER, 5,
                  0); // выводим текст на дисплей (выравнивание по ширине -
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
