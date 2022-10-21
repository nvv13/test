/*****************************************************************************
 *
 * File Name : main.c
 *
 * Description: main
 *
 * Autor : nvv13
 *
 * Date : 2022-05-19
 *****************************************************************************/

#include "wm_type_def.h"

#include "wm_cpu.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "wm_uart.h"
//#include "wm_gpio.h"
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
#include "wm_watchdog.h"

#include "ws2812b.h"

#include "TM1637Display.h"
#include "w_flash_cfg.h"

#include "el_cmd.h"

#define DEMO_TASK_SIZE 1024
static OS_STK DemoTaskStk[DEMO_TASK_SIZE];
#define DEMO_TASK_PRIO 32

u16 i_swith = 18;
u16 i_cnt = 0;
u8 u8_start_reconfigure = 0;
u8 u8_tic = 0;

/**
 * @brief   Allocate the device descriptor
 */
ws2812b_t dev;

void
demo_console_task (void *sdata)
{
  // tls_sys_clk_set (CPU_CLK_240M); // нам мужно 240MHz, под это всё подогнано

  dev.led_numof = 60;
  dev.mode = WS_PIN_MODE;
  dev.data_pin = WM_IO_PB_17;
  // dev.mode = WS_SPI_MODE_4bit;
  ws2812b_init (&dev);
  /* initialize all LED color values to black (off) */
  el_init ();
  puts ("Initialization done.");

  tls_watchdog_init (60 * 1000 * 1000); // u32 usec около 1 минуты

  puts ("WS2812B Test App");

  setBrightness (20, 1);
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
  setSegments (data, 4, 0);
  // uint8_t PointData = 0x00;

  for (;;)
    {

      el_loop (i_swith);

      u8_tic = ~u8_tic;
      // PointData = 0x00;
      data[0] = encodeSign (u8_tic);
      data[1] = encodeDigit (i_swith / 10);
      data[2] = encodeDigit (i_swith % 10);
      data[3] = encodeSign (u8_tic);
      setSegments (data, 4, 0);

      tls_watchdog_clr (); //сбросить
      if (u8_start_reconfigure)
        {
          u8_start_reconfigure = 0;
          ws2812b_init (&dev);
        }

      if (i_cnt++ > 100)
        {
          i_cnt = 0;
          if (i_swith++ > 48)
            i_swith = 1;
        }
    }
}

void
UserMain (void)
{
  printf ("user task\n");

  tls_sys_clk sysclk;
  tls_sys_clk_get (&sysclk);
  printf ("  sysclk.apbclk %d\n", sysclk.apbclk);
  printf ("  sysclk.cpuclk %d\n", sysclk.cpuclk);

  TM1637Display (WM_IO_PB_21, WM_IO_PB_22, DEFAULT_BIT_DELAY);

  tls_os_task_create (NULL, NULL, demo_console_task, NULL,
                      (void *)DemoTaskStk, /* task's stack start address */
                      DEMO_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      DEMO_TASK_PRIO, 0);

  //	while(1)
  //	{
  //	}
}
