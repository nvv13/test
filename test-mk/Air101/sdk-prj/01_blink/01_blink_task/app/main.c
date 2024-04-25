/*****************************************************************************
 *
 * File Name : main.c
 *
 * Description: main
 *
 * Date : 2022-05-19
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
//#include "wm_type_def.h"
//#include "wm_uart.h"
#include "wm_gpio.h"
//#include "wm_pwm.h"
//#include "wm_params.h"
#include "wm_osal.h"
//#include "wm_efuse.h"
//#include "wm_mem.h"
//#include "wm_regs.h"
#include "air101_def.h"



//extern void UserMain(void);


#define DEMO_TASK_SIZE 2048
static OS_STK DemoTaskStk[DEMO_TASK_SIZE];
#define DEMO_TASK_PRIO 32


// console task use UART0 as communication port with PC
void
demo_console_task (void *sdata)
{
  printf ("user task\n");
  tls_gpio_cfg (BUILDIN_LED_D1, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);
  tls_gpio_cfg (BUILDIN_LED_D2, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);
  tls_gpio_cfg (BUILDIN_LED_D3, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);

  u8 u8_led_state = 0;

  for (;;)
    {
      tls_gpio_write (BUILDIN_LED_D1, ~u8_led_state);
      tls_gpio_write (BUILDIN_LED_D2, u8_led_state);
      tls_gpio_write (BUILDIN_LED_D3, u8_led_state);
      tls_os_time_delay (HZ / 2);
      u8_led_state = ~u8_led_state;
    }
}

void
UserMain (void)
{

  tls_os_task_create (NULL, NULL, demo_console_task, NULL,
                      (void *)DemoTaskStk, /* task's stack start address */
                      DEMO_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      DEMO_TASK_PRIO, 0);

  //	while(1)
  //	{
  //	}
}
