/*****************************************************************************
 *
 * File Name : main.c
 *
 *
 * Date : 2023-09-20
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wm_type_def.h"

#include "wm_cpu.h"
#include "wm_gpio.h"
#include "wm_gpio_afsel.h"
#include "wm_io.h"
#include "wm_mem.h"
#include "wm_osal.h"
#include "wm_pmu.h"
#include "wm_psram.h"
#include "wm_regs.h"
#include "wm_rtc.h"
#include "wm_type_def.h"

#include <FreeRTOS.h>
#include <task.h>


#include "wm_timer.h"
#include "wm_watchdog.h"

#include "../../../../../../../w_wifi_pass.h"
//#define MY_WIFI_AP "bred8"
//#define MY_WIFI_PASS "123123123"
//#define MY_WIFI_AP "bred1"
//#define MY_WIFI_PASS "9115676369"

#include "my_recognize.h"
#include "w_flash_cfg.h"
#include "w_wifi.h"
#include "w_ntp.h"

#include "mod1/VS1053.h"

//**********************************************************************************************************************************//
u8 volatile u8_wifi_state = 0;
extern u32 VS1053_WEB_RADIO_nTotal;
static char stantion_uuid[39];

void
UserMain (void)
{

  printf ("test ring buf + VS1053\r\n");

  tls_sys_clk_set (CPU_CLK_240M);

  tls_watchdog_init (20 * 1000 * 1000); // u32 usec microseconds, около 20 сек

  /* vs1053 */
  libVS1053_t user_data53 = {

    .rst_pin = WM_IO_PB_17,  /* HW reset pin */
    .cs_pin = WM_IO_PB_21,   /* ->xcs  SCI Chip Select pin */
    .dcs_pin = WM_IO_PB_22,  /* ->xdcs SDI Chip Select pin */
    .dreq_pin = WM_IO_PB_18, /* <-dreq Data Request pin */

    .spi_cs = WM_IO_PB_23, /* not wire, но назначить надо */
    .spi_ck = WM_IO_PB_24, /*      ck -> sck Clock pin */
    .spi_di = WM_IO_PB_16,//WM_IO_PB_03, //WM_IO_PB_25, 
   /* master miso di <- miso slave, на макетке board
                              HLK-W801-KIT-V1.1 работает только WM_IO_PB_03 или 
 wm_spi_di_config - возможные пины
 			-WM_IO_PB_00
 			-WM_IO_PB_03
 			WM_IO_PB_16 only for 56pin - !если для MISO - спаять светодиод! 
                           а можно и оставить с vs1053 будет работать, только читать из неё не сможет
 			WM_IO_PB_25 only for 56pin - !если для MISO - спаять светодиод!  
                           а можно и оставить с vs1053 будет работать, только читать из неё не сможет

*/
    .spi_do = WM_IO_PB_26, /* master mosi do -> mosi slave */


/**
 * @brief  config the pins used for psram ck cs dat0 dat1 dat2 dat3
 * @param  numsel: config psram ck cs dat0 dat1 dat2 dat3 pins multiplex relation,valid para 0,1
 *			0:                 1: only for 56pin
 *			  psram_ck   PB00    psram_ck   PA15
 *			  psram_cs   PB01    psram_cs   PB27
 *			  psram_dat0 PB02    psram_dat0 PB02
 *			  psram_dat1 PB03    psram_dat1 PB03
 *			  psram_dat2 PB04    psram_dat2 PB04
 *			  psram_dat3 PB05    psram_dat3 PB05

 * @return None
void wm_psram_config(uint8_t numsel);
 */

    /* далее настройки для VS1053_PlayHttpMp3 */
    .no_psram_BufferSize=4000,// подойдет 4000, более - программа начнет глючить
    .psram_BufferSize=26400,   // подойдет 26400 более не надо! глючит! 
    .psram_config=1,//0 или 1 
    .psram_mode=PSRAM_SPI,// делай PSRAM_SPI, PSRAM_QPI - так и не работает
    .psram_frequency_divider=2,//2 - хорошо работает для ESP-PSRAM64H 
    .psram_tCPH=2,//2 - хорошо работает для ESP-PSRAM64H 
    .psram_BURST=1,//1 - хорошо работает для ESP-PSRAM64H 
    .psram_OVERTIMER=2,//2 - хорошо работает для ESP-PSRAM64H 
    .load_buffer_debug=1,//0 , 1 - выводит инфу по заполнению "f" или "+", и опусташению буффера "-", "0" - нехватка данных


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
  u16 u16_volume=75;
  VS1053_setVolume (u16_volume);

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

      tls_os_time_delay (HZ*3);

//      u8 u8_ntp_state = 0;
//      ntp_set_server_demo ("216.239.35.12","time.google.com", "ntp0.ntp-servers.net");
ntp_set_server_demo ("0.fedora.pool.ntp.org","1.fedora.pool.ntp.org","2.fedora.pool.ntp.org");
//      tls_os_time_delay (HZ*3);
//      while (u8_ntp_state < 3)
  //      {
//          printf ("trying to get ntp\n");
//          if (u8_ntp_state == 0 && 
          ntp_demo ();// == WM_SUCCESS)
//            u8_ntp_state = 100;
//          else
//            {
//              tls_os_time_delay (HZ*3);
//              tls_watchdog_clr ();
//              u8_ntp_state++;
//            }
//        }

      struct tm tblock;
      tls_get_rtc (&tblock);
      printf("sec=%d,min=%d,hour=%d,mon=%d,year=%d\n",tblock.tm_sec,tblock.tm_min,tblock.tm_hour,tblock.tm_mon+1,tblock.tm_year+1900);

      tls_watchdog_clr ();

      my_recognize_http_reset ();
      printf ("load default stantion 0\n");
      //flash_cfg_load_stantion_uuid (stantion_uuid, 0);
      //sprintf(stantion_uuid,"%s","960559b0-0601-11e8-ae97-52543be04c81");
      //sprintf(stantion_uuid,"%s","3d0aad11-97ec-469c-835b-64f12c38dd0e");//https 
      //sprintf(stantion_uuid,"%s","fc2e6c39-7139-4f7a-a0c6-a859244332be");//https 
      sprintf(stantion_uuid,"%s","06bb1bd0-99f4-4ddd-b06a-eac29e313724");//trance
      while (u8_wifi_state == 1) // основной цикл(2)
        {
          my_recognize_http_reset ();
          http_get_web_station_by_stationuuid (stantion_uuid);
          //http_get_web_station_by_random();
          printf (" my_recognize_ret_name = %s\n",my_recognize_ret_name (0));
          printf (" my_recognize_ret_url_resolved = %s\n",my_recognize_ret_url_resolved (0));
  
          VS1053_PlayHttpMp3 (my_recognize_ret_url_resolved (0));

          tls_os_time_delay (HZ);
          //tls_watchdog_clr ();
        }
    }


}
