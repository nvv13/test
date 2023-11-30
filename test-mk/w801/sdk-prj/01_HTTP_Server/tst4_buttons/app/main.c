/*

на основе
Mongoose networking library
https://mongoose.ws/documentation/

исходники Mongoose
https://github.com/cesanta/mongoose.git

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wm_gpio.h"

#include "w_wifi.h"

#include "../../../../../../../w_wifi_pass.h"
//#define MY_WIFI_AP "bred8"
//#define MY_WIFI_PASS "123123123"
//#define MY_WIFI_AP "bred1"
//#define MY_WIFI_PASS "9115676369"

u8 volatile u8_wifi_state = 0;

#define USER_APP1_TASK_SIZE 4096
static OS_STK UserApp1TaskStk[USER_APP1_TASK_SIZE];
#define USER_APP1_TASK_PRIO 32

#include "mongoose/mongoose.h"
// Rest of your code ...

const char *s_listening_url = "http://0.0.0.0:80";

#define DF_AR_LAMP_COUNT 3
static unsigned char arLamp[DF_AR_LAMP_COUNT] = { 0, 0, 0 };
static unsigned char pin[DF_AR_LAMP_COUNT]
    = { WM_IO_PB_11, WM_IO_PB_16, WM_IO_PB_25 };
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
              tls_gpio_write (pin[i_lamp], DF_ON);
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
              tls_gpio_write (pin[i_lamp], DF_OFF);
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
user_app1_task (void *sdata)
{

  printf ("user_app1_task start тест кнопок\n");

  tls_gpio_cfg (pin[0], WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);
  tls_gpio_cfg (pin[1], WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);
  tls_gpio_cfg (pin[2], WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);

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
  printf ("UserMain start \n");
  tls_sys_clk_set (CPU_CLK_240M);

  tls_os_task_create (NULL, "user_app1_task", user_app1_task, NULL,
                      (void *)UserApp1TaskStk, /* task's stack start address */
                      USER_APP1_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      USER_APP1_TASK_PRIO, 0);
}
