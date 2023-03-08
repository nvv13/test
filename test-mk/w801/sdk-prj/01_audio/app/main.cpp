/*****************************************************************************
 *
 * File Name : main.cpp
 *
 * Description: use C and C++ mix
 *    https://isocpp.org/wiki/faq/mixing-c-and-cpp
 *
 * Date : 2023-02-16
 *****************************************************************************/

#if defined(__cplusplus)
extern "C"
{
#endif

#include "wm_type_def.h"
#include <stdio.h>
#include <stdlib.h>

#include "wm_cpu.h"
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
  //#include "wm_regs.h"

#include "w_wifi.h"

#include "decode_cmd.h"

#if defined(__cplusplus)
}
#endif

#include "au2io/Audio.h"

#define DEMO_TASK_SIZE 2048
static OS_STK DemoTaskStk[DEMO_TASK_SIZE];
#define DEMO_TASK_PRIO 32

#define DEMO_SOCK_S_TASK_SIZE 1024
static OS_STK sock_s_task_stk[DEMO_SOCK_S_TASK_SIZE];
#define DEMO_SOCK_S_PRIO (DEMO_TASK_PRIO + 1)

u8 u8_wait_start_ota_upgrade = 0;

extern "C" void
demo_console_task (void *sdata)
{

//  Audio audio = Audio (false, 3, I2S_NUM_0); // #99

  u8 u8_wifi_state = 0;
  for (;;) // цикл(1) с подсоединением к wifi
    {
      while (u8_wifi_state == 0)
        {
          printf ("trying to connect wifi\n");
          if (u8_wifi_state == 0
              && demo_connect_net ("bred1", "9115676369") == WM_SUCCESS)
            u8_wifi_state = 1;
          else
            {
              tls_os_time_delay (5000);
            }
        }

//      audio.setPinout (I2S_BCLK, I2S_LRC, I2S_DOUT);
//      audio.setVolume (12); // 0...21

      //    audio.connecttoFS(SD, "/320k_test.mp3");
      //    audio.connecttoFS(SD, "test.wav");
      //    audio.connecttohost("http://www.wdr.de/wdrlive/media/einslive.m3u");
      //    audio.connecttohost("http://macslons-irish-pub-radio.com/media.asx");
      //    audio.connecttohost("http://mp3.ffh.de/radioffh/hqlivestream.aac");
      //    // 128k aac
//      audio.connecttohost (
//          "http://mp3.ffh.de/radioffh/hqlivestream.mp3"); //  128k mp3

      while (u8_wifi_state)
        {
//          audio.loop ();
          /*    if(Serial.available()){ // put streamURL in serial monitor
                  audio.stopSong();
                  String r=Serial.readString(); r.trim();
                  if(r.length()>5) audio.connecttohost(r.c_str());
                  log_i("free heap=%i", ESP.getFreeHeap());
              }*/
          if (u8_wait_start_ota_upgrade)
            {
//              audio.stopSong ();
              u8_wait_start_ota_upgrade = 0;
              printf ("OTA upgrade start, try = " OTA_PATH_FILE "\n");
              t_http_fwup ((char *)OTA_PATH_FILE);
              printf ("OTA upgrade stop, error\n"); //если в это место попало,
                                                    //значит какая-то ошибка
                                                    //случилась и прошивка не
                                                    //скачалась
            }
        }
    }
}

// optional
/*
void
audio_info (const char *info)
{
  printf ("info        %s\n", info);
}
void
audio_id3data (const char *info)
{ // id3 metadata
  printf ("id3data     %s\n", info);
}
void
audio_eof_mp3 (const char *info)
{ // end of file
  printf ("eof_mp3     %s\n", info);
}
void
audio_showstation (const char *info)
{
  printf ("station     %s\n", info);
}
void
audio_showstreamtitle (const char *info)
{
  printf ("streamtitle %s\n", info);
}
void
audio_bitrate (const char *info)
{
  printf ("bitrate     %s\n", info);
}
void
audio_commercial (const char *info)
{ // duration in sec
  printf ("commercial  %s\n", info);
}
void
audio_icyurl (const char *info)
{ // homepage
  printf ("icyurl      %s\n", info);
}
void
audio_lasthost (const char *info)
{ // stream URL played
  printf ("lasthost    %s\n", info);
}

*/

extern "C" void
sock_s_task (void *sdata)
{
  while (1)
    {
      int i_port = 5555;
      printf ("create_socket_server  i_port=%d\n", i_port);
      create_socket_server (i_port);
    }
}

extern "C" int
UserMain (void)
{

  tls_sys_clk_set (CPU_CLK_240M); // нам мужно 240MHz, под это всё подогнано

  tls_os_task_create (NULL, NULL, demo_console_task, NULL,
                      (u8 *)DemoTaskStk, /* task's stack start address */
                      DEMO_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      DEMO_TASK_PRIO, 0);

  tls_os_task_create (NULL, NULL, sock_s_task, NULL,
                      (u8 *)sock_s_task_stk, /* task's stack start address */
                      DEMO_SOCK_S_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      DEMO_SOCK_S_PRIO, 0);

  return 0;
}
