#include "wm_type_def.h"

#include "wm_pmu.h"
#include "wm_gpio.h"

#include <stdio.h>
#include <string.h>

volatile static u8 u8_led_state = 0;

static void
pmu_timer1_irq (u8 *arg)
{
  tls_pmu_timer1_stop ();
  u8_led_state = ~u8_led_state;
  printf ("pmu timer1 irq\n");
}

void
UserMain (void)
{
  enum tls_io_name pin1 = WM_IO_PB_25;

  tls_gpio_cfg (pin1, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);
  //tls_gpio_cfg (WM_IO_PB_11, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);
  //tls_gpio_cfg (WM_IO_PB_16, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);
  //tls_gpio_cfg (WM_IO_PB_25, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);

  tls_pmu_clk_select (0); /*0:select 32K RC osc, 1: select 40M divide clock*/
  tls_pmu_timer1_isr_register ((tls_pmu_irq_callback)pmu_timer1_irq, NULL);

  for (;;)
    {

      tls_pmu_timer1_start (1000);
      printf ("pmu timer1 test start\n");

      printf ("pmu will sleep\n");
      tls_pmu_sleep_start (); /* tls_pmu_standby_start ()  If you want to
                                 verify sleep function, using function
                                 tls_pmu_sleep_start()*/
      printf ("pmu enter sleep\n");

      tls_gpio_write (pin1, u8_led_state);
      //tls_gpio_write (WM_IO_PB_11, u8_led_state);
      //tls_gpio_write (WM_IO_PB_16, u8_led_state);
      //tls_gpio_write (WM_IO_PB_25, u8_led_state);
    }
}
