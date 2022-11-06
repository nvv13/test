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

#include "HD44780Display.h"

#include "PCF857X.h"

void
UserMain (void)
{
  printf ("user task\n");

  // tls_sys_clk_set (CPU_CLK_240M);

  //      PCF857X_Init (LCD44780_I2C_FREQ, LCD44780_I2C_SCL, LCD44780_I2C_SDA);

  // u8 num_pin=7;

  //  PCF857X_buf_write (num_pin, 1);
  //  PCF857X_buf_to_gpio_write ();

  //  PCF857X_gpio_write (num_pin, 1);

  // LCD44780_init (X_GPIO_MODE);
  LCD44780_init (X_I2C_MODE);
  LCD44780_SET_BACKLIGHT (1);
  // LCD44780_on ();

  int i_cnt = 0;

  for (;;)
    {
      tls_os_time_delay (HZ);

      //  PCF857X_gpio_write (num_pin, 0);
      // PCF857X_buf_write (num_pin, 0);
      // PCF857X_buf_to_gpio_write ();

      //      tls_os_time_delay (HZ);

      //  PCF857X_gpio_write (num_pin, 1);
      // PCF857X_buf_write (num_pin, 1);
      // PCF857X_buf_to_gpio_write ();

      LCD44780_return_home ();
      LCD44780_printf ("i_cnt=%d", i_cnt);

      if (++i_cnt > 999999)
        i_cnt = 0;
    }
}
