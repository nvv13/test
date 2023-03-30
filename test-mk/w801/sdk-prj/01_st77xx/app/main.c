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



#include "main.h"
#include <button.h>

#include "st77xx/dispcolor.h"
#include "st77xx/fonts/font.h"
#include "st77xx/delay.h"

#include "test.h"

#if (DISPLAY == DISPLAY_160x80)
  #include "test80x160.h"
#else
  #include "test240x240.h"
#endif



#define USER_APP1_TASK_SIZE 2048
static OS_STK UserApp1TaskStk[USER_APP1_TASK_SIZE];
#define USER_APP1_TASK_PRIO 32


void
user_app1_task (void *sdata)
{
  printf ("user_app1_task start\n");




#if (DISPLAY == DISPLAY_160x80)
  // Инициализация дисплея
  dispcolor_Init(160, 80);
  //dispcolor_Init(80, 160);
  // Инициализация входа для кнопки
  button_Init();
  // Тест вывода основных цветов
  Test_Colors();
  // Тест вывода приглашения с плавным затуханием
  Test_TextFading("Электроника\r\nв объективе", 30, 23);
  // Тест вывода текста разными шрифтами
  Test80x160_Text();
  // Тест вывода простой 2D-графики
  Test80x160_Graphics();

  while (1)
  {
    // Вывод картинок с SD-флешки
    Test80x160_Images();
  }
#else
  // Инициализация дисплея
  dispcolor_Init(240, 240);
  // Инициализация входа для кнопки
  button_Init();
  // Тест вывода основных цветов
  Test_Colors();
  // Тест вывода приглашения с плавным затуханием
  Test_TextFading("Электроника\r\nв объективе", 75, 100);
  // Тест вывода текста разными шрифтами
  Test240x240_Text();
  // Тест вывода простой 2D-графики
  Test240x240_Graphics();

  while (1)
  {
    // Вывод картинок с SD-флешки
    Test240x240_Images();
  }
#endif


//  while (1)
//    { //
//
//      tls_os_time_delay (HZ * 3);

//    } //
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
