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

#include "ff.h"
#include "wm_gpio_afsel.h"

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
#include "mod1/URTouch.h"
#include "mod1/UTFT_Buttons.h"
#include "mod1/UTFT.h"
#include "mod1/u_jpeg.h"

//****************************************************************************************************//
extern uint8_t BigFont[];   // подключаем большой шрифт
extern uint8_t Dingbats1_XL[];
static int but1;

u8 u8_wifi_state = 0;
extern u32 VS1053_WEB_RADIO_nTotal;


static u8 i_switch_menu = 0;
static int i_menu = 0;
static int i_menu2 = 0;
static u8 u8_ind_ch_st = 0;

static u16 u16_volume = 80; //
static char buf_str_ind[50];
static char stantion_uuid[39];
static int i_find_stantion_id = -1;
static char stantion_name_temp[50];
static char stantion_uuid_temp[39];

static void display_refresh (void);

#define MENU_STORE_VOLUME 0
#define MENU_STORE_INDEX 1
#define MENU_MAX_POS (USER_CNT_REC_STANTION_NAME * 2)

#define MENU2_MAX_POS (MAX_INDEX_LOAD_FIND-1)

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

void
Menu2ActionClick (void)
{
  u8_ind_ch_st = i_menu2;
  sprintf (stantion_uuid, my_recognize_ret_stationuuid (u8_ind_ch_st));
  // printf ("flash_cfg_load stantion_uuid %s,u8_stantion_id
  // %d\n",stantion_uuid,u8_stantion_id);
  VS1053_stop_PlayMP3 ();
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
    UTFT_setFont (BigFont);
  else
    UTFT_setFont (BigFont);
  sprintf (buf_str_ind, "%.2d.", (u8_Page_Disp * 4 + u8_stpos));
  UTFT_print (buf_str_ind, 1, 30 + u8_stpos * 10, 0);

  flash_cfg_load_stantion_name (buf_str_ind, (u8_Page_Disp * 4 + u8_stpos));
  if (buf_str_ind[0] == 0xd0)
    buf_str_ind[(l_store_choice ? 24 : 28)] = 0;
  else
    buf_str_ind[(l_store_choice ? 12 : 14)] = 0;
  UTFT_print (buf_str_ind, 16, 30 + u8_stpos * 10, 0);

  if (!l_name_choice)
    {
      if (l_store_choice)
        {
          UTFT_setFont (BigFont);
          sprintf (buf_str_ind, "Store%.2d", (u8_Page_Disp * 4 + u8_stpos));
          UTFT_print (buf_str_ind, 80, 30 + u8_stpos * 10, 0);
        }
      else
        {
          UTFT_setFont (BigFont);
          sprintf (buf_str_ind, "St%.2d", (u8_Page_Disp * 4 + u8_stpos));
          UTFT_print (buf_str_ind, 100, 30 + u8_stpos * 10, 0);
        }
    }
}

volatile static u16 i_delay_WAIT
    = 0; //если не 0, значит ждем данных с сервера веб станции

