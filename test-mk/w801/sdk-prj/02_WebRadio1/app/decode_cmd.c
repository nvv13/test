#include "decode_cmd.h"
#include "my_recognize.h"
#include "w_flash_cfg.h"
#include "wm_rtc.h"
#include "wm_type_def.h"
#include <stdio.h>
#include <string.h>

#include "mod1/VS1053.h"

int
fast_decode_cmd (char *sock_rx, int len)
{
  int i_len_ret = len;

  if (strncmp ((char *)sock_rx, "time", 4) == 0)
    {
      struct tm tblock;
      tls_get_rtc (&tblock);
      extern u32 total_mem_size;
      //не проверил, сколько буфер, а он 1024 байт
      i_len_ret = sprintf (
          sock_rx, "date %d.%02d.%02d %02d:%02d:%02d total_mem_size=%d",
          tblock.tm_year + 1900, tblock.tm_mon + 1, tblock.tm_mday,
          tblock.tm_hour, tblock.tm_min, tblock.tm_sec, total_mem_size);
    }

  if (strncmp ((char *)sock_rx, "upgrade", 7) == 0)
    {
      i_len_ret
          = sprintf (sock_rx, "OTA upgrade start, try = " OTA_PATH_FILE " \n");
      extern u8 u8_wait_start_ota_upgrade;
      u8_wait_start_ota_upgrade = 1;
      VS1053_stop_PlayMP3 ();
    }

  if (strncmp ((char *)sock_rx, "help", 4) == 0)
    {
      struct tm tblock;
      tls_get_rtc (&tblock);
      i_len_ret = sprintf (
          sock_rx, // внимание, буффер всего лишь 1024 байт, проверяй чтоб
                   // вошло, или увеличь буффер
          "help - данная справка (flash ver-" VERSION_FLASH ")\n"
          "time - время и прочие состояния\n"
          "upgrade - обновить прошивку по OTA, лезет на " OTA_PATH_FILE "\n");
    }

  return i_len_ret;
}
