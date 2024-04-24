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
#include "wm_timer.h"
//#include "wm_efuse.h"
//#include "wm_mem.h"
//#include "wm_regs.h"
#include "wm_cpu.h"
#include "air103_def.h"

// extern void UserMain(void);

#define DEMO_TASK_SIZE 2048
static OS_STK DemoTaskStk[DEMO_TASK_SIZE];
#define DEMO_TASK_PRIO 32

#define IR_QUEUE_SIZE 32
static tls_os_queue_t *ir_code_msg_q = NULL;

// static volatile u32 u32_us_tic = 0;

#ifndef UINT32_MAX
#define UINT32_MAX (4294967295U)
#endif

static volatile u32 u32_start = 0;

static void
tic_timer_irq (u8 *arg)
{
  u32 u32_us_tic = tls_os_get_time (); //каждые 2 ms (милисикунды) один тик....

  u32 u32_cnt_tic = 0; // вычислим сколько us прошло
  if (u32_us_tic < u32_start)
    u32_cnt_tic = UINT32_MAX - u32_us_tic + u32_start;
  else
    u32_cnt_tic = u32_us_tic - u32_start;

  u32_start = u32_us_tic; // сохраним для сравнения

  tls_os_queue_send (ir_code_msg_q, (void *)u32_cnt_tic, 0);
}

// console task use UART0 as communication port with PC
void
demo_console_task (void *sdata)
{
  printf ("user task\n");

  tls_gpio_cfg (AIR103_LED_D1, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);
  tls_gpio_cfg (AIR103_LED_D2, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);
  tls_gpio_cfg (AIR103_LED_D3, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);
  u8 u8_led_state = 0;

  tls_os_queue_create (&ir_code_msg_q, IR_QUEUE_SIZE);
  if (ir_code_msg_q == NULL)
    printf ("tls_os_queue_create - error\n");
  else
    printf ("tls_os_queue_create - recive_ir_msg_q\n");

  tls_os_queue_send (ir_code_msg_q, (void *)100, 0);

  u8 timer_id;
  struct tls_timer_cfg timer_cfg;
  timer_cfg.unit = TLS_TIMER_UNIT_MS;
  timer_cfg.timeout = 100; //
  timer_cfg.is_repeat = 1;
  timer_cfg.callback = (tls_timer_irq_callback)tic_timer_irq;
  timer_cfg.arg = NULL;
  timer_id = tls_timer_create (&timer_cfg);
  if (timer_id == WM_TIMER_ID_INVALID)
    printf ("timer WM_TIMER_ID_INVALID\n");

  tls_timer_start (timer_id);
  printf ("timer %d us start\n", timer_cfg.timeout);

  u32 scan_IR_msg = 0;
  u16 u16_total_count = 0;

  for (;;)
    {
      tls_os_queue_receive (ir_code_msg_q, (void **)&scan_IR_msg, 0, 0);
      // printf ("tls_os_queue_receive: 0x%08x , count=%d \n", scan_IR_msg,
      // u16_total_count++);
      printf ("tls_os_queue_receive: %d , count=%d \n", scan_IR_msg,
              u16_total_count++);

      if (scan_IR_msg % 2)
        u8_led_state = 1;
      else
        u8_led_state = 0;

      tls_gpio_write (AIR103_LED_D1, u8_led_state);
      tls_gpio_write (AIR103_LED_D2, u8_led_state);
      tls_gpio_write (AIR103_LED_D3, ~u8_led_state);
    }
}

void
UserMain (void)
{
  //  tls_sys_clk_set (CPU_CLK_240M);

  tls_sys_clk sysclk;
  tls_sys_clk_get (&sysclk);
  printf ("  sysclk.apbclk %d\n", sysclk.apbclk);
  printf ("  sysclk.cpuclk %d\n", sysclk.cpuclk);

  tls_os_task_create (NULL, NULL, demo_console_task, NULL,
                      (void *)DemoTaskStk, /* task's stack start address */
                      DEMO_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      DEMO_TASK_PRIO, 0);

  //	while(1)
  //	{
  //	}
}
