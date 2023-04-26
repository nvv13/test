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
//#define MY_WIFI_AP "bred8"
//#define MY_WIFI_PASS "123123123"
#define MY_WIFI_AP "bred1"
#define MY_WIFI_PASS "9115676369"

#include "my_recognize.h"
#include "w_flash_cfg.h"
#include "w_ntp.h"
#include "w_wifi.h"

#include "decode_cmd.h"

#define DEMO_TASK_SIZE 2048
static OS_STK DemoTaskStk[DEMO_TASK_SIZE];
#define DEMO_TASK_PRIO 32

#define DEMO_SOCK_S_TASK_SIZE 1024
static OS_STK sock_s_task_stk[DEMO_SOCK_S_TASK_SIZE];
#define DEMO_SOCK_S_PRIO (DEMO_TASK_PRIO + 1)

#include "mod1/VS1053.h"
#include "mod1/u8g2.h"
#include "mod1/u8x8_riotos.h"

u8 u8_wait_start_ota_upgrade;

//****************************************************************************************************//
static u8g2_t u8g2;
static u8 u8_volume = 0; //
static char buf_str_ind[10];

#define KNOOB_SW WM_IO_PA_11
#define KNOOB_DT WM_IO_PA_12
#define KNOOB_CLK WM_IO_PA_13

volatile static u16 i_dreb_CLK = 0; // от дребезга
static const u16 i_pos_dreb_CLK = 3;
static const u16 i_pos_dreb_SW = 300; //кнопка
static int i_rotar = 10;
volatile static u16 i_rotar_zero = 0;
volatile static u16 i_rotar_one = 0;
static u8 i_rotar_value = 0;
volatile static u8 i_dreb_SW = 0; // от дребезга кнопки
static u8 u8_enc_state = 0;

static void
demo_timer_irq (u8 *arg) //
{

  if (i_dreb_CLK != 0)
    {
      if (i_dreb_CLK++ > i_pos_dreb_CLK) //можно отсчитывать временной интервал
        {
          i_dreb_CLK = 0; // от дребезга

          u8_enc_state = ~u8_enc_state; // у меня "Полношаговый" энкодер, даёт
                                        // 4 сигнала на один щелчок, поэтому
                                        // исп. переменная u8_enc_state
          if (u8_enc_state)
            {

              if (i_rotar_zero > i_rotar_one)
                i_rotar--;
              else
                i_rotar++;

              if (i_rotar < 0)
                i_rotar = 0;
              if (i_rotar > 100)
                i_rotar = 100;

              u8_volume = 100 - i_rotar;
              VS1053_setVolume (u8_volume);
              u8g2_FirstPage (&u8g2);
              do
                {
                u8g2_SetDrawColor (&u8g2, 1);
                u8g2_SetFont (&u8g2, u8g2_font_courB24_tf);
                sprintf (buf_str_ind, "%.3d", u8_volume);
                u8g2_DrawStr (&u8g2, 10, 24, buf_str_ind);
                u8g2_SetFont (&u8g2, u8g2_font_courB08_tf);
                u8g2_DrawStr (&u8g2, 10, 50, my_recognize_ret_name());
                }
              while (u8g2_NextPage (&u8g2));
            }
          i_rotar_zero = 0;
          i_rotar_one = 0;
          i_rotar_value = 1;
        }
    }

  if (i_dreb_SW != 0
      && i_dreb_SW++ > i_pos_dreb_SW) //можно отсчитывать временной интервал
    {
      i_dreb_SW = 0; // от дребезга
    }
}

static void
KNOOB_SW_isr_callback (void *context)
{
  u16 ret = tls_get_gpio_irq_status (KNOOB_SW);
  if (ret)
    {
      tls_clr_gpio_irq_status (KNOOB_SW);
      if (i_dreb_SW == 0) // защита от ддребезга контактов для кнопки
        {
          i_dreb_SW = 1;
          VS1053_stop_PlayMP3 ();
        }
    }
}
static void
KNOOB_CLK_isr_callback (void *context)
{
  u16 ret = tls_get_gpio_irq_status (KNOOB_CLK);
  if (ret)
    {
      tls_clr_gpio_irq_status (KNOOB_CLK);
      if (i_dreb_CLK == 0)
        {
          if (tls_gpio_read (KNOOB_DT))
            i_rotar_one++;
          else
            i_rotar_zero++;
          i_dreb_CLK = 1;
        }
    }
}

//****************************************************************************************************//

