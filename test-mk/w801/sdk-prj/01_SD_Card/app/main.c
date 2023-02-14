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

#include "wm_gpio.h"
#include "wm_osal.h"

#include "n_delay.h" //нужна для UTFT 

#include "UTFT.h"

#define USER_APP1_TASK_SIZE 2048
static OS_STK UserApp1TaskStk[USER_APP1_TASK_SIZE];
#define USER_APP1_TASK_PRIO 32

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

  int i_file_num = 0;
  char FileName[100];
  while (1)
    { //

      sprintf (FileName, "0:0-%d.raw", i_file_num);
      UTFT_loadBitmap (
          0, 0, 480, 320,
          FileName); // выводим на дисплей картинку начиная с координаты 0,0
                     // размером 480,320 из файла 0-x.raw
      tls_os_time_delay (HZ * 1);
      if (++i_file_num > 30)
        i_file_num = 0;

    } //
}

void
UserMain (void)
{
  printf ("UserMain start");
  tls_sys_clk_set (CPU_CLK_240M);

  tls_os_task_create (NULL, NULL, user_app1_task, NULL,
                      (void *)UserApp1TaskStk, /* task's stack start address */
                      USER_APP1_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      USER_APP1_TASK_PRIO, 0);
}
