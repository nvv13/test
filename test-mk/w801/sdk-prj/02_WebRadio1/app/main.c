/*****************************************************************************
 *
 * File Name : main.c
 *
 * Description: main
 *
 * Date : 2023-04-25
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wm_type_def.h"

#include "wm_cpu.h"
#include "wm_gpio.h"
#include "wm_osal.h"
#include "wm_regs.h"
#include "wm_timer.h"
#include "wm_watchdog.h"

//#include "../../../../../../w_wifi_pass.h"
//#define MY_WIFI_AP "bred8"
//#define MY_WIFI_PASS "123123123"
#define MY_WIFI_AP "bred1"
#define MY_WIFI_PASS "9115676369"

#include "my_recognize.h"
#include "w_flash_cfg.h"
#include "w_wifi.h"

#define DEMO_TASK_SIZE 2048
static OS_STK DemoTaskStk[DEMO_TASK_SIZE];
#define DEMO_TASK_PRIO 32

#include "mod1/VS1053.h"
#include "mod1/u8g2.h"
#include "mod1/u8x8_riotos.h"

//****************************************************************************************************//
u8 u8_wifi_state = 0;
extern u32 VS1053_WEB_RADIO_nTotal;

static u8g2_t u8g2;

static u8 i_switch_menu = 0;
static int i_menu = 0;
static u8 u8_ind_ch_st = 0;

static u16 u16_volume = 0; //
static char buf_str_ind[50];
static char stantion_uuid[39];
static int i_find_stantion_id = -1;
static char stantion_name_temp[50];
static char stantion_uuid_temp[39];

static void display_refresh (void);

#define MENU_STORE_VOLUME 0
#define MENU_STORE_INDEX 1
#define MENU_MAX_POS (USER_CNT_REC_STANTION_NAME * 2)

void
MenuActionClick (void)
{
  if (i_menu == MENU_STORE_VOLUME)
    flash_cfg_store_u16 (u16_volume, MENU_STORE_VOLUME);
  else
    {
      u8 u8_stantion_id = (i_menu / 2);
      if ((u8_stantion_id * 2) == i_menu && u8_stantion_id > 0)
        u8_stantion_id--;
      //
      // printf ("MenuActionClick i_menu %d,i_menu2 %d,u8_stantion_id
      // %d\n",i_menu,i_menu % 2,u8_stantion_id);
      if (i_menu % 2 != 0)
        {
          flash_cfg_load_stantion_uuid (stantion_uuid, u8_stantion_id);
          // printf ("flash_cfg_load stantion_uuid %s,u8_stantion_id
          // %d\n",stantion_uuid,u8_stantion_id);
          VS1053_stop_PlayMP3 ();
        }
      else
        {
          i_find_stantion_id = flash_cfg_find_stantion_id_by_uuid (
              my_recognize_ret_stationuuid (u8_ind_ch_st));
          if (i_find_stantion_id == -1 || i_find_stantion_id == u8_stantion_id)
            {
              flash_cfg_store_stantion_name (
                  my_recognize_ret_name (u8_ind_ch_st), u8_stantion_id);
              flash_cfg_store_stantion_uuid (
                  my_recognize_ret_stationuuid (u8_ind_ch_st), u8_stantion_id);
            }
          else
            { //Если нашло в другом месте, то обмен позиций
              flash_cfg_load_stantion_uuid (stantion_uuid_temp,
                                            u8_stantion_id);
              flash_cfg_load_stantion_name (stantion_name_temp,
                                            u8_stantion_id);

              flash_cfg_store_stantion_name (stantion_name_temp,
                                             i_find_stantion_id);
              flash_cfg_store_stantion_uuid (stantion_uuid_temp,
                                             i_find_stantion_id);

              flash_cfg_store_stantion_name (
                  my_recognize_ret_name (u8_ind_ch_st), u8_stantion_id);
              flash_cfg_store_stantion_uuid (
                  my_recognize_ret_stationuuid (u8_ind_ch_st), u8_stantion_id);
            }
          // printf (
          //    "i_find_stantion_id=%d, u8_ind_ch_st=%d, u8_stantion_id=%d\n",
          //    i_find_stantion_id, u8_ind_ch_st, u8_stantion_id);
          display_refresh ();
        }
    }
}