// console task use UART0 as communication port with PC
void
demo_console_task (void *sdata)
{

  printf ("wifi test app\n");

  u8_wait_start_ota_upgrade = 0;

  tls_watchdog_init (30 * 1000
                     * 1000); // u32 usec около 6 сек --около 1-2 минуты
  u8 u8_wifi_state = 0;

  puts ("Initializing to I2C oled Display.");

  // u8g2_Setup_sh1106_i2c_128x64_noname_f (&u8g2, U8G2_R0,
  // u8x8_byte_hw_i2c_riotos, u8x8_gpio_and_delay_riotos);

  // попадаються дисплеи с неправильным i2c, не дающие ask, тогда используем
  // u8x8_byte_hw_i2c_no_ask_riotos
  u8g2_Setup_ssd1309_i2c_128x64_noname0_f (&u8g2, U8G2_R0,
                                           u8x8_byte_hw_i2c_no_ask_riotos,
                                           u8x8_gpio_and_delay_riotos);

  if (u8g2.u8x8.i2c_address == 255) // заменяем default на настоящий адрес
    u8g2.u8x8.i2c_address = 0x3C;
  u8x8_riotos_t user_data_8x8 = {
    .pin_cs = GPIO_UNDEF,    //
    .pin_dc = GPIO_UNDEF,    //
    .pin_reset = GPIO_UNDEF, //

    .i2c_scl = WM_IO_PA_01,
    .i2c_sda = WM_IO_PA_04,
    .i2c_freq = 100000 // частота i2c в герцах

  };
  u8g2_SetUserPtr (&u8g2, &user_data_8x8);
  puts ("Initializing display.");
  u8g2_InitDisplay (&u8g2);
  u8g2_SetPowerSave (&u8g2, 0);

  /* vs1053 */
  libVS1053_t user_data53 = {

    .rst_pin = WM_IO_PB_17,  /* HW reset pin */
    .cs_pin = WM_IO_PB_21,   /* ->xcs  SCI Chip Select pin */
    .dcs_pin = WM_IO_PB_22,  /* ->xdcs SDI Chip Select pin */
    .dreq_pin = WM_IO_PB_18, /* <-dreq Data Request pin */

    .spi_cs = WM_IO_PB_23, /* not wire */
    .spi_ck = WM_IO_PB_24, /*      ck -> sck Clock pin */
    .spi_di = WM_IO_PB_03, /* master miso di <- miso slave, на макетке board
                              HLK-W801-KIT-V1.1 работает только WM_IO_PB_03  */
    .spi_do = WM_IO_PB_26, /* master mosi do -> mosi slave */
  };

  /**
   * config the pins used for spi di
   * WM_IO_PB_00 - не работает,
   * WM_IO_PB_03 - работает!
   * WM_IO_PB_16 only for 56pin - не работает, мешает светодиод подключенный к
   * данному контакту на макетке
   * WM_IO_PB_25 only for 56pin - не работает, мешает светодиод подключенный к
   * данному контакту на макетке
   */

  VS1053_VS1053 (&user_data53);
  VS1053_begin ();

  u8 timer_id;
  struct tls_timer_cfg timer_cfg;
  // timer_cfg.unit = TLS_TIMER_UNIT_MS;
  timer_cfg.unit = TLS_TIMER_UNIT_US;
  timer_cfg.timeout = 100;
  timer_cfg.is_repeat = 1;
  timer_cfg.callback = (tls_timer_irq_callback)demo_timer_irq;
  timer_cfg.arg = NULL;
  timer_id = tls_timer_create (&timer_cfg);
  tls_timer_start (timer_id);
  printf ("timer start\n");
  //
  tls_gpio_cfg (KNOOB_SW, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);
  tls_gpio_isr_register (KNOOB_SW, KNOOB_SW_isr_callback, NULL);
  tls_gpio_irq_enable (KNOOB_SW, WM_GPIO_IRQ_TRIG_FALLING_EDGE);
  //
  tls_gpio_cfg (KNOOB_CLK, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);
  tls_gpio_isr_register (KNOOB_CLK, KNOOB_CLK_isr_callback, NULL);
  tls_gpio_irq_enable (KNOOB_CLK, WM_GPIO_IRQ_TRIG_FALLING_EDGE);
  //
  tls_gpio_cfg (KNOOB_DT, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);
  //

  u8_volume = 100;
  i_rotar = 100 - u8_volume;
  VS1053_setVolume (u8_volume);
  u8g2_FirstPage (&u8g2);
  do
    {
      u8g2_SetDrawColor (&u8g2, 1);
      u8g2_SetFont (&u8g2, u8g2_font_courB24_tf);
      sprintf (buf_str_ind, "%.3d", u8_volume);
      u8g2_DrawStr (&u8g2, 10, 24, buf_str_ind);
    }
  while (u8g2_NextPage (&u8g2));

  // char buff[100];

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

      tls_watchdog_clr ();

      //          http_get_demo
      //          ("https://de1.api.radio-browser.info/json/stations/"
      //                         "bycodec/mp3?limit=1&order=random");
      //          my_recognize_http_reset ();

      while (u8_wifi_state == 1) // основной цикл(2)
        {
          my_recognize_http_reset ();
          http_get_demo (
              "http://all.api.radio-browser.info/json/stations/bycodec/"
              "mp3?limit=1&order=random");
          u8g2_FirstPage (&u8g2);
          do
            {
              u8g2_SetDrawColor (&u8g2, 1);
              u8g2_SetFont (&u8g2, u8g2_font_courB24_tf);
              sprintf (buf_str_ind, "%.3d", u8_volume);
              u8g2_DrawStr (&u8g2, 10, 24, buf_str_ind);
              u8g2_SetFont (&u8g2, u8g2_font_courB08_tf);
              u8g2_DrawStr (&u8g2, 10, 50, my_recognize_ret_name());
            }
          while (u8g2_NextPage (&u8g2));

          VS1053_PlayHttpMp3 (my_recognize_ret_url_resolved ());

          tls_os_time_delay (HZ*5);
          tls_watchdog_clr ();
          tls_get_rtc (&tblock); // получаем текущее время
          // printf("
          // sec=%d,min=%d,hour=%d,mon=%d,year=%d\n",tblock.tm_sec,tblock.tm_min,tblock.tm_hour,tblock.tm_mon+1,tblock.tm_year+1900);

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

          /*
                  u8g2_FirstPage (&u8g2);
                   do
                     {
                       u8g2_SetDrawColor (&u8g2, 1);
                       u8g2_SetFont (&u8g2, u8g2_font_courB24_tf);
                       sprintf (buf_str_ind, "%.3d", u8_volume);
                       u8g2_DrawStr (&u8g2, 10, 24, buf_str_ind);
                     }
                   while (u8g2_NextPage (&u8g2));
         */
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
