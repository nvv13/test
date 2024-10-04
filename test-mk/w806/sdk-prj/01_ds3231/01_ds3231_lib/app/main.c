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
#include "wm_rtc.h"
#include "wm_timer.h"

#include "wm_gpio.h"
#include "wm_gpio_afsel.h"
#include "wm_osal.h"

#include "ds3231.h"

#define USER_APP1_TASK_SIZE 2048
static OS_STK UserApp1TaskStk[USER_APP1_TASK_SIZE];
#define USER_APP1_TASK_PRIO 32

static ds3231_t _dev;

void
user_app1_task (void *sdata)
{

  int res;

  puts ("DS3231 RTC test\n");

  i2c_param_t user_i2c = {
    .i2c_freq = 400000,     /* частота i2c в герцах */
    .i2c_scl = WM_IO_PA_01, /* WM_IO_PA_01 or WM_IO_PB_20 */
    .i2c_sda = WM_IO_PA_04, /* WM_IO_PA_04 or WM_IO_PB_19 */
  };

  ds3231_params_t par
      = { .bus = &user_i2c,
          .opt = DS3231_OPT_BAT_ENABLE /* | DS3231_OPT_INTER_ENABLE */ };

  /* initialize the i2c */
  i2c_init (&user_i2c);
  tls_os_time_delay (HZ);

  /* initialize the device */
  res = ds3231_init (&_dev, &par);
  if (res != 0)
    {
      puts ("error: unable to initialize DS3231 [I2C initialization error]");
      return;
    }

  struct tm tblock;

  for (;;) // цикл(1) с подсоединением к wifi и запросом времени
    {
          ds3231_get_time (&_dev, &tblock);
          //            tls_get_rtc (&tblock); // получаем текущее время
          printf (" cur time %d.%02d.%02d %02d:%02d:%02d\r",
                  tblock.tm_year + 1900, tblock.tm_mon + 1, tblock.tm_mday,
                  tblock.tm_hour, tblock.tm_min, tblock.tm_sec);
          tls_os_time_delay (HZ);
          // tls_watchdog_clr ();
    }
}

void
UserMain (void)
{
  printf ("UserMain start \n");
  tls_sys_clk_set (CPU_CLK_240M);

  tls_os_task_create (NULL, NULL, user_app1_task, NULL,
                      (void *)UserApp1TaskStk, /* task's stack start address */
                      USER_APP1_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      USER_APP1_TASK_PRIO, 0);
}
