/*****************************************************************************
 *
 * File Name : main.c
 *
 * Description: main
 *
 * Date : 2023-11-28
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
      if (mg_http_match_uri (hm, "/api/log/static"))
        {
          struct mg_http_serve_opts opts
              = { .root_dir = "/web_root", .fs = &mg_fs_packed };
          mg_http_serve_file (c, hm, "/web_root/log.txt", &opts);
        }
      else if (mg_http_match_uri (hm, "/api/log/live"))
        {
          c->data[0] = 'L'; // Mark that connection as live log listener
          mg_printf (c,
                     "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
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
  snprintf (buf, sizeof (buf), "mg_millis is: %lu\n", (unsigned long)mg_millis());
  broadcast_message (arg, buf);
}

void
user_app1_task (void *sdata)
{

  printf ("user_app1_task start \n");

  struct mg_mgr mgr;
  mg_mgr_init (&mgr); // Init manager
  //mg_log_set (MG_LL_DEBUG); // Set debug log level. Default is MG_LL_INFO
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
          mg_http_listen (&mgr, "http://0.0.0.0:80", fn,
                          NULL); // Setup listener

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