static void
display_refresh (void)
{
    {
      //UTFT_clrScr (); // стираем всю информацию с дисплея

      UTFT_setColor2 (VGA_BLACK); // Устанавливаем цвет
      UTFT_fillRect (0          //по горизонтали?
                     ,
                     0 // по вертикали?
                     ,
                     200//791 //длинна?
                     ,
                     200 //высота?
      ); // Рисуем закрашенный прямоугольник 

      //u8g2_SetDrawColor (&u8g2, 1);
      if (i_switch_menu == 0)
        {
          UTFT_setColor2 (VGA_SILVER); // 
          UTFT_setFont (BigFont);
          sprintf (buf_str_ind, "vol:%.3d", u16_volume);
          UTFT_print (buf_str_ind, 10, 20 , 0);

          UTFT_setFont (BigFont);

          UTFT_setColor2 (VGA_GREEN); // 

          UTFT_print ( my_recognize_ret_name (u8_ind_ch_st), 1, 40,0);
          UTFT_print ( my_recognize_ret_tags (u8_ind_ch_st), 1, 60,0);
          UTFT_print ( my_recognize_ret_country (u8_ind_ch_st), 1, 80,0);
          UTFT_print ( my_recognize_ret_codec (u8_ind_ch_st), 1, 120,0);
          UTFT_print ( my_recognize_ret_bitrate (u8_ind_ch_st),100 , 120,0);
          UTFT_print ( (my_recognize_ret_https (u8_ind_ch_st) ? "https" : "http"), 240, 120 ,0);
        }
      if (i_switch_menu == 1)
        {
          UTFT_setFont (BigFont);
          sprintf (buf_str_ind, "Menu1");
          UTFT_print (buf_str_ind, 0, 20 , 0);
          //

          UTFT_setFont (BigFont);
          sprintf (buf_str_ind, "%d", i_menu);
          UTFT_print (buf_str_ind, 75, 10 , 0);

          if (i_menu == MENU_STORE_VOLUME)
            UTFT_setFont (BigFont);
          else
            UTFT_setFont (BigFont);
          UTFT_print ("St.Vol", 75, 20, 0);

          u8 u8_stantion_id = (i_menu / 2);
          if ((u8_stantion_id * 2) == i_menu && u8_stantion_id > 0)
            u8_stantion_id--;
          display_menu_stantion_pos (u8_stantion_id, 0);
          display_menu_stantion_pos (u8_stantion_id, 1);
          display_menu_stantion_pos (u8_stantion_id, 2);
          display_menu_stantion_pos (u8_stantion_id, 3);

          if (i_find_stantion_id >= 0)
            {
              UTFT_setFont (BigFont);
              sprintf (buf_str_ind, "St%.2d.OV(%.2d)", u8_stantion_id,
                       i_find_stantion_id);
              UTFT_print(buf_str_ind,0,45,0);
              i_find_stantion_id = -1;
              i_delay_WAIT = 1;
            }
        }

      if (i_switch_menu == 2)
        {
          UTFT_setFont (BigFont);
          sprintf (buf_str_ind, "Menu2");
          UTFT_print (buf_str_ind, 0, 20 , 0);

          UTFT_setFont (BigFont);
          sprintf (buf_str_ind, "%d", i_menu2);
          UTFT_print (buf_str_ind, 85, 15 , 0);

          sprintf (buf_str_ind, my_recognize_ret_name (i_menu2));
          UTFT_print (buf_str_ind, 1, 38 , 0);

          UTFT_setFont (BigFont);
          UTFT_print ( my_recognize_ret_codec (i_menu2), 1, 60,0);
          UTFT_print ( my_recognize_ret_bitrate (i_menu2), 60, 60,0);
          UTFT_print ( (my_recognize_ret_https (i_menu2) ? "https" : "http"), 90, 60,0);
        }

      if (VS1053_status_get_status () != VS1053_PLAY
          || VS1053_WEB_RADIO_nTotal < 512)
        {
          //u8g2_SetDrawColor (&u8g2, 1);
          UTFT_setFont (BigFont);
          UTFT_setColor2 (VGA_YELLOW); // 
          UTFT_print("WAIT...", 30, 245,0);
        }
    }
}

//****************************************************************************************************//


static const u16 i_pos_dreb_CLK = 1; // таймер 300 Мкс, значит будет 300 MKs
volatile static u16 i_dreb_CLK = 0; // от дребезга

static int i_rotar = 10;
volatile static u16 i_rotar_zero = 0;
volatile static u16 i_rotar_one = 0;
static u8 u8_enc_state = 0;

static const u16 i_pos_dreb_SW
    = 1000; //кнопка,таймер 300 Мкс, значит будет 300 миллисекунд.
volatile static u8 i_dreb_SW = 0; // от дребезга кнопки

static const u16 i_pos_DBL_CLICK
    = 4000; // таймер 300 Мкс, значит будет 1.2 сек
volatile static u16 i_delay_SW_DBL_CLICK
    = 0; // двойной клик, переход в меню и обратно

