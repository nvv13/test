/*****************************************************************************
 *
 * File Name : main.c
 *
 * Description: main
 *
 * Date : 2023-11-02
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "wm_type_def.h"
//#include "wm_cpu.h"
//#include "wm_watchdog.h"
//#include "wm_uart.h"
//#include "wm_gpio.h"
//#include "wm_hostspi.h"
//#include "wm_socket.h"
//#include "wm_sockets.h"
//#include "wm_wifi.h"
//#include "wm_hspi.h"
//#include "wm_pwm.h"
//#include "wm_params.h"
//#include "wm_osal.h"
//#include "wm_timer.h"
//#include "wm_netif.h"
//#include "wm_efuse.h"
//#include "wm_mem.h"
//#include "wm_regs.h"
//#include "wm_rtc.h"
//#include "wm_uart.h"
//#include "ff.h"
//#include "utils.h"
//#include "wm_gpio_afsel.h"
//#include "my_recognize.h"
//#include "w_flash_cfg.h"
//#include "w_ntp.h"

#include "w_wifi.h"

#include "../../../../../../../w_wifi_pass.h"
//#define MY_WIFI_AP "bred8"
//#define MY_WIFI_PASS "123123123"
//#define MY_WIFI_AP "bred1"
//#define MY_WIFI_PASS "9115676369"

u8 volatile u8_wifi_state = 0;

#define USER_APP1_TASK_SIZE 2048
static OS_STK UserApp1TaskStk[USER_APP1_TASK_SIZE];
#define USER_APP1_TASK_PRIO 32

#include "mongoose/mongoose.h"
// Rest of your code ...

// Mongoose event handler function, gets called by the mg_mgr_poll()
static void
fn (struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
  if (ev == MG_EV_HTTP_MSG)
    {
      // The MG_EV_HTTP_MSG event means HTTP request. `hm` holds parsed
      // request, see https://mongoose.ws/documentation/#struct-mg_http_message
      struct mg_http_message *hm = (struct mg_http_message *)ev_data;

      // If the requested URI is "/api/hi", send a simple JSON response back
      if (mg_http_match_uri (hm, "/api/hi"))
        {
          // Use mg_http_reply() API function to generate JSON response. It
          // adds a Content-Length header automatically. In the response, we
          // show the requested URI and HTTP body:
          mg_http_reply (c, 200, "", "{%m:%m,%m:%m}\n", // See mg_snprintf doc
                         MG_ESC ("uri"), mg_print_esc, hm->uri.len,
                         hm->uri.ptr, MG_ESC ("body"), mg_print_esc,
                         hm->body.len, hm->body.ptr);
        }
      else
        {
          // For all other URIs, serve static content from the current
          // directory
          // struct mg_http_serve_opts opts = { .root_dir = "." };
          struct mg_http_serve_opts opts = {
            .root_dir = "0:web_root",
            .fs = &mg_fs_fat
           };

          mg_http_serve_dir (c, hm, &opts);
        }
    }
}

void
user_app1_task (void *sdata)
{

  printf ("user_app1_task start \n");

  struct mg_mgr mgr;
  mg_mgr_init (&mgr);       // Init manager
  mg_log_set (MG_LL_DEBUG); // Set debug log level. Default is MG_LL_INFO

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
          mg_http_listen (&mgr, "http://0.0.0.0:8000", fn,
                          NULL); // Setup listener

          while (u8_wifi_state == 1)
            mg_mgr_poll (&mgr, 1000); // Infinite event loop
        }
    }
  mg_mgr_free (&mgr);
}

void
UserMain (void)
{
  printf ("UserMain start \n");
  tls_sys_clk_set (CPU_CLK_240M);

  tls_os_task_create (NULL, NULL, user_app1_task, NULL,
                      (void *)UserApp1TaskStk, /* task's stack start address */
                      USER_APP1_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      USER_APP1_TASK_PRIO, 0);
}
