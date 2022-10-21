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

#include "w_flash_cfg.h"
#include "w_wifi.h"

#include "decode_cmd.h"
#include "el_cmd.h"

#define DEMO_TASK_SIZE 1024
static OS_STK DemoTaskStk[DEMO_TASK_SIZE];
#define DEMO_TASK_PRIO 32

#define DEMO_SOCK_S_TASK_SIZE 1024
static OS_STK sock_s_task_stk[DEMO_SOCK_S_TASK_SIZE];
#define DEMO_SOCK_S_PRIO (DEMO_TASK_PRIO + 1)

u16 i_light = 10;
u16 i_swith = 0;
u8 u8_wait_start_ota_upgrade = 0;
u8 u8_start_reconfigure = 0;
u16 i_mode_global = 0;
u16 i_max_out = 0;

/**
 * @brief   Allocate the device descriptor
 */
ws2812b_t dev;

void
demo_console_task (void *sdata)
{
  tls_sys_clk_set (CPU_CLK_240M); // нам мужно 240MHz, под это всё подогнано

  dev.led_numof = 60;
  dev.data_pin = WM_IO_PB_17;
  // dev.mode = WS_PIN_MODE;
  dev.mode = WS_SPI_MODE_8bit;
  dev.rgb = WS_RGB_MODE;
  ws2812b_init (&dev);
  /* initialize all LED color values to black (off) */
  el_init ();
  puts ("Initialization done.");

  tls_watchdog_init (60 * 1000 * 1000); // u32 usec около 1 минуты

  u8 u8_wifi_state = 0;
  for (;;) // цикл(1) с подсоединением к wifi и запросом времени
    {
      while (u8_wifi_state == 0)
        {
          printf ("trying to connect wifi\n");
          if (u8_wifi_state == 0
              && demo_connect_net ("bred1", "9115676369") == WM_SUCCESS)
            u8_wifi_state = 1;
          else
            {
              tls_os_time_delay (5000);
            }
        }

      puts ("WS2812B Test App");

      while (u8_wifi_state)
        {

          el_loop (i_swith);

          tls_watchdog_clr (); //сбросить
          if (u8_wait_start_ota_upgrade)
            {
              u8_wait_start_ota_upgrade = 0;
              printf ("OTA upgrade start, try = " OTA_PATH_FILE "\n");
              t_http_fwup (OTA_PATH_FILE);
              printf ("OTA upgrade stop, error\n"); //если в это место попало,
                                                    //значит какая-то ошибка
                                                    //случилась и прошивка не
                                                    //скачалась
            }
          if (u8_start_reconfigure)
            {
              u8_start_reconfigure = 0;
              ws2812b_init (&dev);
            }
        }
    }
}

void
sock_s_task (void *sdata)
{

  while (1)
    {
      int i_port = 5555;
      printf ("create_socket_server  i_port=%d\n", i_port);
      create_socket_server (i_port);
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

  tls_os_task_create (NULL, NULL, demo_console_task, NULL,
                      (void *)DemoTaskStk, /* task's stack start address */
                      DEMO_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      DEMO_TASK_PRIO, 0);

  tls_os_task_create (NULL, NULL, sock_s_task, NULL,
                      (void *)sock_s_task_stk, /* task's stack start address */
                      DEMO_SOCK_S_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      DEMO_SOCK_S_PRIO, 0);

  //	while(1)
  //	{
  //	}
}
