#include <string.h>
#include <stdio.h>
#include "wm_rtc.h"
#include "wm_type_def.h"
#include "w_flash_cfg.h"
#include "decode_cmd.h"


int fast_decode_cmd(char* sock_rx, int len)
{
int i_len_ret=len;

if (len > 5 &&	strncmp((char *)sock_rx, "light", 5) == 0) {
 extern u16 i_light;
 int i_out = atoi((char *)sock_rx + 5);
 if(i_out>4 && i_out<2001) 
  {
  i_light=i_out;
  i_len_ret=sprintf(sock_rx, "ok, set i_light=%d", i_out);
  }
  else
  i_len_ret=sprintf(sock_rx, "error value=%d, cur i_light=%d", i_out,i_light);
 }



if (len > 2 &&	strncmp((char *)sock_rx, "dr", 2) == 0) {
 int i_out = atoi((char *)sock_rx + 2);
 extern u16 i_swith;
  if(i_out==0)
   i_len_ret=sprintf(sock_rx, "ok, get i_swith=%d", i_swith);
   else
   {
   i_swith=i_out;
   i_len_ret=sprintf(sock_rx, "ok, set i_swith=%d", i_swith);
   }
 }


if (strncmp((char *)sock_rx, "time", 4) == 0) {
    struct tm tblock;
    tls_get_rtc(&tblock);
    extern struct tm t_last_start_main_task;
    //не проверил, сколько буфер, а он 512 байт
    i_len_ret=sprintf(sock_rx,"date %d.%02d.%02d %02d:%02d:%02d  \n"
                              "t_last_start_main_task=%d.%02d.%02d %02d:%02d:%02d\n"
           ,tblock.tm_year+1900
           ,tblock.tm_mon+1
           ,tblock.tm_mday
           ,tblock.tm_hour
           ,tblock.tm_min
           ,tblock.tm_sec
           ,t_last_start_main_task.tm_year+1900
           ,t_last_start_main_task.tm_mon+1
           ,t_last_start_main_task.tm_mday
           ,t_last_start_main_task.tm_hour
           ,t_last_start_main_task.tm_min
           ,t_last_start_main_task.tm_sec
         );


 }

if (strncmp((char *)sock_rx, "upgrade", 7) == 0) {
    i_len_ret=sprintf(sock_rx,
      "OTA upgrade start, try = " OTA_PATH_FILE " \n"
       );
    extern u8 u8_wait_start_ota_upgrade;
    u8_wait_start_ota_upgrade=1;
 }


if (strncmp((char *)sock_rx, "help", 4) == 0) {
    i_len_ret=sprintf(sock_rx,  // внимание, буффер всего лишь 1024 байт, проверяй чтоб вошло, или увеличь буффер
      "help - данная справка (flash ver-"VERSION_FLASH")\n"
      "time - время и прочие состояния\n"
      "lightXXX - установить/узнать яркость индикатора, где XXX число от 5 до 1999, если 0 то выдаст текущее значение яркости\n"        
      "drX - режим, где X, \n"
      "upgrade - обновить прошивку по OTA, лезет на "
      OTA_PATH_FILE 
      "\n"
       );


 }

return i_len_ret;
}