static void
display_menu_stantion_pos (u8 u8_stantion_id, u8 u8_stpos)
{
  u8 u8_Page_Disp = u8_stantion_id / 4; //по 4 станции влезает на дисплей

  bool l_name_choice = ((u8_Page_Disp * 4 + u8_stpos) == u8_stantion_id
                        && i_menu % 2 != 0 && i_menu != 0);
  bool l_store_choice = ((u8_Page_Disp * 4 + u8_stpos) == u8_stantion_id
                         && i_menu % 2 == 0 && i_menu != 0);

  if (l_name_choice)
    u8g2_SetFont (&u8g2, u8g2_font_6x12_t_cyrillic);
  else
    u8g2_SetFont (&u8g2, u8g2_font_4x6_t_cyrillic);
  sprintf (buf_str_ind, "%.2d.", (u8_Page_Disp * 4 + u8_stpos));
  u8g2_DrawStr (&u8g2, 1, 30 + u8_stpos * 10, buf_str_ind);
  flash_cfg_load_stantion_name (buf_str_ind, (u8_Page_Disp * 4 + u8_stpos));
  if (buf_str_ind[0] == 0xd0)
    buf_str_ind[(l_store_choice ? 24 : 28)] = 0;
  else
    buf_str_ind[(l_store_choice ? 12 : 14)] = 0;
  u8g2_DrawUTF8 (&u8g2, 16, 30 + u8_stpos * 10, buf_str_ind);

  if (!l_name_choice)
    {
      if (l_store_choice)
        {
          u8g2_SetFont (&u8g2, u8g2_font_6x12_t_cyrillic);
          sprintf (buf_str_ind, "Store%.2d", (u8_Page_Disp * 4 + u8_stpos));
          u8g2_DrawStr (&u8g2, 80, 30 + u8_stpos * 10, buf_str_ind);
        }
      else
        {
          u8g2_SetFont (&u8g2, u8g2_font_4x6_t_cyrillic);
          sprintf (buf_str_ind, "St%.2d", (u8_Page_Disp * 4 + u8_stpos));
          u8g2_DrawStr (&u8g2, 100, 30 + u8_stpos * 10, buf_str_ind);
        }
    }
}

volatile static u16 i_delay_WAIT
    = 0; //если не 0, значит ждем данных с сервера веб станции

