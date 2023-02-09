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

#define USER_APP2_TASK_SIZE 2048
static OS_STK UserApp2TaskStk[USER_APP2_TASK_SIZE];
#define USER_APP2_TASK_PRIO 33

void
user_app1_task (void *sdata)
{
  printf ("user_app1_task start\n");



//Цветной графический дисплей 3.2 TFT 480x320 MEGA http://iarduino.ru/shop/Displei/cvetnoy-graficheskiy-displey-3-2-tft-480x320.html
                // подключаем библиотеку UTFT
UTFT_UTFT(TFT32MEGA_2, 38,39,40,41,0);           // объявляем объект myGLCD класса библиотеки UTFT указывая тип дисплея TFT32MEGA и номера выводов Arduino к которым подключён дисплей: RS, WR, CS, RST. Выводы параллельной шины данных не указываются
//          TFT32MEGA_2                        // если изображение на дисплее отображается зеркально, значит для инициализации Вашего дисплея, нужно указать не TFT32MEGA, а TFT32MEGA_2.
                                               // (тип TFT32MEGA - для дисплеев на базе чипа ILI9481, а тип TFT32MEGA_2 - для дисплеев на базе чипа HX8357С).
                                               //
   UTFT_InitLCD(LANDSCAPE);                           // инициируем дисплей
   UTFT_clrScr();                            // стираем всю информацию с дисплея
                                               //
for(;;){                                   //
   UTFT_fillScr2(VGA_RED   ); tls_os_time_delay(HZ);    // заливаем дисплей красным,     ждём 1 секунду
   UTFT_fillScr2(VGA_GREEN ); tls_os_time_delay(HZ);    // заливаем дисплей зелёным,     ждём 1 секунду
   UTFT_fillScr2(VGA_BLUE  ); tls_os_time_delay(HZ);    // заливаем дисплей синим,       ждём 1 секунду
   UTFT_fillScr2(VGA_SILVER); tls_os_time_delay(HZ);    // заливаем дисплей серебристым, ждём 1 секунду
   UTFT_fillScr2(VGA_MAROON); tls_os_time_delay(HZ);    // заливаем дисплей бордовым,    ждём 1 секунду
   UTFT_fillScr2(VGA_NAVY  ); tls_os_time_delay(HZ);    // заливаем дисплей тем. синим,  ждём 1 секунду
}                                              //


}

void
user_app2_task (void *sdata)
{
  printf ("user_app2_task start\n");
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

  tls_os_task_create (NULL, NULL, user_app2_task, NULL,
                      (void *)UserApp2TaskStk, /* task's stack start address */
                      USER_APP2_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      USER_APP2_TASK_PRIO, 0);
}
