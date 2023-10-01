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
#include <string.h>
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
#include "wm_timer.h"
#include "wm_watchdog.h"

#include "ws2812b.h"

#include "TM1637Display.h"
#include "w_flash_cfg.h"
#include "w_wifi.h"

#include "IR_Scan.h"
#include "decode_cmd.h"
#include "el_cmd.h"

#define DEMO_TASK_SIZE 1024
static OS_STK DemoTaskStk[DEMO_TASK_SIZE];
#define DEMO_TASK_PRIO 32

#define DEMO_SOCK_S_TASK_SIZE 1024
static OS_STK sock_s_task_stk[DEMO_SOCK_S_TASK_SIZE];
#define DEMO_SOCK_S_PRIO (DEMO_TASK_PRIO + 1)

#define DEMO_MODE_SW 1000
u16 i_light = 10;
u16 i_swith = DEMO_MODE_SW;
static u16 i_swith_demo = 2; // 1...52
u8 u8_wait_start_ota_upgrade = 0;
u8 u8_start_reconfigure = 0;
u16 i_mode_global = 0;
u16 i_max_out = 0;
static u8 volatile u8_dreb = 2; // от дребезга кнопки

static u16 volatile u16_delay_timer_sw = 0;

static int volatile i_delay_timer_IR = -1;
static int volatile i_IR_decode = 0;

static void
demo_timer_irq (u8 *arg) // здесь будет смена режима
{
  if (u8_dreb > 0)
    u8_dreb--;
  //
  if (u16_delay_timer_sw++ > 150) // 15 sec
    {
      if (i_swith == DEMO_MODE_SW)
        {
          if (i_swith_demo++ > 52)
            i_swith_demo = 1;
          extern volatile bool changeFlag;
          changeFlag = true;
        }
      u16_delay_timer_sw = 0;
    }

  if (u32_IR_scan_result != 0)
    {
      u8 u8_dig = 255;
      switch (u32_IR_scan_result)
        {
        case 0xbb44ff00:
          u8_dig = 1;
          break;
        case 0xbc43ff00:
          u8_dig = 2;
          break;
        case 0xf807ff00:
          u8_dig = 3;
          break;
        case 0xf609ff00:
          u8_dig = 4;
          break;
        case 0xe916ff00:
          u8_dig = 5;
          break;
        case 0xf20dff00:
          u8_dig = 6;
          break;
        case 0xf30cff00:
          u8_dig = 7;
          break;
        case 0xa15eff00:
          u8_dig = 8;
          break;
        case 0xf708ff00:
          u8_dig = 9;
          break;
        case 0xa55aff00:
          u8_dig = 0;
          break;
        }
      if (u8_dig != 255) // нажата 1
        {
          i_IR_decode = (i_IR_decode * 10) + u8_dig;
          i_delay_timer_IR = 0; // подождем, вдруг еще цифра будет
        }
      u32_IR_scan_result = 0;
    }

  if (i_delay_timer_IR >= 0)
    {
      if ((i_delay_timer_IR++) > 20) // 2 sec
        {
          i_delay_timer_IR = -1;
          if (i_IR_decode > 0)
            {
              i_swith = i_IR_decode;
              i_IR_decode = 0;
              if (i_swith > 52)
                i_swith = DEMO_MODE_SW;
              extern volatile bool changeFlag;
              changeFlag = true;
            }
        }
    }
}

/*

  кнопка (убрал в прошивке, часто ловила помехи) 

#define DEMO_ISR_IO WM_IO_PA_01
static void
demo_gpio_isr_callback (void *context)
{
  u16 ret = tls_get_gpio_irq_status (DEMO_ISR_IO);
  if (ret)
    {
      tls_clr_gpio_irq_status (DEMO_ISR_IO);
      {
        if (u8_dreb == 0) // защита от ддребезга контактов для кнопки
          {
            if (i_swith == DEMO_MODE_SW)
              i_swith = 2;
            if (i_swith++ > 52)
              i_swith = DEMO_MODE_SW;
            extern volatile bool changeFlag;
            changeFlag = true;
            u8_dreb = 2;
          }
      }
    }
}
*/

/**
 * @brief   Allocate the device descriptor
 */
ws2812b_t dev;

void
demo_console_task (void *sdata)
{
  memset(&dev,0,sizeof(ws2812b_t));
  dev.led_numof = 100;
  dev.data_pin = WM_IO_PB_17;
  // dev.mode = WS_PIN_MODE;
  dev.mode = WS_SPI_MODE_8bit;
  dev.rgb = WS_RGB_MODE; // для ...
  // dev.rgb = WS_GRB_MODE; // для ws2812b обычно
  ws2812b_init (&dev);
  /* initialize all LED color values to black (off) */
  el_init ();

  TM1637Display (WM_IO_PB_21, WM_IO_PB_22, DEFAULT_BIT_DELAY);

  setBrightness (20, 1);
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
  setSegments (data, 4, 0);
  u8 u8_tic = 0;

  u8 timer_id;
  struct tls_timer_cfg timer_cfg;
  timer_cfg.unit = TLS_TIMER_UNIT_MS;
  timer_cfg.timeout = 100; //
  timer_cfg.is_repeat = 1;
  timer_cfg.callback = (tls_timer_irq_callback)demo_timer_irq;
  timer_cfg.arg = NULL;
  timer_id = tls_timer_create (&timer_cfg);
  if (true)
    {
      tls_timer_start (timer_id);
      printf ("timer start\n");
    }

 /*
  кнопка (убрал в прошивке, часто ловила помехи) 

  u16 gpio_pin;       
  gpio_pin = DEMO_ISR_IO;
  tls_gpio_cfg (gpio_pin, WM_GPIO_DIR_INPUT,
                WM_GPIO_ATTR_PULLHIGH); // WM_GPIO_ATTR_FLOATING);
  tls_gpio_isr_register (gpio_pin, demo_gpio_isr_callback, NULL);
  tls_gpio_irq_enable (gpio_pin, WM_GPIO_IRQ_TRIG_RISING_EDGE);
  printf ("\nbutton gpio %d rising isr\n", gpio_pin);
*/

  IR_Scan_create (WM_IO_PA_02, NULL);
  printf ("IR_Scan_create\n");

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

          u8_tic = ~u8_tic;
          if (i_swith == DEMO_MODE_SW)
            {
              el_loop (i_swith_demo);
              data[0] = encodeSign (u8_tic) + (VIEW_POINT_DATA);
              data[1] = encodeDigit (i_swith_demo / 10) + (VIEW_POINT_DATA);
              data[2] = encodeDigit (i_swith_demo % 10) + (VIEW_POINT_DATA);
              data[3] = encodeSign (u8_tic) + (VIEW_POINT_DATA & u8_tic);
            }
          else
            {
              el_loop (i_swith);
              data[0] = encodeSign (u8_tic);
              data[1] = encodeDigit (i_swith / 10);
              data[2] = encodeDigit (i_swith % 10);
              data[3] = encodeSign (u8_tic);
            }
          setSegments (data, 4, 0);

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

  tls_sys_clk_set (CPU_CLK_240M); // нам мужно 240MHz, под это всё подогнано
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
