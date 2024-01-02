/*

на основе
Mongoose networking library
https://mongoose.ws/documentation/

исходники Mongoose
https://github.com/cesanta/mongoose.git

*/



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

#include "../../../../../../../w_wifi_pass.h"
//#define MY_WIFI_AP "bred8"
//#define MY_WIFI_PASS "123123123"
//#define MY_WIFI_AP "bred1"
//#define MY_WIFI_PASS "9115676369"


#include "mod1/IR_Scan.h"
#include "decode_cmd.h"
#include "el_cmd.h"

#define DEMO_TASK_SIZE 1024
static OS_STK DemoTaskStk[DEMO_TASK_SIZE];
#define DEMO_TASK_PRIO 32

#define DEMO_SOCK_S_TASK_SIZE 4096
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

static u16 volatile u16_delay_timer_sw = 0;

static int volatile i_delay_timer_IR = -1;
static int volatile i_IR_decode = 0;

static void
demo_timer_irq (u8 *arg) // здесь будет смена режима
{
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


  IR_Scan_create (WM_IO_PA_02, NULL);
  printf ("IR_Scan_create\n");

  puts ("Initialization done.");

  tls_watchdog_init (60 * 1000 * 1000); // u32 usec около 1 минуты

  for (;;) // цикл(1) 
    {

      tls_os_time_delay (HZ * 3);

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






























#include "mongoose/mongoose.h"
// Rest of your code ...

const char *s_listening_url = "http://0.0.0.0:80";

#define DF_AR_LAMP_COUNT 3
static unsigned char arLamp[DF_AR_LAMP_COUNT] = { 0, 0, 0 };
#define DF_ON 0 //зависит от того, к земле или к питанию привяханы светодиоды
#define DF_OFF 1
// Mongoose event handler function, gets called by the mg_mgr_poll()

// HTTP request handler function. It implements the following endpoints:
//   /api/log/static - returns contents of log.txt file
//   /api/log/live - hangs forever, and returns live log messages
//   all other URI - serves web_root/ directory
static void
fn (struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
  if (ev == MG_EV_HTTP_MSG)
    {
      struct mg_http_message *hm = (struct mg_http_message *)ev_data;
      if (mg_http_match_uri (hm, "/api/log/live"))
        {
          c->data[0] = 'L'; // Mark that connection as live log listener
          mg_printf (c,
                     "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
        }
      else if (mg_http_match_uri (hm, "/api/sost"))
        {
          int i_lamp = 255;
          if (hm->query.len > 0)
            i_lamp = hm->query.ptr[0] - 48;
          if (i_lamp < DF_AR_LAMP_COUNT)
            {
              MG_INFO (("lamp %d sost %d", i_lamp, arLamp[i_lamp]));
            }
          mg_http_reply (c, 200, "", "%u",
                         (i_lamp < DF_AR_LAMP_COUNT ? arLamp[i_lamp] : 255));
        }
      else if (mg_http_match_uri (hm, "/api/on"))
        {
          int i_lamp = 255;
          if (hm->query.len > 0)
            i_lamp = hm->query.ptr[0] - 48;
          if (i_lamp < DF_AR_LAMP_COUNT)
            {
              arLamp[i_lamp] = 1;
              MG_INFO (("lamp %d on", i_lamp));
              //tls_gpio_write (pin[i_lamp], DF_ON);
            }
          mg_http_reply (c, 200, "", "%u", 123);
        }
      else if (mg_http_match_uri (hm, "/api/off"))
        {
          int i_lamp = 255;
          if (hm->query.len > 0)
            i_lamp = hm->query.ptr[0] - 48;
          if (i_lamp < DF_AR_LAMP_COUNT)
            {
              arLamp[i_lamp] = 0;
              MG_INFO (("lamp %d off", i_lamp));
              //tls_gpio_write (pin[i_lamp], DF_OFF);
            }
          mg_http_reply (c, 200, "", "%u", 321);
        }

      else
        {
          struct mg_http_serve_opts opts
              = { .root_dir = "/web_root", .fs = &mg_fs_packed };
          mg_http_serve_dir (c, ev_data, &opts);
        }
    }
  (void)fn_data;
}

static void
broadcast_message (struct mg_mgr *mgr, const char *message)
{
  struct mg_connection *c;
  for (c = mgr->conns; c != NULL; c = c->next)
    {
      if (c->data[0] == 'L')
        mg_http_printf_chunk (c, "%s", message);
    }
}

// Timer function - called periodically.
// Prepare log message. Save it to a file, and broadcast.
static void
timer_fn (void *arg)
{
  char buf[64];
  snprintf (buf, sizeof (buf), "= %lu\n", (unsigned long)mg_millis () / 1000);
  broadcast_message (arg, buf);
}


void
sock_s_task (void *sdata)
{

  printf ("sock_s_task start\n");

  struct mg_mgr mgr;
  mg_mgr_init (&mgr); // Init manager
  // mg_log_set (MG_LL_DEBUG); // Set debug log level. Default is MG_LL_INFO
  mg_log_set (MG_LL_INFO);
  mg_timer_add (&mgr, 1000, MG_TIMER_REPEAT, timer_fn, &mgr);

  for (;;) // цикл(1) с подсоединением к wifi и запросом времени
    {
      while (u8_wifi_state == 0)
        {
          printf ("trying to connect wifi\n");
          if (u8_wifi_state == 0
              && wifi_connect (MY_WIFI_AP, MY_WIFI_PASS) == WM_SUCCESS)
            {
              while (u8_wifi_state == 0)
                {
                  tls_os_time_delay (100);
                }
            }
          else
            {
              tls_os_time_delay (HZ * 5);
            }
        }

      tls_os_time_delay (HZ * 1);

      while (u8_wifi_state == 1) // основной цикл(2)
        {
          printf ("mg_http_listen start \n");
          mg_http_listen (&mgr, s_listening_url, fn, NULL); // Setup listener

          while (u8_wifi_state == 1)
            mg_mgr_poll (&mgr, 500); // Infinite event loop
        }
    }
  mg_mgr_free (&mgr);

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
