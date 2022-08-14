#include <string.h>
#include <stdio.h>
#include "wm_type_def.h"
#include "w_flash_cfg.h"
#include "decode_cmd.h"
#include "my_recognize.h"


int fast_decode_cmd(char* sock_rx, int len)
{
int i_len_ret=len;
if (len > 5 &&	strncmp((char *)sock_rx, "light", 5) == 0) {
 int i_out = atoi((char *)sock_rx + 5);
 extern u16 i_max_out;
 if(i_out>4 && i_out<2001) 
  {
  i_max_out=i_out;
  flash_cfg_store_u16(i_max_out, MEM_CELL_FROM_LIGTH_LEVEL);
  i_len_ret=sprintf(sock_rx, "ok, set i_max_out=%d", i_out);
  }
  else
  i_len_ret=sprintf(sock_rx, "error value=%d, cur i_max_out=%d", i_out,i_max_out);
 }


if (strncmp((char *)sock_rx, "time", 4) == 0) {
    struct tm tblock;
    tls_get_rtc(&tblock);
    struct tm t_last_query=my_recognize_ret_t_last_query();
    extern struct tm t_last_start_main_task;
    extern u32 total_mem_size;
    //не проверил, сколько буфер, а он 1024 байт
    i_len_ret=sprintf(sock_rx,"date %d.%02d.%02d %02d:%02d:%02d  currrent temperature=%d,%d  \n"
                              "last query=%d.%02d.%02d %02d:%02d:%02d\n"
                              "t_last_start_main_task=%d.%02d.%02d %02d:%02d:%02d\n"
                               "total_mem_size=%d"
           ,tblock.tm_year+1900
           ,tblock.tm_mon+1
           ,tblock.tm_mday
           ,tblock.tm_hour
           ,tblock.tm_min
           ,tblock.tm_sec
            ,my_recognize_ret_cur_temperature_sign() * my_recognize_ret_cur_temperature()
            ,my_recognize_ret_cur_temperature_mantissa()
           ,t_last_query.tm_year+1900
           ,t_last_query.tm_mon+1
           ,t_last_query.tm_mday
           ,t_last_query.tm_hour
           ,t_last_query.tm_min
           ,t_last_query.tm_sec
           ,t_last_start_main_task.tm_year+1900
           ,t_last_start_main_task.tm_mon+1
           ,t_last_start_main_task.tm_mday
           ,t_last_start_main_task.tm_hour
           ,t_last_start_main_task.tm_min
           ,t_last_start_main_task.tm_sec
             ,total_mem_size
         );


 }

if (strncmp((char *)sock_rx, "upgrade", 7) == 0) {
    i_len_ret=sprintf(sock_rx,
      "OTA upgrade start, try = " OTA_PATH_FILE " \n"
       );
    extern u8 u8_wait_start_ota_upgrade;
    u8_wait_start_ota_upgrade=1;
 }


if (len > 5 &&	strncmp((char *)sock_rx, "mode=", 5) == 0) {
 int i_out = atoi((char *)sock_rx + 5);
 extern u16 i_mode_global;
 if(i_out>0 && i_out<4) 
  {
  i_mode_global=i_out;
  flash_cfg_store_u16(i_mode_global, MEM_CELL_FROM_GL_MODE);
  i_len_ret=sprintf(sock_rx, "ok, set i_mode_global=%d", i_out);
  }
  else
  i_len_ret=sprintf(sock_rx, "error value=%d, cur i_mode_global=%d", i_out,i_mode_global);
 }

if (strncmp((char *)sock_rx, "help", 4) == 0) {
    struct tm tblock;
    tls_get_rtc(&tblock);
    i_len_ret=sprintf(sock_rx,  // внимание, буффер всего лишь 1024 байт, проверяй чтоб вошло, или увеличь буффер
      "help - данная справка (flash ver-"VERSION_FLASH")\n"
      "time - время и прочие состояния\n"
      "lightXXX - установить/узнать яркость индикатора, где XXX число от 4 до 2000, если 0 то выдаст текущее значение яркости\n"        
      "mode=X - режим, где X, только 1-погода, 2-погода и время, 3-только время\n"
      "upgrade - обновить прошивку по OTA, лезет на "
      OTA_PATH_FILE 
      "\n"
       );


 }

return i_len_ret;
}
