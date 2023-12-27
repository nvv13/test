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

#include "wm_type_def.h"

#include "wm_cpu.h"

#include "wm_watchdog.h"
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
#include "wm_timer.h"
//#include "wm_netif.h"
//#include "wm_efuse.h"
//#include "wm_mem.h"
//#include "wm_regs.h"
#include "wm_rtc.h"
#include "wm_uart.h"

#include "mod1/UTFT.h"
#include "mod1/psram.h"
#include "mod1/u_jpeg.h"

//#include "ff.h"
//#include "utils.h"
//#include "wm_gpio_afsel.h"

#include "my_recognize.h"
#include "w_flash_cfg.h"
#include "w_ntp.h"
#include "w_wifi.h"

#include "mod1/VS1053.h"
#include "mod1/lfile_http.h"

#include "../../../../../../../w_wifi_pass.h"
//#define MY_WIFI_AP "bred8"
//#define MY_WIFI_PASS "123123123"
//#define MY_WIFI_AP "bred1"
//#define MY_WIFI_PASS "9115676369"

#include "w_wifi.h"

extern void dumpBuffer (char *name, char *buffer, int len);

static char stantion_uuid[39];
static boolean b_ChkStationUuid = false;
static u16 u16_volume = 75;
static u16 stantion_index = 0;

//#define TOUCH_ORIENTATION PORTRAIT
#define TOUCH_ORIENTATION LANDSCAPE

#define USER_APP1_TASK_SIZE 2048
static OS_STK UserApp1TaskStk[USER_APP1_TASK_SIZE];
#define USER_APP1_TASK_PRIO 32

#define USER_APP2_TASK_SIZE 2048
static OS_STK UserApp2TaskStk[USER_APP2_TASK_SIZE];
#define USER_APP2_TASK_PRIO 31 // меньше цифра, меньше приоретет!

extern uint8_t SmallFont[]; // подключаем маленький шрифт
extern uint8_t BigFont[];   // подключаем большой шрифт
extern uint8_t
    SevenSegNumFont[]; // подключаем шрифт имитирующий семисегментный индикатор
extern uint8_t SmallSymbolFont[];
extern uint8_t Dingbats1_XL[];

// static char char_buff[100];

static void
my_timer_irq (u8 *arg) // здесь будет смена режима
{

  /*
                sprintf (char_buff, "  %d / %d  ",
     VS1053_WEB_RADIO_buf_chunk_free,VS1053_WEB_RADIO_buf_chunk_total );
                UTFT_setFont (BigFont);
                UTFT_setColor2 (VGA_RED);
                UTFT_print (char_buff, CENTER, 50, 0); // выводим текст на
     дисплей
  */
}

/*
#define URL_ARR_SIZE 14
static const char *aUrl[URL_ARR_SIZE] = {
  "http://188.9.157.80:81/axis-cgi/jpg/image.cgi?resolution=320x240",
  "http://www.cek.ef.uni-lj.si/EBSLG2004/"
  "EBSLG%20Continental%20Meeting%202004,%20Ljubljana/slides/P1010039.jpg",
  "http://www.cek.ef.uni-lj.si/EBSLG2004/"
  "EBSLG%20Continental%20Meeting%202004,%20Ljubljana/slides/P1010038.jpg",
  "http://mobiliv.ru/_ph/94/2/663041550.jpg",
  "http://mobiliv.ru/_ph/94/2/842704552.jpg",
  "http://mobiliv.ru/_ph/94/2/111786880.jpg",
  "http://mobiliv.ru/_ph/94/2/619137566.jpg",
  "http://mobiliv.ru/_ph/94/2/732111012.jpg",
  "http://mobiliv.ru/_ph/94/2/649581166.jpg",
  "http://mobiliv.ru/_ph/94/2/883010373.jpg",
  "http://mobiliv.ru/_ph/94/2/344263385.jpg",
  "http://mobiliv.ru/_ph/94/2/63757809.jpg",
  "http://mobiliv.ru/_ph/94/2/597387039.jpg",
  "http://mobiliv.ru/_ph/94/2/395978212.jpg"
};
*/

