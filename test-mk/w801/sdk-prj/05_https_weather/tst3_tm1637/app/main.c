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
#include "wm_regs.h"
#include "wm_rtc.h"
#include "wm_timer.h"
#include "wm_watchdog.h"
//#include "wm_cpu.h"
//#include "csi_core.h"

//#include "../../../../../../w_wifi_pass.h"
//#define MY_WIFI_AP	"bred8"
//#define MY_WIFI_PASS    "123123123"
#define MY_WIFI_AP "bred1"
#define MY_WIFI_PASS "9115676369"

#include "my_recognize.h"
#include "w_flash_cfg.h"
#include "w_https.h"
#include "w_ntp.h"
#include "w_wifi.h"

#include "decode_cmd.h"

#define DEMO_TASK_SIZE 2048
static OS_STK DemoTaskStk[DEMO_TASK_SIZE];
#define DEMO_TASK_PRIO 32

#define DEMO_SOCK_S_TASK_SIZE 1024
static OS_STK sock_s_task_stk[DEMO_SOCK_S_TASK_SIZE];
#define DEMO_SOCK_S_PRIO (DEMO_TASK_PRIO + 1)

struct tm t_last_start_main_task;

#include "TM1637Display.h"

u16 i_mode_global = GL_MODE_WEATHER_CLOCK; //

static u8 i_5643_hour = 0;
static u8 i_5643_min = 0;
static u8 u8_sec_state = 0;

#define LCD_VAL_LG_spb_low 8 // 8-8=0
#define LCD_VAL_LG_low 6
#define LCD_VAL_LG_middle 4
#define LCD_VAL_LG_spb_hi 2
#define LCD_VAL_LG_hi 1 // 8-1=7
u16 i_max_out = LCD_VAL_LG_spb_hi; //

static int i_5643_t_sign = 0;
static int i_5643_t_value = 88;
static int i_5643_t_mantissa = 0;

static u8 iMode = MODE_WEATHER;

u8 u8_wait_start_ota_upgrade;

