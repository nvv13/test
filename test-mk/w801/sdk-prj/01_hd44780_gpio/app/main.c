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

void
UserMain (void)
{
  printf ("user task\n");

  // tls_sys_clk_set (CPU_CLK_240M);

  HD44780LCD_HD44780LCD (2, 16); // instantiate an object

  HD44780LCD_LCDInit (LCDCursorTypeOn);
  HD44780LCD_LCDClearScreen ();
  HD44780LCD_LCDBackLightSet (true);
  HD44780LCD_LCDGOTO (LCDLineNumberOne, 0);

  int i_cnt = 0;

  for (;;)
    {
      tls_os_time_delay (HZ);

      HD44780LCD_LCDBackLightSet (true);

      HD44780LCD_LCDGOTO (LCDLineNumberOne, 0);

      char testString[] = "Hello World";
      HD44780LCD_LCDSendString (testString);
      HD44780LCD_LCDSendChar ('!'); // Display a single character

      HD44780LCD_LCDGOTO (LCDLineNumberTwo, 0);
      HD44780LCD_printf ("i_cnt=%d", i_cnt);

      if (++i_cnt > 999999)
        i_cnt = 0;
    }
}
