#include "decode_cmd.h"
//#include "my_recognize.h"
#include "w_flash_cfg.h"
#include "wm_rtc.h"
#include "wm_type_def.h"
#include "ws2812b.h"
#include <stdio.h>
#include <string.h>

int
fast_decode_cmd (char *sock_rx, int len)
{
  int i_len_ret = len;
  if (len > 5 && strncmp ((char *)sock_rx, "light", 5) == 0)
    {
      int i_out = atoi ((char *)sock_rx + 5);
      extern u16 i_max_out;
      if (i_out > 0 && i_out < 9)
        {
          i_max_out = i_out;
          flash_cfg_store_u16 (i_max_out, MEM_CELL_FROM_LIGTH_LEVEL);
          i_len_ret = sprintf (sock_rx, "ok, set i_max_out=%d", i_out);
        }
      else
        i_len_ret = sprintf (sock_rx, "error value=%d, cur i_max_out=%d",
                             i_out, i_max_out);
    }

  if (len > 2 && strncmp ((char *)sock_rx, "dr", 2) == 0)
    {
      extern u16 i_swith;
      int i_out = atoi ((char *)sock_rx + 2);
      if (i_out == 0)
        i_len_ret = sprintf (sock_rx, "ok, get i_swith=%d", i_swith);
      else
        {
          i_swith = i_out;
          i_len_ret = sprintf (sock_rx, "ok, set i_swith=%d", i_swith);
        }
    }

  if (strncmp ((char *)sock_rx, "time", 4) == 0)
    {
      extern ws2812b_t dev;
      struct tm tblock;
      tls_get_rtc (&tblock);
      i_len_ret = sprintf (sock_rx,
                           "date %d.%02d.%02d %02d:%02d:%02d\n"
                           "led_numof=%d\n"
                           "data_pin=%d\n"
                           "mode=%d\n"
                           "spi_clk=%d\n"
                           "spi_on_bit=%d\n"
                           "spi_off_bit=%d\n",
                           tblock.tm_year + 1900, tblock.tm_mon + 1,
                           tblock.tm_mday, tblock.tm_hour, tblock.tm_min,
                           tblock.tm_sec, dev.led_numof, (int)dev.data_pin
                           , (int)dev.mode, dev.spi_clk, dev.spi_on_bit
                           , dev.spi_off_bit);
    }

  if (strncmp ((char *)sock_rx, "upgrade", 7) == 0)
    {
      i_len_ret
          = sprintf (sock_rx, "OTA upgrade start, try = " OTA_PATH_FILE " \n");
      extern u8 u8_wait_start_ota_upgrade;
      u8_wait_start_ota_upgrade = 1;
    }

  if (len > 5 && strncmp ((char *)sock_rx, "mode=", 5) == 0)
    {
      int i_out = atoi ((char *)sock_rx + 5);
      extern u8 u8_start_reconfigure;
      extern ws2812b_t dev;
      if (i_out >= 0 && i_out <= 6)
        {
          dev.mode = i_out;
          //flash_cfg_store_u16 (i_mode_global, MEM_CELL_FROM_GL_MODE);
          i_len_ret = sprintf (sock_rx, "ok, set dev.mode=%d", i_out);
          u8_start_reconfigure=1;
        }
      else
        i_len_ret = sprintf (sock_rx, "error value=%d, cur dev.mode=%d",
                             i_out, (u8)dev.mode);
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
          "drX - режим эф.ленты, где X, только 1-, 2-, 3-выкл. \n"
          "mode=Y - режим драйвера ленты, где Y, 0-WS_PIN_MODE, WS_SPI_MODE: 1-3bit, 2-4bit, 3-5bit, 4-6bit, 5-7bit, 6-8bit \n"
          "upgrade - обновить прошивку по OTA, лезет на " OTA_PATH_FILE "\n");
    }

  return i_len_ret;
}