// console task use UART0 as communication port with PC
void
demo_console_task (void *sdata)
{
  printf ("wifi test app\n");

  u8_wait_start_ota_upgrade = 0;
  u8 cnt_no_value = 0;

  tls_watchdog_init (12 * 1000
                     * 1000); // u32 usec около 6 сек --около 1-2 минуты
  u8 i_start_reCheck = 0;
  u8 u8_wifi_state = 0;

  tls_get_rtc (&t_last_start_main_task);

  for (;;) // цикл(1) с подсоединением к wifi и запросом времени
    {
      while (u8_wifi_state == 0)
        {
          printf ("trying to connect wifi\n");
          if (u8_wifi_state == 0
              && demo_connect_net (MY_WIFI_AP, MY_WIFI_PASS) == WM_SUCCESS)
            u8_wifi_state = 1;
          else
            {
              tls_os_time_delay (5000);
            }
        }

      u8 u8_ntp_state = 0;
      while (u8_ntp_state == 0)
        {
          printf ("trying to get ntp\n");
          if (u8_ntp_state == 0 && ntp_demo () == WM_SUCCESS)
            u8_ntp_state = 1;
          else
            {
              tls_os_time_delay (5000);
            }
        }

      struct tm tblock;
      tls_get_rtc (&tblock);

      if (tblock.tm_hour >= 23
          || tblock.tm_hour < 6) // ночь, установить минимальную яркость!
        i_max_out = LCD_VAL_LG_spb_low;
      else
        {
          flash_cfg_load_u16 (&i_max_out, MEM_CELL_FROM_LIGTH_LEVEL);
          printf ("flash_cfg_load_u16=%d\n", i_max_out);
          if (i_max_out < LCD_VAL_LG_hi || i_max_out > LCD_VAL_LG_spb_low)
            i_max_out = LCD_VAL_LG_hi;
          i_5643_hour = i_max_out / 100;
          i_5643_min = i_max_out % 100;
        }
      setBrightness (i_max_out, 1);
      u16 i_max_out_last = i_max_out;
      uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
      setSegments (data, 4, 0);
      uint8_t PointData = 0x00;

      flash_cfg_load_u16 (&i_mode_global, MEM_CELL_FROM_GL_MODE);
      if (i_mode_global < GL_MODE_WEATHER || i_mode_global > GL_MODE_CLOCK)
        i_mode_global = GL_MODE_WEATHER_CLOCK;

      if (i_mode_global == GL_MODE_CLOCK)
        iMode = MODE_CLOCK;
      else
        iMode = MODE_WEATHER;
      https_demo ();

      while (u8_wifi_state == 1) // основной цикл(2)
        {
          tls_os_time_delay (300);
          tls_watchdog_clr ();
          tls_get_rtc (&tblock); // получаем текущее время
          // printf("
          // sec=%d,min=%d,hour=%d,mon=%d,year=%d\n",tblock.tm_sec,tblock.tm_min,tblock.tm_hour,tblock.tm_mon+1,tblock.tm_year+1900);
          u8_sec_state = ~u8_sec_state;

          if (i_max_out_last != i_max_out)
            {
              setBrightness (i_max_out, 1);
              i_max_out_last = i_max_out;
            }

          i_5643_hour = tblock.tm_hour;
          i_5643_min = tblock.tm_min;

          if (my_recognize_ret_cur_temperature () != MY_RECOGNIZE_NO_VALUE)
            {
              i_5643_t_sign = my_recognize_ret_cur_temperature_sign ();
              i_5643_t_value = my_recognize_ret_cur_temperature ();
              i_5643_t_mantissa = my_recognize_ret_cur_temperature_mantissa ();
            }

          // if((tblock.tm_min==0 || tblock.tm_min==10 || tblock.tm_min==20 ||
          // tblock.tm_min==30 || tblock.tm_min==40 || tblock.tm_min==50 ||
          //    tblock.tm_min==5 || tblock.tm_min==15 || tblock.tm_min==25 ||
          //    tblock.tm_min==35 || tblock.tm_min==45 || tblock.tm_min==55
          //   ) && tblock.tm_sec==0) // запросим снова ntp, - синхр время раз
          //   в сутки
          if((tblock.tm_hour==3 || i_start_reCheck<2 /* было, мигнул свет и... , вообщим добавим еще разок другой */ ) && 
        tblock.tm_min==0 && tblock.tm_sec==0) // запросим снова ntp, - синхр время раз в сутки
            {
              i_start_reCheck++;
              u8_wifi_state = 0; // переход на цикл(1)
              // tls_sys_reset(); так то, это не надо, вроде все стабильно
              // работает
            }

          if (tblock.tm_hour == 23 && tblock.tm_min == 0 && tblock.tm_sec == 0
              && i_max_out != LCD_VAL_LG_spb_low) // ночь, установить
                                                  // минимальную яркость!
            {
              i_max_out = LCD_VAL_LG_spb_low;
            }

          if (tblock.tm_hour == 6 && tblock.tm_min == 0 && tblock.tm_sec == 0
              && i_max_out
                     == LCD_VAL_LG_spb_low) // утро, восстановить яркость!
            {
              flash_cfg_load_u16 (&i_max_out, MEM_CELL_FROM_LIGTH_LEVEL);
              if (i_max_out < LCD_VAL_LG_hi || i_max_out > LCD_VAL_LG_spb_low)
                i_max_out = LCD_VAL_LG_middle;
            }

          if (i_mode_global != GL_MODE_CLOCK
              && (tblock.tm_min == 0
                  || (tblock.tm_min > 0 && tblock.tm_min % 2 == 0))
              && tblock.tm_sec == 0) //каждые 2 минуты
            {
              u8_wifi_state
                  = 0; // переход на цикл(1) wifi по новой и запрос температуры
              if (my_recognize_ret_cur_temperature () == MY_RECOGNIZE_NO_VALUE)
                {
                  cnt_no_value++;
                  if (cnt_no_value >= 3)
                    tls_sys_reset ();
                }
              else
                cnt_no_value = 0;
              struct tm t_last_query = my_recognize_ret_t_last_query ();
              printf ("cur_temperature=%d,%d  cnt_no_value=%d\n"
                      "last query=%d.%02d.%02d %02d:%02d:%02d\n"
                      "t_last_start_main_task=%d.%02d.%02d %02d:%02d:%02d\n",
                      my_recognize_ret_cur_temperature_sign ()
                          * my_recognize_ret_cur_temperature (),
                      my_recognize_ret_cur_temperature_mantissa (),
                      cnt_no_value, t_last_query.tm_year + 1900,
                      t_last_query.tm_mon + 1, t_last_query.tm_mday,
                      t_last_query.tm_hour, t_last_query.tm_min,
                      t_last_query.tm_sec,
                      t_last_start_main_task.tm_year + 1900,
                      t_last_start_main_task.tm_mon + 1,
                      t_last_start_main_task.tm_mday,
                      t_last_start_main_task.tm_hour,
                      t_last_start_main_task.tm_min,
                      t_last_start_main_task.tm_sec);
              printf ("    date %d.%02d.%02d %02d:%02d:%02d\n",
                      tblock.tm_year + 1900, tblock.tm_mon + 1, tblock.tm_mday,
                      tblock.tm_hour, tblock.tm_min, tblock.tm_sec);
              // tls_mem_alloc_info();
              tls_os_time_delay (1000 - 300);
            }

          if (u8_wait_start_ota_upgrade)
            {
              u8_wait_start_ota_upgrade = 0;
              printf ("OTA upgrade start, try = " OTA_PATH_FILE "\n");
              tls_watchdog_clr ();
              t_http_fwup (OTA_PATH_FILE);
              printf ("OTA upgrade stop, error\n"); //если в это место попало,
                                                    //значит какая-то ошибка
                                                    //случилась и прошивка не
                                                    //скачалась
            }

          if (i_mode_global == GL_MODE_CLOCK)
            iMode = MODE_CLOCK;
          else
            {
              if (i_mode_global == GL_MODE_WEATHER)
                iMode = MODE_WEATHER;
              else
                {
                  if (tblock.tm_sec % 10 == 0
                      && iMode != MODE_CLOCK) //показать время на 3 секунды,
                                              //каждые 10 сек
                    iMode = MODE_CLOCK;
                  if ((tblock.tm_sec + 7) % 10 == 0
                      && iMode != MODE_WEATHER) // через 3 сек, переключаем на
                                                // погоду
                    iMode = MODE_WEATHER;
                }
            }

          if (iMode == MODE_CLOCK)
            {
              if (u8_sec_state)
                PointData = VIEW_POINT_DATA;
              else
                PointData = 0x00;
              data[0] = encodeDigit (i_5643_hour / 10) + PointData;
              data[1] = encodeDigit (i_5643_hour % 10) + PointData;
              data[2] = encodeDigit (i_5643_min / 10) + PointData;
              data[3] = encodeDigit (i_5643_min % 10) + PointData;
              setSegments (data, 4, 0);
            }
          else
            {
              int i_t = i_5643_t_value;
              if (i_5643_t_mantissa > 5)
                i_t++;
              PointData = 0x00;
              data[0] = encodeSign ((i_5643_t_sign == 1 ? 0 : 1));
              data[1] = encodeDigit (i_t / 10);
              data[2] = encodeDigit (i_t % 10);
              data[3] = encodeDigit (0x0C);
              setSegments (data, 4, 0);
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

  // tls_sys_clk_set(CPU_CLK_240M);

  TM1637Display (WM_IO_PB_21, WM_IO_PB_22, DEFAULT_BIT_DELAY);

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
}
