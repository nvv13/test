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

#include <stdio.h>
#include <stdlib.h>
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
  //#include "wm_regs.h"

#if defined(__cplusplus)
}
#endif

#include "au2io/Audio.h"


extern "C" int
UserMain (void)
{

  Audio audio =  Audio (false, 3, I2S_NUM_0); // #99

  audio.setPinout (I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume (12); // 0...21

  //    audio.connecttoFS(SD, "/320k_test.mp3");
  //    audio.connecttoFS(SD, "test.wav");
  //    audio.connecttohost("http://www.wdr.de/wdrlive/media/einslive.m3u");
  //    audio.connecttohost("http://macslons-irish-pub-radio.com/media.asx");
  //    audio.connecttohost("http://mp3.ffh.de/radioffh/hqlivestream.aac"); //
  //    128k aac
  audio.connecttohost (
      "http://mp3.ffh.de/radioffh/hqlivestream.mp3"); //  128k mp3

  while (1)
    {
      audio.loop ();
      /*    if(Serial.available()){ // put streamURL in serial monitor
              audio.stopSong();
              String r=Serial.readString(); r.trim();
              if(r.length()>5) audio.connecttohost(r.c_str());
              log_i("free heap=%i", ESP.getFreeHeap());
          }*/
    }

  return 0;
}

// optional
//void audio_info(const char *info){
//    printf ("info        %s\n", info);
//}
