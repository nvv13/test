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
//#include "wm_pwm.h"
//#include "wm_params.h"
#include "wm_osal.h"
//#include "wm_efuse.h"
//#include "wm_mem.h"
//#include "wm_regs.h"
#include "csi_core.h"
#include "wm_cpu.h"
#include "air103_def.h"

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
  printf (" CPU:xt804 - ");
  if (sizeof (short) == 2)
    {
      if (un.c[0] == 1 && un.c[1] == 2)
        printf ("big-endiann \n");
      else if (un.c[0] == 2 && un.c[1] == 1)
        printf ("little-endiann \n");
      else
        printf ("unknownn \n");
    }
  else
    printf ("sizeof(short) = %d \n", sizeof (short));

  tls_sys_clk sysclk;
  u32 t1 = 0;
  u32 t2 = 0;
  u32 t3 = 0;
  u32 t4 = 0;
  u32 t5 = 0;
  u32 t6 = 0;


  CBlink ob_Blink1 = CBlink (BUILDIN_LED_D1);
  CBlink ob_Blink2 = CBlink (BUILDIN_LED_D2);
  CBlink ob_Blink3 = CBlink (BUILDIN_LED_D3);
  while (1)
    {
      tls_sys_clk_set (CPU_CLK_240M);
      tls_sys_clk_get (&sysclk);
      printf (
          "sysclk.cpuclk=%d csi_coret_get_load=%d  csi_coret_get_value=%d\n",
          sysclk.cpuclk, csi_coret_get_load (), csi_coret_get_value ());

      t1 = csi_coret_get_value ();
      t2 = csi_coret_get_value ();
      t3 = csi_coret_get_value ();
      t4 = csi_coret_get_value ();
      t5 = csi_coret_get_value ();
      t6 = csi_coret_get_value ();
      printf ("%d %d %d %d %d %d\n", t1, t2, t3, t4, t5, t6);

      ob_Blink1.Toggle ();
      tls_os_time_delay (HZ);

      tls_sys_clk_set (CPU_CLK_160M);
      tls_sys_clk_get (&sysclk);
      printf (
          "sysclk.cpuclk=%d csi_coret_get_load=%d  csi_coret_get_value=%d\n",
          sysclk.cpuclk, csi_coret_get_load (), csi_coret_get_value ());

      t1 = csi_coret_get_value ();
      t2 = csi_coret_get_value ();
      t3 = csi_coret_get_value ();
      t4 = csi_coret_get_value ();
      t5 = csi_coret_get_value ();
      t6 = csi_coret_get_value ();
      printf ("%d %d %d %d %d %d\n", t1, t2, t3, t4, t5, t6);

      ob_Blink1.Toggle ();
      tls_os_time_delay (HZ);

      tls_sys_clk_set (CPU_CLK_80M);
      tls_sys_clk_get (&sysclk);
      printf (
          "sysclk.cpuclk=%d csi_coret_get_load=%d  csi_coret_get_value=%d\n",
          sysclk.cpuclk, csi_coret_get_load (), csi_coret_get_value ());

      t1 = csi_coret_get_value ();
      t2 = csi_coret_get_value ();
      t3 = csi_coret_get_value ();
      t4 = csi_coret_get_value ();
      t5 = csi_coret_get_value ();
      t6 = csi_coret_get_value ();
      printf ("%d %d %d %d %d %d\n", t1, t2, t3, t4, t5, t6);

      ob_Blink1.Toggle ();
      tls_os_time_delay (HZ);

      tls_sys_clk_set (CPU_CLK_40M);
      tls_sys_clk_get (&sysclk);
      printf (
          "sysclk.cpuclk=%d csi_coret_get_load=%d  csi_coret_get_value=%d\n",
          sysclk.cpuclk, csi_coret_get_load (), csi_coret_get_value ());

      t1 = csi_coret_get_value ();
      t2 = csi_coret_get_value ();
      t3 = csi_coret_get_value ();
      t4 = csi_coret_get_value ();
      t5 = csi_coret_get_value ();
      t6 = csi_coret_get_value ();
      printf ("%d %d %d %d %d %d\n", t1, t2, t3, t4, t5, t6);

      ob_Blink2.Toggle ();
      tls_os_time_delay (HZ);

      tls_sys_clk_set (CPU_CLK_2M);
      tls_sys_clk_get (&sysclk);
      printf (
          "sysclk.cpuclk=%d csi_coret_get_load=%d  csi_coret_get_value=%d\n",
          sysclk.cpuclk, csi_coret_get_load (), csi_coret_get_value ());

      t1 = csi_coret_get_value ();
      t2 = csi_coret_get_value ();
      t3 = csi_coret_get_value ();
      t4 = csi_coret_get_value ();
      t5 = csi_coret_get_value ();
      t6 = csi_coret_get_value ();
      printf ("%d %d %d %d %d %d\n", t1, t2, t3, t4, t5, t6);

      ob_Blink2.Toggle ();
      tls_os_time_delay (HZ);

      ob_Blink3.Toggle ();
    }
  return 0;
}