static void
display_refresh (void)
{
  u8g2_FirstPage (&u8g2);
  do
    {
      u8g2_SetDrawColor (&u8g2, 1);
      if (i_switch_menu == 0)
        {
          u8g2_SetFont (&u8g2, u8g2_font_courB18_tf);
          sprintf (buf_str_ind, "vol:%.3d", u16_volume);
          u8g2_DrawStr (&u8g2, 10, 20, buf_str_ind);
          u8g2_SetFont (&u8g2, u8g2_font_5x7_t_cyrillic);
          u8g2_DrawUTF8 (&u8g2, 1, 30, my_recognize_ret_name (u8_ind_ch_st));
          u8g2_DrawUTF8 (&u8g2, 1, 40, my_recognize_ret_tags (u8_ind_ch_st));
          u8g2_DrawStr (&u8g2, 1, 50, my_recognize_ret_country (u8_ind_ch_st));
          u8g2_DrawStr (&u8g2, 1, 60, my_recognize_ret_codec (u8_ind_ch_st));
          u8g2_DrawStr (&u8g2, 60, 60,
                        my_recognize_ret_bitrate (u8_ind_ch_st));
          u8g2_DrawStr (
              &u8g2, 90, 60,
              (my_recognize_ret_https (u8_ind_ch_st) ? "https" : "http"));
        }
      else
        {
          u8g2_SetFont (&u8g2, u8g2_font_courB18_tf);
          sprintf (buf_str_ind, "Menu");
          u8g2_DrawStr (&u8g2, 0, 20, buf_str_ind);
          //

          u8g2_SetFont (&u8g2, u8g2_font_5x8_t_cyrillic);
          sprintf (buf_str_ind, "%d", i_menu);
          u8g2_DrawStr (&u8g2, 65, 10, buf_str_ind);

          if (i_menu == MENU_STORE_VOLUME)
            u8g2_SetFont (&u8g2, u8g2_font_6x12_t_cyrillic);
          else
            u8g2_SetFont (&u8g2, u8g2_font_5x7_t_cyrillic);
          u8g2_DrawStr (&u8g2, 65, 20, "Store Vol");

          u8 u8_stantion_id = (i_menu / 2);
          if ((u8_stantion_id * 2) == i_menu && u8_stantion_id > 0)
            u8_stantion_id--;
          display_menu_stantion_pos (u8_stantion_id, 0);
          display_menu_stantion_pos (u8_stantion_id, 1);
          display_menu_stantion_pos (u8_stantion_id, 2);
          display_menu_stantion_pos (u8_stantion_id, 3);

          if (i_find_stantion_id >= 0)
            {
              u8g2_SetFont (&u8g2, u8g2_font_courB18_tf);
              sprintf (buf_str_ind, "St%.2d.OV(%.2d)", u8_stantion_id,
                       i_find_stantion_id);
              u8g2_DrawStr (&u8g2, 0, 45, buf_str_ind);
              i_find_stantion_id = -1;
              i_delay_WAIT = 1;
            }
        }

      if (VS1053_status_get_status () != VS1053_PLAY
          || VS1053_WEB_RADIO_nTotal < 512)
        {
          u8g2_SetDrawColor (&u8g2, 1);
          u8g2_SetFont (&u8g2, u8g2_font_courB18_tf);
          u8g2_DrawStr (&u8g2, 30, 45, "WAIT...");
        }
    }
  while (u8g2_NextPage (&u8g2));
}

//****************************************************************************************************//

#define KNOOB_SW WM_IO_PA_11
#define KNOOB_CLK WM_IO_PA_12
#define KNOOB_DT WM_IO_PA_13

static const u16 i_pos_dreb_CLK = 1; // таймер 300 Мкс, значит будет 300 MKs
volatile static u16 i_dreb_CLK = 0; // от дребезга

static int i_rotar = 10;
volatile static u16 i_rotar_zero = 0;
volatile static u16 i_rotar_one = 0;
static u8 u8_enc_state = 0;

static const u16 i_pos_dreb_SW
    = 2000; //кнопка,таймер 300 Мкс, значит будет 600 миллисекунд.
volatile static u8 i_dreb_SW = 0; // от дребезга кнопки

static const u16 i_pos_DBL_CLICK
    = 6000; // таймер 300 Мкс, значит будет 1.8 сек
volatile static u16 i_delay_SW_DBL_CLICK
    = 0; // двойной клик, переход в меню и обратно

static const u16 i_pos_delay_volume
    = 6000; // таймер 300 Мкс, значит будет 0.9 сек
