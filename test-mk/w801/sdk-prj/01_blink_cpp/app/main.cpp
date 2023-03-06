/*****************************************************************************
 *
 * File Name : main.cpp
 *
 * Description: use C and C++ mix
 *    https://isocpp.org/wiki/faq/mixing-c-and-cpp
 *
 * Date : 2023-02-16
 *****************************************************************************/

#if defined(__cplusplus)
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
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
  //#include "wm_regs.h"

#if defined(__cplusplus)
}
#endif

class CBlink
{
private:
  enum tls_io_name blink_pin;
  u8 u8_led_state;

public:
  CBlink (tls_io_name pin)
  {
    blink_pin = pin;
    u8_led_state = 0;
    tls_gpio_cfg (blink_pin, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);
    tls_gpio_write (blink_pin, u8_led_state);
  }
  void
  Toggle (void)
  {
    u8_led_state = ~u8_led_state;
    tls_gpio_write (blink_pin, u8_led_state);
  }
};

extern "C" int
UserMain (void)
{
  union
  {
    short s;
    char c[sizeof (short)];
  } un;
  un.s = 0x0102;
  printf (" CPU:xt804 ");
  if (sizeof (short) == 2)
    {
      if (un.c[0] == 1 && un.c[1] == 2)
        printf ("big-endiann");
      else if (un.c[0] == 2 && un.c[1] == 1)
        printf ("little-endiann");
      else
        printf ("unknownn");
    }
  else
   printf("sizeof(short) = %dn", sizeof(short));

  CBlink ob_Blink1 = CBlink (WM_IO_PB_05);
  CBlink ob_Blink2 = CBlink (WM_IO_PB_25);
  while (1)
    {
      ob_Blink1.Toggle ();
      tls_os_time_delay (HZ / 2);
      ob_Blink1.Toggle ();
      tls_os_time_delay (HZ / 2);
      ob_Blink1.Toggle ();
      tls_os_time_delay (HZ / 2);
      ob_Blink2.Toggle ();
    }
  return 0;
}
