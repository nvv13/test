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

#include "lcd5643.h"

u16 i_mode_global = GL_MODE_WEATHER_CLOCK; //

static u8 i_5643_hour = 0;
static u8 i_5643_min = 0;
static u8 u8_sec_state = 0;
static u16 i_out = 0;

#define LCD_VAL_LG_spb_low 600
#define LCD_VAL_LG_low 200
#define LCD_VAL_LG_middle 50
#define LCD_VAL_LG_spb_hi 10
#define LCD_VAL_LG_hi 5
u16 i_max_out = LCD_VAL_LG_spb_hi; //

#define LCD_VAL_night LCD_VAL_LG_spb_hi // ночь, установить минимальную яркость! или другую

static int i_5643_t_sign = 0;
static int i_5643_t_value = 0;
static int i_5643_t_mantissa = 0;

static u8 iMode = MODE_WEATHER;

static void
demo_timer_irq (u8 *arg) // здесь будет вывод на LCD
{
  //        printf("timer irq hour=%d,min=%d\n",i_5643_hour,i_5643_min);
  u8 i_HiHour;
  u8 i_LoHour;
  u8 i_HiMin;
  u8 i_LoMin;

  if (iMode == MODE_CLOCK)
    {
      i_HiHour = i_5643_hour / 10;
      i_LoHour = i_5643_hour % 10;
      i_HiMin = i_5643_min / 10;
      i_LoMin = i_5643_min % 10;
    }

  // printf("timer irq hh:mm %d%d:%d%d \n",i_HiHour,i_LoHour,i_HiMin,i_LoMin);
  int i_t = i_5643_t_value;
  if (i_5643_t_mantissa > 5)
    i_t++;

  switch (i_out)
    {
    case 0:
      {
        if (iMode == MODE_CLOCK)
          lcd5643printDigit (OUT_DIG_1, i_HiHour, iMode);
        else
          {
            if (i_5643_t_value > 0)
              lcd5643printDigit (OUT_SIG_1, 1, iMode); // "+"
            else
              lcd5643printDigit (OUT_SIG_1, 0, iMode); // "-"
          }
      };
      break;
    case 1:
      {
        if (iMode == MODE_CLOCK)
          lcd5643printDigit (OUT_DIG_2, i_LoHour, iMode);
        else
          lcd5643printDigit (OUT_DIG_2, i_t / 10, iMode);
      };
      break;
    case 2:
      {
        if (iMode == MODE_CLOCK)
          lcd5643printDigit (OUT_DIG_3, i_HiMin, iMode);
        else
          lcd5643printDigit (OUT_DIG_3, i_t % 10, iMode);
      };
      break;
    case 3:
      {
        if (iMode == MODE_CLOCK)
          lcd5643printDigit (OUT_DIG_4, i_LoMin, iMode);
        else
          lcd5643printDigit (OUT_C_4, 0, iMode);
      };
      break;
    case 4:
      {
        if (iMode == MODE_CLOCK)
          lcd5643printDigit (OUT_SEC_IND, u8_sec_state, iMode); // on sec state
      };
      break;
    default:
      {
        lcd5643printDigit (
            5, 0, iMode); // off  пока i_out будет больше 4, - выключить LCD
      };
      break;
    }

  if (i_out++ > i_max_out) // от 5 ...
    {
      i_out = 0;
    }
}

u8 u8_wait_start_ota_upgrade;

// console task use UART0 as communication port with PC
void
demo_console_task (void *sdata)
{
  printf ("wifi test app\n");

  u8_wait_start_ota_upgrade = 0;
  u8 cnt_no_value = 0;
  u8 timer_id;
  struct tls_timer_cfg timer_cfg;

  // timer_cfg.unit = TLS_TIMER_UNIT_MS;
  // timer_cfg.timeout = 1;//4
  timer_cfg.unit
      = TLS_TIMER_UNIT_US; // чтобы небыло мерцания на минимальной яркости,
                           // пришлось сделать время таймера поменьше
  timer_cfg.timeout = 25;
  timer_cfg.is_repeat = 1;
  timer_cfg.callback = (tls_timer_irq_callback)demo_timer_irq;
  timer_cfg.arg = NULL;
  timer_id = tls_timer_create (&timer_cfg);
  tls_timer_start (timer_id);
  printf ("timer start\n");

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
        i_max_out = LCD_VAL_night; 
      else
        {
          flash_cfg_load_u16 (&i_max_out, MEM_CELL_FROM_LIGTH_LEVEL);
          printf ("flash_cfg_load_u16=%d\n", i_max_out);
          if (i_max_out < LCD_VAL_LG_hi || i_max_out > LCD_VAL_LG_spb_low)
            i_max_out = LCD_VAL_LG_hi;
          i_5643_hour = i_max_out / 100;
          i_5643_min = i_max_out % 100;
        }

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
              && i_max_out != LCD_VAL_night) // ночь, установить
                                                 // минимальную яркость!
            {
              i_max_out = LCD_VAL_night;
            }

          if (tblock.tm_hour == 6 && tblock.tm_min == 0 && tblock.tm_sec == 0
              && i_max_out == LCD_VAL_LG_spb_hi) // утро, восстановить яркость!
            {
              i_max_out = LCD_VAL_LG_hi;
            }

          if (i_mode_global != GL_MODE_CLOCK
              && (tblock.tm_min == 0
                  || (tblock.tm_min > 0 && tblock.tm_min % 2 == 0))
              && tblock.tm_sec == 0) //каждые 2 минуты
            {
              u8_wifi_state
                  = 0; // переход на цикл(1) wifi по новой и запрос температуы
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
              tls_timer_stop (timer_id);
              tls_watchdog_clr ();
              t_http_fwup (OTA_PATH_FILE);
              printf ("OTA upgrade stop, error\n"); //если в это место попало,
                                                    //значит какая-то ошибка
                                                    //случилась и прошивка не
                                                    //скачалась
              tls_timer_start (timer_id);
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

          //
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

  tls_gpio_cfg (WM_IO_PB_21, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLLOW); // 1
  tls_gpio_cfg (WM_IO_PB_22, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLLOW); // 2
  tls_gpio_cfg (WM_IO_PB_23, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLLOW); // 3
  tls_gpio_cfg (WM_IO_PB_24, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLLOW); // 4
  tls_gpio_cfg (WM_IO_PB_25, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLLOW); // 5
  tls_gpio_cfg (WM_IO_PB_26, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLLOW); // 6
  tls_gpio_cfg (WM_IO_PB_18, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLLOW); // 7
  tls_gpio_cfg (WM_IO_PB_17, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLLOW); // 8
  tls_gpio_cfg (WM_IO_PB_16, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLLOW); // 9
  tls_gpio_cfg (WM_IO_PB_11, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLLOW); // 10
  tls_gpio_cfg (WM_IO_PB_10, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLLOW); // 11
  tls_gpio_cfg (WM_IO_PB_15, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLLOW); // 12

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

  //    tls_os_time_delay(1000 * 10);
  //    tls_os_disp_task_stat_info();
  //	while(1)
  //	{
  //	}
}