volatile static u16 i_delay_volume = 0;

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

              if (i_switch_menu == 0)
                {
                  if (i_rotar_zero > i_rotar_one)
                    i_rotar--;
                  else
                    i_rotar++;

                  if (i_rotar < 0)
                    i_rotar = 0;
                  if (i_rotar > 100)
                    i_rotar = 100;

                  u16_volume = 100 - i_rotar;
                  if (i_delay_volume == 0)
                    {
                      i_delay_volume = 1;
                      VS1053_setVolume (u16_volume);
                    }
                }
              else
                {
                  if (i_rotar_zero > i_rotar_one)
                    i_menu--;
                  else
                    i_menu++;
                  if (i_menu < 0)
                    i_menu = MENU_MAX_POS;
                  if (i_menu > MENU_MAX_POS)
                    i_menu = 0;
                }

              display_refresh ();
            }
          i_rotar_zero = 0;
          i_rotar_one = 0;
        }
    }

  if (i_dreb_SW != 0
      && i_dreb_SW++ > i_pos_dreb_SW) //можно отсчитывать временной интервал
    {
      i_dreb_SW = 0; // от дребезга
    }

  if (i_delay_SW_DBL_CLICK
      > 0) //если время двойного нажатия просрочено (1 сек), то действие в
           //зависимости от , в меню мы или нет
    {
      if (i_delay_SW_DBL_CLICK++ > i_pos_DBL_CLICK)
        {
          i_delay_SW_DBL_CLICK = 0;
          if (i_switch_menu == 0)
            {
              // printf ("i_switch_menu == 0\n");
              stantion_uuid[0] = 0;
              VS1053_stop_PlayMP3 ();
            }
          else
            MenuActionClick ();
        }
    }

  if (i_delay_volume > 0) // регулятор громкости, защитим от зависаний, слишком
                          // быстро - нельзя
    {
      if (i_delay_volume++ > i_pos_delay_volume)
        {
          i_delay_volume = 0;
          VS1053_setVolume (u16_volume);
        }
    }

  if (i_delay_WAIT > 0) // ждем (1 сек) для того чтоб убрать надпись "WAIT..."
                        // на дисплее, если уже воспроизведение
    {
      if (i_delay_WAIT++ > i_pos_DBL_CLICK)
        {
          if (VS1053_status_get_status () != VS1053_PLAY
              || VS1053_WEB_RADIO_nTotal < 512)
            i_delay_WAIT = 1;
          else
            {
              i_delay_WAIT = 0;
              display_refresh ();
            }
        }
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
          if (i_delay_SW_DBL_CLICK == 0)
            i_delay_SW_DBL_CLICK = 1;
          else
            {
              i_delay_SW_DBL_CLICK = 0;
              i_switch_menu = ~i_switch_menu;
              // i_menu = 0;
              /*
              if (i_switch_menu == 0)
                {
                  u16 menu_tmp;
                  flash_cfg_load_u16 (&menu_tmp, MENU_STORE_INDEX);
                  if (menu_tmp != i_menu)
                    flash_cfg_store_u16 (i_menu, MENU_STORE_INDEX);
                }
              */
              if (i_switch_menu != 0 && i_menu > 1 && i_menu % 2 == 0)
                i_menu--;
              display_refresh ();
            }
        }
    }
}
static void
KNOOB_CLK_isr_callback (void *context)
{
  u16 retC = tls_get_gpio_irq_status (KNOOB_CLK);
  //u16 retD = tls_get_gpio_irq_status (KNOOB_DT);
  if (retC)// || retD)
    {
      if (retC)
        tls_clr_gpio_irq_status (KNOOB_CLK);
      //if (retD)
      //  tls_clr_gpio_irq_status (KNOOB_DT);

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
/*
constrain(x, a, b)
Функция проверяет и если надо задает новое значение,
так чтобы оно была в области допустимых значений, заданной параметрами.

Параметры
x: проверяемое значение, любой тип
a: нижняя граница области допустимых значений, любой тип
b: верхняя граница области допустимых значений, любой тип
*/
static int
constrain (int x, int a, int b)
{
  if (x < a)
    return a;
  if (x > b)
    return b;
  return x;
}
static int
random (int min_num, int max_num)
{
  int result = 0, low_num = 0, hi_num = 0;

  if (min_num < max_num)
    {
      low_num = min_num;
      hi_num = max_num + 1; // include max_num in output
    }
  else
    {
      low_num = max_num;
      hi_num = min_num + 1; // include max_num in output
    }

  result = (rand () % (hi_num - low_num)) + low_num;
  result = constrain (result, low_num, hi_num - 1);
  return result;
}

// console task use UART0 as communication port with PC
void
demo_console_task (void *sdata)
{

  printf ("wifi test app\n");

  tls_watchdog_init (20 * 1000 * 1000); // u32 usec microseconds, около 20 сек
  srand (tls_os_get_time ());           // time(NULL));

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
    .i2c_freq = 150000 // частота i2c в герцах

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

    .spi_cs = WM_IO_PB_23, /* not wire, но назначить надо */
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
  // timer_cfg.unit = TLS_TIMER_UNIT_MS; // MS или Миллисекунды = 10^-3
  timer_cfg.unit = TLS_TIMER_UNIT_US; // US или Микросекунды = 10^-6
  timer_cfg.timeout = 300; // 100 US, значит частота 10KHz
  timer_cfg.is_repeat = 1;
  timer_cfg.callback = (tls_timer_irq_callback)demo_timer_irq;
  timer_cfg.arg = NULL;
  timer_id = tls_timer_create (&timer_cfg);
  tls_timer_start (timer_id);
  printf ("timer start\n");
  //
  tls_gpio_cfg (KNOOB_SW, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);
  tls_gpio_isr_register (KNOOB_SW, KNOOB_SW_isr_callback, NULL);
  tls_gpio_irq_enable (KNOOB_SW, WM_GPIO_IRQ_TRIG_RISING_EDGE);
  //
  tls_gpio_cfg (KNOOB_CLK, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);
  tls_gpio_isr_register (KNOOB_CLK, KNOOB_CLK_isr_callback, NULL);
  tls_gpio_irq_enable (KNOOB_CLK, WM_GPIO_IRQ_TRIG_RISING_EDGE);
  //
  tls_gpio_cfg (KNOOB_DT, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);
  //tls_gpio_isr_register (KNOOB_DT, KNOOB_CLK_isr_callback, NULL);
  //tls_gpio_irq_enable (KNOOB_DT, WM_GPIO_IRQ_TRIG_RISING_EDGE);
  //

  stantion_uuid[0] = 0;
  flash_cfg_load_u16 (&u16_volume, MENU_STORE_VOLUME);
  u16 i_menu_temp;
  flash_cfg_load_u16 (&i_menu_temp, MENU_STORE_INDEX);
  i_menu = i_menu_temp;
  if (i_menu > MENU_MAX_POS)
    i_menu = 0;
  if (u16_volume > 100) //после обновления прошивки?
    u16_volume = 80;
  i_rotar = 100 - u16_volume;
  VS1053_setVolume (u16_volume);
  display_refresh ();

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
              tls_os_time_delay (HZ * 5);
            }
        }

      tls_watchdog_clr ();

      tls_os_time_delay (HZ);
      my_recognize_http_reset ();
      printf ("load default stantion 0\n");
      flash_cfg_load_stantion_uuid (stantion_uuid, 0);

      while (u8_wifi_state == 1) // основной цикл(2)
        {
          my_recognize_http_reset ();
          display_refresh ();

          if (strlen (stantion_uuid) == 36)
            {
              http_get_web_station_by_stationuuid (stantion_uuid);
              stantion_uuid[0] = 0;
              u8_ind_ch_st = 0;
            }
          else
            {
              http_get_web_station_by_random ();
              for (u8 ind = 0; ind < MAX_INDEX_LOAD_FIND; ind++)
                {
                  printf ("ind=%d,%s\n", ind, my_recognize_ret_name (ind));
                }

              u8_ind_ch_st = random (0, MAX_INDEX_LOAD_FIND - 1);
              printf ("u8_ind_ch_st=%d\n", u8_ind_ch_st);
            }

          display_refresh ();
          i_delay_WAIT = 1;

          VS1053_PlayHttpMp3 (my_recognize_ret_url_resolved (u8_ind_ch_st));

          tls_os_time_delay (HZ);
          tls_watchdog_clr ();
        }
    }
}

void
UserMain (void)
{
  printf ("user task\n");

  tls_sys_clk_set (CPU_CLK_240M);

  tls_os_task_create (NULL, NULL, demo_console_task, NULL,
                      (void *)DemoTaskStk, /* task's stack start address */
                      DEMO_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      DEMO_TASK_PRIO, 0);
}
