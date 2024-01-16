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
#include "w_ntp.h"
#include "w_wifi.h"

#define INIT_TASK_SIZE 2048
static OS_STK InitTaskStk[INIT_TASK_SIZE];
#define INIT_TASK_PRIO 32

#define MENU_TASK_SIZE 2048
static OS_STK MenuTaskStk[MENU_TASK_SIZE];
#define MENU_TASK_PRIO (INIT_TASK_PRIO)

#include "mod1/VS1053.h"
#include "mod1/encoder.h"
#include "mod1/u8g2.h"
#include "mod1/u8x8_riotos.h"

//****************************************************************************************************//
#include "my_display.h"
#include "my_rand.h"

//****************************************************************************************************//

static int i_rotar = 10;
//****************************************************************************************************//

void
menu_task (void *sdata)
{
  set_encoder_diff (0);
  bool btn_state = get_encoder_btn_state ();
  bool add_btn_1 = get_add_btn_1_state ();
  while (1)
    {

      int i_enc_diff = get_encoder_diff ();
      if (i_enc_diff != 0)
        {
          set_encoder_diff (0);
          {
            switch (i_switch_menu)
              {
              case 0:
                {
                  i_rotar+=i_enc_diff;
                  if (i_rotar < 0)
                    i_rotar = 0;
                  if (i_rotar > 100)
                    i_rotar = 100;
                  if (u16_volume != 100 - i_rotar)
                    {
                      u16_volume = 100 - i_rotar;
                      VS1053_setVolume (u16_volume);
                      tls_os_time_delay (HZ / 2);
                    }
                };
                break;
              case 1:
                {
                  i_menu+=i_enc_diff;
                  if (i_menu < 0)
                    i_menu = MENU_MAX_POS;
                  if (i_menu > MENU_MAX_POS)
                    i_menu = 0;
                };
                break;
              case 2:
                {
                  i_menu2+=i_enc_diff;
                  if (i_menu2 < 0)
                    i_menu2 = MENU2_MAX_POS;
                  if (i_menu2 > MENU2_MAX_POS)
                    i_menu2 = 0;
                };
                break;
              }
            display_refresh ();
          }
        }

      bool btn_state_curr = get_encoder_btn_state ();
      if (btn_state != btn_state_curr) //Нажали
        {
          btn_state = btn_state_curr;
          if (btn_state)
            {
              // sprintf (msg, " button RELEASE ");
            }
          else
            {
              // sprintf (msg, "  button PUSH   ");
              switch (i_switch_menu)
                {
                case 0:
                  {
                    stantion_uuid[0] = 0;
                    VS1053_stop_PlayMP3 ();
                  };
                  break;
                case 1:
                  MenuActionClick ();
                  break;
                case 2:
                  Menu2ActionClick ();
                  break;
                }
            }
        }

      if (i_delay_WAIT > 0) // ждем (x сек) для того чтоб убрать надпись
                            // "WAIT..." на дисплее, если уже воспроизведение
        {
          if (i_delay_WAIT++ > 100)
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

      bool add_btn_1_curr = get_add_btn_1_state ();
      if (add_btn_1 != add_btn_1_curr) //Нажали
        {
          add_btn_1 = add_btn_1_curr;
          if (add_btn_1)
            {
              // sprintf (msg, " button RELEASE ");
            }
          else
            {
              // sprintf (msg, "  button PUSH   ");
              i_switch_menu++;
              if (i_switch_menu > 2)
                i_switch_menu = 0;
              if (i_switch_menu == 1 && i_menu > 1 && i_menu % 2 == 0)
                i_menu--;
              display_refresh ();
            }
        }

      tls_os_time_delay (HZ / 100);
    }
}

void
init_task (void *sdata)
{

  tls_watchdog_init (20 * 1000 * 1000); // u32 usec microseconds, около 20 сек
  srand (tls_os_get_time ());           // time(NULL));

  printf ("Initializing to I2C oled Display.\n");

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
  u8g2_InitDisplay (&u8g2);
  u8g2_SetPowerSave (&u8g2, 0);

  printf ("Initializing to SPI vs1053.\n");
  /* vs1053 */
  libVS1053_t user_data53 = {

    .rst_pin = WM_IO_PB_17,  /* HW reset pin */
    .cs_pin = WM_IO_PB_21,   /* ->xcs  SCI Chip Select pin */
    .dcs_pin = WM_IO_PB_22,  /* ->xdcs SDI Chip Select pin */
    .dreq_pin = WM_IO_PB_18, /* <-dreq Data Request pin */

    .spi_cs = WM_IO_PB_23, /* not wire, но назначить надо */
    .spi_ck = WM_IO_PB_24, /*      ck -> sck Clock pin */
    .spi_di = WM_IO_PB_16, // WM_IO_PB_03, //WM_IO_PB_25,
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
    = 1024 * 100, // 26400,   // подойдет 26400 более не надо! глючит!
    .psram_config = 1, // 0 или 1
    .psram_mode = PSRAM_SPI, // делай PSRAM_SPI, PSRAM_QPI - так и не работает
    .psram_frequency_divider = 2, // 2 - хорошо работает для ESP-PSRAM64H
    .psram_tCPH = 2,  // 2 - хорошо работает для ESP-PSRAM64H
    .psram_BURST = 1, // 1 - хорошо работает для ESP-PSRAM64H
    .psram_OVERTIMER = 2, // 2 - хорошо работает для ESP-PSRAM64H
    .load_buffer_debug = 0, // 0 , 1 - выводит инфу по заполнению "f" или "+",
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

  printf ("Initializing to GPIO ENCODER.\n");
  libENCODER_t enc_pin = {
    .ENCODER_S = WM_IO_PA_11,
    .ENCODER_A = WM_IO_PA_12,
    .ENCODER_B = WM_IO_PA_13,
    .ADD_BUTTON_1 = WM_IO_PA_14,
    .ADD_BUTTON_2 = NO_GPIO_PIN,
    .ADD_BUTTON_3 = NO_GPIO_PIN,
  };
  bsp_encoder_init (&enc_pin);

  printf ("Load def and prev store values.\n");
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

  /**/
  tls_os_task_create (NULL, "MENU_TASK", menu_task, NULL,
                      (void *)MenuTaskStk, /* task's stack start address */
                      MENU_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      MENU_TASK_PRIO, 0);
  /**/

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

      tls_watchdog_clr ();

      if (my_sost != VS1053_PLAY_BUF)
        {
          tls_os_time_delay (HZ);

          // ntp_set_server_demo ("0.fedora.pool.ntp.org",
          //                     "1.fedora.pool.ntp.org",
          //                     "2.fedora.pool.ntp.org");
          ntp_demo ();

          my_recognize_http_reset ();
          printf ("load default stantion 0\n");
          flash_cfg_load_stantion_uuid (stantion_uuid, 0);
        }

      while (u8_wifi_state == 1) // основной цикл(2)
        {

          tls_watchdog_clr ();

          while (u8_wifi_state == 1) // основной цикл(2)
            {
              if (my_sost != VS1053_PLAY_BUF)
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
                              printf ("ind=%d,%s\n", ind,
                                      my_recognize_ret_name (ind));
                            }

                          u8_ind_ch_st = random (0, MAX_INDEX_LOAD_FIND - 1);
                          printf ("u8_ind_ch_st=%d\n", u8_ind_ch_st);
                        }
                    }
                  display_refresh ();
                  i_delay_WAIT = 1;
                }
              VS1053_PlayHttpMp3 (
                  my_recognize_ret_url_resolved (u8_ind_ch_st));

              if (my_sost != VS1053_PLAY_BUF)
                tls_os_time_delay (HZ);
            }
        }
    }
}

void
UserMain (void)
{
  printf ("user task\n");

  tls_sys_clk_set (CPU_CLK_240M);

  tls_os_task_create (NULL, "INIT_TASK", init_task, NULL,
                      (void *)InitTaskStk, /* task's stack start address */
                      INIT_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      INIT_TASK_PRIO, 0);
}