#define CONSOLE_BUF_SIZE 512
u8 rx_buf[CONSOLE_BUF_SIZE + 1];
volatile int rx_data_len;
volatile u32 rptr;

s16
proc_console_rx (u16 len, void *user_data)
{
  rx_data_len += len;
  return 0;
}

void
user_app2_task (void *sdata)
{
  memset (rx_buf, 0, CONSOLE_BUF_SIZE + 1);
  rx_data_len = 0;
  rptr = 0;

  tls_uart_set_baud_rate (TLS_UART_0, 115200);
  tls_uart_rx_callback_register (TLS_UART_0, proc_console_rx, NULL);

  while (1)
    {
      tls_os_time_delay (200);
      if (b_ChkStationUuid && VS1053_WEB_RADIO_nTotal > (1024 * 1024 * 1))
        {
          b_ChkStationUuid = false;
          char cfg_stantion_uuid[39];
          flash_cfg_load_stantion_uuid (cfg_stantion_uuid, stantion_index);
          if (!strstr (cfg_stantion_uuid, stantion_uuid))
            {
              flash_cfg_store_stantion_uuid (stantion_uuid, stantion_index);
              printf ("flash_cfg_store_ new stantion_uuid index=%d\r\n",
                      stantion_index);
            }
          u16 st_index = 0;
          flash_cfg_load_u16 (&st_index, 0);
          if (stantion_index != st_index)
            {
              flash_cfg_store_u16 (stantion_index, 0);
              printf ("flash_cfg_store_u16 new stantion_index=%d\r\n",
                      stantion_index);
            }
        }
      if (rx_data_len > 0)
        {
          if (rptr + rx_data_len > CONSOLE_BUF_SIZE)
            {
              printf ("rptr + rx_data_len > CONSOLE_BUF_SIZE\r\n");
              memset (rx_buf, 0, CONSOLE_BUF_SIZE + 1);
              rx_data_len = 0;
              rptr = 0;
            }
          else
            {
              int ret = tls_uart_read (TLS_UART_0, rx_buf + rptr, rx_data_len);
              if (ret > 0)
                {
                  // printf ("%s", (char *)(rx_buf + rptr));
                  // dumpBuffer ("\ruart0", (char *)rx_buf, 64);
                  /*

                  Клавиши - стрелки, кода:
                  uart0  Left: 1B, 5B, 44,
                  uart0    Up: 1B, 5B, 41,
                  uart0  Down: 1B, 5B, 42,
                  uart0 Right: 1B, 5B, 43,

                  */
                  char *pHeaderEnd = strstr ((char *)(rx_buf + rptr), "\r");
                  rx_data_len -= ret;
                  rptr += ret;
                  if (pHeaderEnd)
                    {
                      (*pHeaderEnd) = 0;
                      pHeaderEnd--;
                      if ((*pHeaderEnd) == '"')
                        (*pHeaderEnd) = 0;
                      pHeaderEnd = (char *)rx_buf;
                      if ((*pHeaderEnd) == '"')
                        pHeaderEnd++;
                      printf ("buf = \"%s\"\r\n", pHeaderEnd);
                      if (strlen (pHeaderEnd) == 36)
                        {
                          sprintf (stantion_uuid, "%s", pHeaderEnd);
                          tls_os_time_delay (500);
                          printf ("new %d stantion_uuid = \"%s\"\r\n",
                                  stantion_index, stantion_uuid);
                          my_sost = VS1053_QUERY_TO_STOP;
                        }
                      else
                        {
                          if (strstr (pHeaderEnd, "D"))
                            {
                              switch (VS1053_WEB_RADIO_load_buffer_debug)
                                {
                                case VSHTTP_DEBUG_TYPE2:
                                  {
                                    VS1053_WEB_RADIO_load_buffer_debug
                                        = VSHTTP_DEBUG_NO_DEBUG;
                                    printf ("off debug\r\n");
                                  };
                                  break;
                                case VSHTTP_DEBUG_NO_DEBUG:
                                  {
                                    VS1053_WEB_RADIO_load_buffer_debug
                                        = VSHTTP_DEBUG_TYPE1;
                                    printf ("on debug 1\r\n");
                                  };
                                  break;
                                case VSHTTP_DEBUG_TYPE1:
                                  {
                                    VS1053_WEB_RADIO_load_buffer_debug
                                        = VSHTTP_DEBUG_TYPE2;
                                    printf ("on debug 2\r\n");
                                  };
                                  break;
                                default:
                                  VS1053_WEB_RADIO_load_buffer_debug
                                      = VSHTTP_DEBUG_NO_DEBUG;
                                }
                              tls_watchdog_clr ();
                            }
                          if (strstr (pHeaderEnd, "W"))
                            {
                              // restart WiFi ?
                              u8_wifi_state = 0;
                              tls_watchdog_clr ();
                            } 
                        }
                      memset (rx_buf, 0, CONSOLE_BUF_SIZE + 1);
                      rx_data_len = 0;
                      rptr = 0;
                    }
                  else
                    {
                      pHeaderEnd = strstr ((char *)(rx_buf),
                                           "\x1B\x5B\x41"); // Up
                      if (pHeaderEnd && u16_volume < 100)
                        {
                          u16_volume++;
                          VS1053_setVolume (u16_volume);
                          memset (rx_buf, 0, CONSOLE_BUF_SIZE + 1);
                          rx_data_len = 0;
                          rptr = 0;
                          printf ("setVolume++ = %d \r\n", u16_volume);
                          tls_watchdog_clr ();
                        }
                      pHeaderEnd = strstr ((char *)(rx_buf),
                                           "\x1B\x5B\x42"); // Down
                      if (pHeaderEnd && u16_volume > 2)
                        {
                          u16_volume--;
                          VS1053_setVolume (u16_volume);
                          memset (rx_buf, 0, CONSOLE_BUF_SIZE + 1);
                          rx_data_len = 0;
                          rptr = 0;
                          printf ("setVolume-- = %d \r\n", u16_volume);
                          tls_watchdog_clr ();
                        }

                      pHeaderEnd = strstr ((char *)(rx_buf),
                                           "\x1B\x5B\x44"); // Left
                      if (pHeaderEnd && stantion_index > 0)
                        {
                          stantion_index--;
                          memset (rx_buf, 0, CONSOLE_BUF_SIZE + 1);
                          rx_data_len = 0;
                          rptr = 0;
                          printf ("set -- stantion_index = %d \r\n",
                                  stantion_index);
                          char cfg_stantion_uuid[39];
                          flash_cfg_load_stantion_uuid (cfg_stantion_uuid,
                                                        stantion_index);
                          if (strlen (cfg_stantion_uuid) == 36
                              && cfg_stantion_uuid[0] >= 0x30
                              && cfg_stantion_uuid[0] <= 0x7A)
                            {
                              flash_cfg_load_stantion_uuid (stantion_uuid,
                                                            stantion_index);
                              my_sost = VS1053_QUERY_TO_STOP;
                              tls_watchdog_clr ();
                            }
                          else
                            printf ("empty stantion_index = %d \r\n",
                                    stantion_index);
                        }

                      pHeaderEnd = strstr ((char *)(rx_buf),
                                           "\x1B\x5B\x43"); // Right
                      if (pHeaderEnd && stantion_index < 43)
                        {
                          stantion_index++;
                          memset (rx_buf, 0, CONSOLE_BUF_SIZE + 1);
                          rx_data_len = 0;
                          rptr = 0;
                          printf ("set ++ stantion_index = %d \r\n",
                                  stantion_index);
                          char cfg_stantion_uuid[39];
                          flash_cfg_load_stantion_uuid (cfg_stantion_uuid,
                                                        stantion_index);
                          if (strlen (cfg_stantion_uuid) == 36
                              && cfg_stantion_uuid[0] >= 0x30
                              && cfg_stantion_uuid[0] <= 0x7A)
                            {
                              flash_cfg_load_stantion_uuid (stantion_uuid,
                                                            stantion_index);
                              my_sost = VS1053_QUERY_TO_STOP;
                              tls_watchdog_clr ();
                            }
                          else
                            printf ("empty stantion_index = %d \r\n",
                                    stantion_index);
                        }
                    }
                }
            }
        }
    }
}