static const u16 i_pos_delay_volume
    = 6000; // таймер 300 Мкс, значит будет 1.8 сек
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
              if (i_switch_menu == 1)
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
              if (i_switch_menu == 2)
                {
                  if (i_rotar_zero > i_rotar_one)
                    i_menu2--;
                  else
                    i_menu2++;
                  if (i_menu2 < 0)
                    i_menu2 = MENU2_MAX_POS;
                  if (i_menu2 > MENU2_MAX_POS)
                    i_menu2 = 0;
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
          if (i_switch_menu == 1)
            MenuActionClick ();
          if (i_switch_menu == 2)
            Menu2ActionClick ();
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

      int x = 0, y = 0;
      if (URTouch_flag_touch_isr)
        {
          int pressed_button = UTFT_Buttons_checkButtons();

          if (pressed_button==but1)
           UTFT_print("Button 1", 330, 330, 0);
          if (pressed_button==-1)
           UTFT_print("None    ", 330, 330, 0);

          //URTouch_read ();
          //x = URTouch_getX ();
          //y = URTouch_getY ();
          //if (x >= 0 && y >= 0)
          //  {
          //    UTFT_setColor2 (VGA_BLUE); // 800x480
          //    UTFT_fillCircle (x, y, 2); // Рисуем закрашенную окружность
          //  }
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

  printf ("WebRadio2 test app\n");

  tls_watchdog_init (20 * 1000 * 1000); // u32 usec microseconds, около 20 сек
  srand (tls_os_get_time ());           // time(NULL));

  printf ("user_app1_task start 3.97 TFT_397T_NT35510 800x480 16bit bus\n");

  //Цветной графический дисплей 3.97 TFT 800x480
  // подключаем библиотеку UTFT
  UTFT_UTFT (TFT_397T_NT35510, (u8)WM_IO_PA_01, (u8)WM_IO_PA_02,
             (u8)WM_IO_PA_03, (u8)WM_IO_PA_04, 0, 0);
  //                               byte RS,         byte WR,         byte CS,
  //                               byte RST, byte SER, u32 spi_freq
  // UTFT тип дисплея TFT_397T_NT35510
  // и номера выводов W801 к которым подключён дисплей: RS, WR,
  // CS, RST. Выводы параллельной шины данных не указываются
  // в данном случае, параллельная 16 бит шина = PB0 ... PB15

  UTFT_InitLCD (LANDSCAPE); // инициируем дисплей
  UTFT_clrScr (); // стираем всю информацию с дисплея
  UTFT_setColor2 (VGA_SILVER); // 

  // URTouch_URTouch(byte tclk, byte tcs, byte tdin, byte dout, byte irq);
  URTouch_URTouch ((u8)WM_IO_PA_05 // byte tclk
                   ,
                   (u8)WM_IO_PA_06 // byte tcs
                   ,
                   (u8)WM_IO_PA_07 // byte tdin
                   ,
                   (u8)WM_IO_PA_08 // byte dout
                   ,
                   (u8)WM_IO_PA_09 // byte irq
  );

  URTouch_InitTouch (LANDSCAPE);
// LANDSCAPE = URTouch_set_calibrate: calx=2C8F31, caly=3DDC053, cals=1DF31F
// PORTRAIT  = URTouch_set_calibrate: calx=31CEF2, caly=3ECC03D, cals=1DF31F
  URTouch_set_calibrate (0x2C8F31, 0x3DDC053, 0x1DF31F);
  URTouch_setPrecision (PREC_MEDIUM);

  UTFT_Buttons_UTFT_Buttons();
  UTFT_Buttons_setTextFont(BigFont);
  UTFT_Buttons_setSymbolFont(Dingbats1_XL);

  but1 = UTFT_Buttons_addButton( 400,  400, 300,  30, "Button 1",0);

  FATFS fs;
  FRESULT res_sd;
  char buff[256]; // буффер для названия директории при сканировании файловой
                  // системы
  wm_sdio_host_config (0);



  /* vs1053 */
  libVS1053_t user_data53 = {

    .rst_pin  = WM_IO_PA_10,  /* HW reset pin */
    .cs_pin   = WM_IO_PA_11,   /* ->xcs  SCI Chip Select pin */
    .dcs_pin  = WM_IO_PA_12,  /* ->xdcs SDI Chip Select pin */
    .dreq_pin = WM_IO_PA_13, /* <-dreq Data Request pin */

    .spi_cs = WM_IO_PA_00, /* not wire, но назначить надо */
    .spi_ck = WM_IO_PB_02, /*      ck -> sck Clock pin */
    .spi_di = WM_IO_PB_03, /* master miso di <- miso slave, на макетке board
                              HLK-W801-KIT-V1.1 работает только WM_IO_PB_03  */
    .spi_do = WM_IO_PB_05, /* master mosi do -> mosi slave */

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
  timer_cfg.timeout = 300; // 300 US, значит частота 3.333KHz
  timer_cfg.is_repeat = 1;
  timer_cfg.callback = (tls_timer_irq_callback)demo_timer_irq;
  timer_cfg.arg = NULL;
  timer_id = tls_timer_create (&timer_cfg);
  tls_timer_start (timer_id);
  printf ("timer start\n");
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
          if (i_switch_menu != 2)
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
