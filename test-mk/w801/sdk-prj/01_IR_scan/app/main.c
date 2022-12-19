/*****************************************************************************
 *
 * File Name : main.c
 *
 * Description: main
 *
 * Date : 2022-06-05
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "wm_type_def.h"
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
#include "wm_cpu.h"
#include "wm_regs.h"
#include "wm_rtc.h"
#include "wm_timer.h"
#include "wm_watchdog.h"
//#include "csi_core.h"

#include "HD44780LCD.h"
#include "n_delay.h"

static volatile int i_tsop_one = 0;
static volatile int i_tsop_zero = 0;

#define PA1_ISR_IO_TSOP48 WM_IO_PA_01
static void
isr_callback_tsop48 (void *context)
{

  u16 ret = tls_get_gpio_irq_status (PA1_ISR_IO_TSOP48);
  if (ret)
    {
      tls_clr_gpio_irq_status (PA1_ISR_IO_TSOP48);
      // if(i_dreb2==0)
      {
        if (tls_gpio_read (PA1_ISR_IO_TSOP48))
          i_tsop_one++;
        else
          i_tsop_zero++;
      }
    }
}

static volatile int i_tsop = 0;

static void
tic_timer_irq (u8 *arg) //
{
  if (i_tsop_zero > 0 || i_tsop_one > 0) //
    {
      if (i_tsop_zero > i_tsop_one)
        i_tsop--;
      else
        i_tsop++;
      i_tsop_zero = 0;
      i_tsop_one = 0;
    }
}

const u8 u8_row = 2;
const u8 u8_col = 16;

void
UserMain (void)
{
  printf ("user task LCD test u8_row=%d, u8_col=%d\n", u8_row, u8_col);

  // tls_sys_clk_set (CPU_CLK_240M);

  HD44780LCD_HD44780LCD (u8_row, u8_col); // instantiate an object

  HD44780LCD_LCDInit (LCDCursorTypeOn);
  HD44780LCD_LCDClearScreen ();
  HD44780LCD_LCDBackLightSet (true);

  u8 timer_id;
  struct tls_timer_cfg timer_cfg;

  // timer_cfg.unit = TLS_TIMER_UNIT_MS;
  timer_cfg.unit = TLS_TIMER_UNIT_US; //
  timer_cfg.timeout = 50;
  timer_cfg.is_repeat = 1;
  timer_cfg.callback = (tls_timer_irq_callback)tic_timer_irq;
  timer_cfg.arg = NULL;
  timer_id = tls_timer_create (&timer_cfg);
  tls_timer_start (timer_id);
  printf ("timer start\n");

  //
  tls_gpio_cfg (PA1_ISR_IO_TSOP48, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);
  tls_gpio_isr_register (PA1_ISR_IO_TSOP48, isr_callback_tsop48, NULL);
  tls_gpio_irq_enable (
      PA1_ISR_IO_TSOP48,
      WM_GPIO_IRQ_TRIG_DOUBLE_EDGE); /**< both rising edge and falling edge
                                        arise the interrupt */
  printf ("\nPA1_ISR_IO_TSOP48 gpio %d DOUBLE_EDGE isr\n", PA1_ISR_IO_TSOP48);

  for (;;)
    {
    }
}
