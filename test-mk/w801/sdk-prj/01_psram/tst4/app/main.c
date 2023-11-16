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

#include "mod1/UTFT.h"
#include "mod1/psram.h"
#include "mod1/u_jpeg.h"

//#include "ff.h"
//#include "utils.h"
//#include "wm_gpio_afsel.h"

#include "my_recognize.h"
#include "w_flash_cfg.h"
#include "w_wifi.h"

#include "mod1/VS1053.h"
#include "mod1/lfile_http.h"

#include "../../../../../../../w_wifi_pass.h"
//#define MY_WIFI_AP "bred8"
//#define MY_WIFI_PASS "123123123"
//#define MY_WIFI_AP "bred1"
//#define MY_WIFI_PASS "9115676369"

#include "w_wifi.h"

u8 volatile u8_wifi_state = 0;
static char stantion_uuid[39];

//#define TOUCH_ORIENTATION PORTRAIT
#define TOUCH_ORIENTATION LANDSCAPE

#define USER_APP1_TASK_SIZE 2048
static OS_STK UserApp1TaskStk[USER_APP1_TASK_SIZE];
#define USER_APP1_TASK_PRIO 32

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

void
user_app1_task (void *sdata)
{

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
  56pin - !если для MISO - спаять светодиод! а можно и оставить с vs1053 будет
  работать, только читать из неё не сможет

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
    = 1024 * 50, // 26400,   // подойдет 26400 более не надо! глючит!
    .psram_config = 1, // 0 или 1
    .psram_mode = PSRAM_SPI, // делай PSRAM_SPI, PSRAM_QPI - так и не работает
    .psram_frequency_divider = 2, // 2 - хорошо работает для ESP-PSRAM64H
    .psram_tCPH = 2,  // 2 - хорошо работает для ESP-PSRAM64H
    .psram_BURST = 1, // 1 - хорошо работает для ESP-PSRAM64H
    .psram_OVERTIMER = 2, // 2 - хорошо работает для ESP-PSRAM64H
    .load_buffer_debug = 1, // 0 , 1 - выводит инфу по заполнению "f" или "+",
                            // и опусташению буффера "-", "0" - нехватка данных

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
  u16 u16_volume = 75;
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
  if (true)
    {
      tls_timer_start (timer_id);
      printf ("timer start\n");
    }

  UTFT_clrScr ();
  UTFT_setFont (SmallFont);

  tls_watchdog_init (30 * 1000 * 1000); // u32 usec microseconds, около 60 сек
  for (;;) // цикл(1) с подсоединением к wifi и запросом времени
    {
      while (u8_wifi_state == 0)
        {
          printf ("trying to connect wifi\n");
          if (u8_wifi_state == 0
              && demo_connect_net (MY_WIFI_AP, MY_WIFI_PASS) == WM_SUCCESS)
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

      tls_os_time_delay (HZ * 5);
      tls_watchdog_clr ();

      while (u8_wifi_state == 1) // основной цикл(2)
        {

          tls_watchdog_clr ();

          printf ("load default stantion 0\n");
          // flash_cfg_load_stantion_uuid (stantion_uuid, 0);
          // sprintf(stantion_uuid,"%s","960559b0-0601-11e8-ae97-52543be04c81");
          // sprintf(stantion_uuid,"%s","3d0aad11-97ec-469c-835b-64f12c38dd0e");//https
          // sprintf(stantion_uuid,"%s","fc2e6c39-7139-4f7a-a0c6-a859244332be");//https
          sprintf (stantion_uuid, "%s",
                   "06bb1bd0-99f4-4ddd-b06a-eac29e313724"); // trance
          while (u8_wifi_state == 1) // основной цикл(2)
            {
              if (my_sost != VS1053_PLAY_BUF)
                {
                  my_recognize_http_reset ();
                  http_get_web_station_by_stationuuid (stantion_uuid);
                  // http_get_web_station_by_random();
                  printf (" my_recognize_ret_name = %s\n",
                          my_recognize_ret_name (0));
                  printf (" my_recognize_ret_url_resolved = %s\n",
                          my_recognize_ret_url_resolved (0));
                }

              VS1053_PlayHttpMp3 (my_recognize_ret_url_resolved (0));

              if (my_sost != VS1053_PLAY_BUF)
                tls_os_time_delay (HZ);
              // tls_watchdog_clr ();
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
