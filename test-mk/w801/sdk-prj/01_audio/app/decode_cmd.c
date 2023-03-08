#include "decode_cmd.h"
//#include "my_recognize.h"
#include "wm_rtc.h"
#include "wm_type_def.h"
#include <stdio.h>
#include <string.h>

int
fast_decode_cmd (char *sock_rx, int len)
{
  int i_len_ret = len;

  if (strncmp ((char *)sock_rx, "upgrade", 7) == 0)
    {
      i_len_ret
          = sprintf (sock_rx, "OTA upgrade start, try = " OTA_PATH_FILE " \n");
      extern u8 u8_wait_start_ota_upgrade;
      u8_wait_start_ota_upgrade = 1;
    }

  if (strncmp ((char *)sock_rx, "help", 4) == 0)
    {
      struct tm tblock;
      tls_get_rtc (&tblock);
      i_len_ret = sprintf (
          sock_rx, // !!!внимание!!!, буффер всего лишь 1024 байт, проверяй
                   // чтоб вошло, или увеличь буффер
          "help - данная справка (flash ver-" VERSION_FLASH ")\n"
          "upgrade - обновить прошивку по OTA, лезет на " OTA_PATH_FILE "\n");
    }

  return i_len_ret;
}