void
user_app1_task (void *sdata)
{

  tls_watchdog_init (30 * 1000 * 1000); // u32 usec microseconds, около 60 сек

  /* vs1053 */
  libVS1053_t user_data53 = {

    .rst_pin = WM_IO_PB_17,  /* HW reset pin */
    .cs_pin = WM_IO_PB_11,   /* ->xcs  SCI Chip Select pin */
    .dcs_pin = WM_IO_PB_16,  /* ->xdcs SDI Chip Select pin */
    .dreq_pin = WM_IO_PB_18, /* <-dreq Data Request pin */

    .spi_cs = WM_IO_PB_23, /* not wire, но назначить надо */
    .spi_ck = WM_IO_PB_24, /*      ck -> sck Clock pin */
    .spi_di = WM_IO_PB_25, // WM_IO_PB_03, //WM_IO_PB_16,
    /* master miso di <- miso slave, на макетке board
                               HLK-W801-KIT-V1.1 работает только WM_IO_PB_03
  или wm_spi_di_config - возможные пины -WM_IO_PB_00 -WM_IO_PB_03 WM_IO_PB_16
  only for 56pin - !если для MISO - спаять светодиод! а можно и оставить с
  vs1053 будет работать, только читать из неё не сможет WM_IO_PB_25 only for
  56pin - !если для MISO - спаять светодиод! а можно и оставить с vs1053
  будет работать, только читать из неё не сможет

 */
    .spi_do = WM_IO_PB_26, /* master mosi do -> mosi slave */

    /**
     * @brief  config the pins used for psram ck cs dat0 dat1 dat2 dat3
     * @param  numsel: config psram ck cs dat0 dat1 dat2 dat3 pins multiplex
    relation,valid para 0,1 *			0:                 1: only for
    56pin *			  psram_ck   PB00    psram_ck   PA15 *
    psram_cs   PB01    psram_cs   PB27
     *			  psram_dat0 PB02    psram_dat0 PB02
     *			  psram_dat1 PB03    psram_dat1 PB03
     *			  psram_dat2 PB04    psram_dat2 PB04
     *			  psram_dat3 PB05    psram_dat3 PB05

     * @return None
    void wm_psram_config(uint8_t numsel);
     */

    /* далее настройки для VS1053_PlayHttpMp3 */
    .no_psram_BufferSize
    = 4000, // подойдет 4000, более - программа начнет глючить
    .psram_BufferSize
    = 1024 * 260, // 26400,   // подойдет 26400 более не надо! глючит!
    .psram_config = 1, // 0 или 1
    .psram_mode = PSRAM_SPI, // делай PSRAM_SPI, PSRAM_QPI - так и не работает
    .psram_frequency_divider = 2, // 2 - хорошо работает для ESP-PSRAM64H
    .psram_tCPH = 2,  // 2 - хорошо работает для ESP-PSRAM64H
    .psram_BURST = 1, // 1 - хорошо работает для ESP-PSRAM64H
    .psram_OVERTIMER = 2, // 2 - хорошо работает для ESP-PSRAM64H
    .load_buffer_debug = VSHTTP_DEBUG_NO_DEBUG, //VSHTTP_DEBUG_TYPE2, // 0 , 1 - выводит инфу по заполнению "f" или "+",
                            // и опусташению буффера "-", "0" - нехватка данных
    .spi_fastest_speed
    = 0, // 0 - 4 MHz работает на большенстве плат, 1 - 6 MHz

  };

  /**
   * config the pins used for spi di
   * WM_IO_PB_00 - не работает,
   * WM_IO_PB_03 - работает!
   * WM_IO_PB_16 only for 56pin - не работает, мешает светодиод подключенный
   * к данному контакту на макетке WM_IO_PB_25 only for 56pin - не работает,
   * мешает светодиод подключенный к данному контакту на макетке
   */

  VS1053_VS1053 (&user_data53);
  VS1053_begin ();
  u16_volume = 75;
  VS1053_setVolume (u16_volume);

  // подключаем библиотеку UTFT

  printf ("user_app1_task start TFT02_0V89 240x320 SDIO SPI st7789\n");
  UTFT_UTFT (TFT02_0V89,
             (u8)NO_GPIO_PIN // WM_IO_PB_17  // SDA
             ,
             (u8)NO_GPIO_PIN // WM_IO_PB_15  // SCL
             ,
             (u8)WM_IO_PB_14 //(u8)NO_GPIO_PIN //  //CS  CS
             ,
             (u8)WM_IO_PB_21 // RST reset RES
             ,
             (u8)WM_IO_PB_22 // SER => DC !
             ,
             // 120000000
             60000000
             /* spi_freq(Герц) для 5 контактных SPI дисплеев
                (где отдельно ножка комманда/данные)
             програмируеться HW SPI на ножки (предопред)
                 wm_spi_cs_config (WM_IO_PB_14);
                 wm_spi_ck_config (WM_IO_PB_15);
                 wm_spi_di_config (WM_IO_PB_16);
                 wm_spi_do_config (WM_IO_PB_17);
             но, можно отказаться от HW SPI в пользу Soft SPI
             установив spi_freq=0
             эмуляции SPI, это удобно для разных ножек
           */
  );

  /*
    printf ("user_app1_task start TFT02_0V89 240x320 HW SPI st7789\n");
    UTFT_UTFT (TFT02_0V89
               ,
               (u8)NO_GPIO_PIN // WM_IO_PB_17  //SDA
               ,
               (u8)NO_GPIO_PIN // WM_IO_PB_15  //SCL
               ,
               (u8)NO_GPIO_PIN // WM_IO_PB_14  //CS
               ,
               (u8)WM_IO_PB_21 // RST reset RES
               ,
               (u8)WM_IO_PB_22 // SER => DC !
               ,
               20000000
    );
  */

  // UTFT_InitLCD (TOUCH_ORIENTATION); // инициируем дисплей
  UTFT_InitLCD (LANDSCAPE); // инициируем дисплей
  // UTFT_InitLCD (PORTRAIT);

  u8 timer_id;
  struct tls_timer_cfg timer_cfg;
  timer_cfg.unit = TLS_TIMER_UNIT_MS;
  timer_cfg.timeout = 1000; //
  timer_cfg.is_repeat = 1;
  timer_cfg.callback = (tls_timer_irq_callback)my_timer_irq;
  timer_cfg.arg = NULL;
  timer_id = tls_timer_create (&timer_cfg);
  if (false)
    {
      tls_timer_start (timer_id);
      printf ("timer start\n");
    }

  tls_os_task_create (NULL, NULL, user_app2_task, NULL,
                      (void *)UserApp2TaskStk, /* task's stack start address */
                      USER_APP2_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      USER_APP2_TASK_PRIO, 0);

  UTFT_clrScr ();
  UTFT_setFont (SmallFont);

  stantion_index = 0;
  flash_cfg_load_u16 (&stantion_index, 0);
  if (stantion_index > 44)
    stantion_index = 0;
  printf ("load default stantion index = %d\n", stantion_index);
  flash_cfg_load_stantion_uuid (stantion_uuid, stantion_index);
  if (strlen (stantion_uuid) != 36)
    {
      printf ("strlen (stantion_uuid) != 36, set default uuid\n");
      // sprintf(stantion_uuid,"%s","960559b0-0601-11e8-ae97-52543be04c81");
      // sprintf(stantion_uuid,"%s","3d0aad11-97ec-469c-835b-64f12c38dd0e");//https
      // sprintf(stantion_uuid,"%s","fc2e6c39-7139-4f7a-a0c6-a859244332be");//https
      sprintf (stantion_uuid, "%s",
               "06bb1bd0-99f4-4ddd-b06a-eac29e313724"); // America Stereo Relax
                                                        // - норм

      // лучьшее sprintf (stantion_uuid,
      // "%s","dbaf0701-7987-11ea-8a3b-52543be04c81"); //depeche
      // mode,ebm,futurepop,synt

      // норм sprintf (stantion_uuid, "%s",
      //         "01899f00-cbe9-46bc-85ee-e7bfe6496f97"); // "Europe 2 Happy
      //                                                  // Rock Hours"
      //                                                  // https 128

      // так себе sprintf (
      //     stantion_uuid, "%s",
      //     "45d3bd35-1f11-4027-a1f1-f273b22a343d"); // "Slide Guitar /
      // Caprice
      // Radio""AAC+""bitrate":="320,"
      // sprintf (stantion_uuid, "%s",
      //         "8767165e-d2bd-4c74-838c-430c822ed6a5");// "Sfliny
      //         Classic Rock. " 320 https!
    }

  // printf ("start scan_demo \n");
  // scan_demo();
  // printf ("stop scan_demo \n");
  printf ("start scan_format2_demo \n");
  scan_format2_demo ();
  printf ("stop scan_format2_demo \n");

  printf ("key insert \"stantion_uuid\" + Enter - change stantion, uuid "
          "looking for https://www.radio-browser.info/ \n");
  printf ("key D + Enter - change Debug \n");
  printf ("key Up Down - volume control (1-100) \n");
  printf ("key Right Left - stantion index (0-43)\n");
  tls_watchdog_clr ();
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
              // u8_wifi_state = 1;
            }
          else
            {
              tls_os_time_delay (HZ * 5);
            }
        }

      tls_os_time_delay (HZ * 3);
      tls_watchdog_clr ();
      // ntp_set_server_demo ("0.fedora.pool.ntp.org", "1.fedora.pool.ntp.org",
      //                     "2.fedora.pool.ntp.org");
      ntp_demo ();

      while (u8_wifi_state == 1) // основной цикл(2)
        {

          tls_watchdog_clr ();

          while (u8_wifi_state == 1) // основной цикл(2)
            {
              if (my_sost != VS1053_PLAY_BUF)
                {
                  my_recognize_http_reset ();
                  http_get_web_station_by_stationuuid (stantion_uuid);
                  // http_get_web_station_by_random();
                  VS1053_WEB_RADIO_nTotal = 0;
                  b_ChkStationUuid = true;
                  printf (" my_recognize_ret_name = %s\n",
                          my_recognize_ret_name (0));
                  printf (" my_recognize_ret_url_resolved = %s\n",
                          my_recognize_ret_url_resolved (0));
                  printf (" my_recognize_ret_country = %s\n",
                          my_recognize_ret_country (0));
                  printf (" my_recognize_ret_tags = %s\n",
                          my_recognize_ret_tags (0));
                  printf (" my_recognize_ret_codec = %s\n",
                          my_recognize_ret_codec (0));
                  printf (" my_recognize_ret_bitrate = %s\n",
                          my_recognize_ret_bitrate (0));

                  UTFT_clrScr (); // стираем всю информацию с дисплея
                  UTFT_setFont (BigFont); // устанавливаем большой шрифт
                  UTFT_setColor2 (VGA_BLUE); // устанавливаем синий цвет текста
                  UTFT_print (my_recognize_ret_name (0), LEFT, 10, 0);
                  UTFT_print (my_recognize_ret_country (0), LEFT, 40, 0);
                  UTFT_print (my_recognize_ret_tags (0), LEFT, 70, 0);
                  UTFT_print (my_recognize_ret_codec (0), LEFT, 100, 0);
                  UTFT_print (my_recognize_ret_bitrate (0), LEFT, 130, 0);
                }

              VS1053_PlayHttpMp3 (my_recognize_ret_url_resolved (0));

              if (my_sost != VS1053_PLAY_BUF)
                tls_os_time_delay (HZ);
            }
        }
      tls_watchdog_clr ();
    }
}

void
UserMain (void)
{
  printf ("UserMain start");
  tls_sys_clk_set (CPU_CLK_240M);
  // tls_sys_clk_set (CPU_CLK_2M);
  // tls_sys_clk_set (CPU_CLK_40M);
  // tls_sys_clk_set (CPU_CLK_80M);
  // tls_sys_clk_set (CPU_CLK_160M);

  tls_os_task_create (NULL, NULL, user_app1_task, NULL,
                      (void *)UserApp1TaskStk, /* task's stack start address */
                      USER_APP1_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      USER_APP1_TASK_PRIO, 0);
}
