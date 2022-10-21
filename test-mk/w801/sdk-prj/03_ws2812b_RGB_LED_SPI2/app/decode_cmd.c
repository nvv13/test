#include "decode_cmd.h"
//#include "my_recognize.h"
#include "el_cmd.h"
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
      extern volatile bool changeFlag;
      int i_out = atoi ((char *)sock_rx + 2);
      if (i_out >= 0 && i_out <= 999)
        {
          i_swith = i_out;
          changeFlag = true;
          // flash_cfg_store_u16 (i_mode_global, MEM_CELL_FROM_GL_MODE);
          i_len_ret = sprintf (sock_rx, "ok, set i_swith=%d", i_swith);
        }
      else
        i_len_ret = sprintf (sock_rx, "error value=%d, cur dev.mode=%d", i_out,
                             i_swith);
    }

  if (strncmp ((char *)sock_rx, "time", 4) == 0)
    {
      extern ws2812b_t dev;
      extern u32 u32_US_PER_MS;
      extern u16 i_swith;
      struct tm tblock;
      tls_get_rtc (&tblock);
      i_len_ret = sprintf (sock_rx,
                           "date %d.%02d.%02d %02d:%02d:%02d\n"
                           "led_numof=%d\n"
                           "data_pin=%d\n"
                           "mode=%d\n"
                           "spi_clk=%d\n"
                           "spi_on_bit=%d\n"
                           "spi_off_bit=%d\n"
                           "dr=%d\n"
                           "spd=%d\n",
                           tblock.tm_year + 1900, tblock.tm_mon + 1,
                           tblock.tm_mday, tblock.tm_hour, tblock.tm_min,
                           tblock.tm_sec, dev.led_numof, (int)dev.data_pin,
                           (int)dev.mode, dev.spi_clk, dev.spi_on_bit,
                           dev.spi_off_bit, i_swith, u32_US_PER_MS);
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
          // flash_cfg_store_u16 (i_mode_global, MEM_CELL_FROM_GL_MODE);
          i_len_ret = sprintf (sock_rx, "ok, set dev.mode=%d", i_out);
          u8_start_reconfigure = 1;
        }
      else
        i_len_ret = sprintf (sock_rx, "error value=%d, cur dev.mode=%d", i_out,
                             (u8)dev.mode);
    }

  if (len > 3 && strncmp ((char *)sock_rx, "on=", 3) == 0)
    {
      int i_out = atoi ((char *)sock_rx + 3);
      extern ws2812b_t dev;
      if (i_out >= 0 && i_out <= 8)
        {
          dev.spi_on_bit = i_out;
          // flash_cfg_store_u16 (i_mode_global, MEM_CELL_FROM_GL_MODE);
          i_len_ret = sprintf (sock_rx, "ok, set dev.spi_on_bit=%d", i_out);
        }
      else
        i_len_ret = sprintf (sock_rx, "error value=%d, cur dev.spi_on_bit=%d",
                             i_out, dev.spi_on_bit);
    }

  if (len > 4 && strncmp ((char *)sock_rx, "off=", 4) == 0)
    {
      int i_out = atoi ((char *)sock_rx + 4);
      extern ws2812b_t dev;
      if (i_out >= 0 && i_out <= 8)
        {
          dev.spi_off_bit = i_out;
          // flash_cfg_store_u16 (i_mode_global, MEM_CELL_FROM_GL_MODE);
          i_len_ret = sprintf (sock_rx, "ok, set dev.spi_off_bit=%d", i_out);
        }
      else
        i_len_ret = sprintf (sock_rx, "error value=%d, cur dev.spi_off_bit=%d",
                             i_out, dev.spi_on_bit);
    }

  if (len > 4 && strncmp ((char *)sock_rx, "clk=", 4) == 0)
    {
      int i_out = atoi ((char *)sock_rx + 4);
      extern ws2812b_t dev;
      extern u8 u8_start_reconfigure;
      if (i_out >= 1000 && i_out <= 120000000)
        {
          dev.spi_clk = i_out;
          // flash_cfg_store_u16 (i_mode_global, MEM_CELL_FROM_GL_MODE);
          i_len_ret = sprintf (sock_rx, "ok, set dev.spi_clk=%d", i_out);
          u8_start_reconfigure = 1;
        }
      else
        i_len_ret = sprintf (sock_rx, "error value=%d, cur dev.spi_clk=%d",
                             i_out, dev.spi_clk);
    }

  if (len > 4 && strncmp ((char *)sock_rx, "spd=", 4) == 0)
    {
      int i_out = atoi ((char *)sock_rx + 4);
      extern u32 u32_US_PER_MS;
      if (i_out >= 1 && i_out <= 10000)
        {
          u32_US_PER_MS = i_out;
          // flash_cfg_store_u16 (i_mode_global, MEM_CELL_FROM_GL_MODE);
          i_len_ret = sprintf (sock_rx, "ok, set u32_US_PER_MS=%d", i_out);
          ef_count_delay_STEP ();
        }
      else
        i_len_ret = sprintf (sock_rx, "error value=%d, cur u32_US_PER_MS=%d",
                             i_out, u32_US_PER_MS);
    }

  if (strncmp ((char *)sock_rx, "help", 4) == 0)
    {
      struct tm tblock;
      tls_get_rtc (&tblock);
      i_len_ret = sprintf (
          sock_rx, // !!!внимание!!!, буффер всего лишь 1024 байт, проверяй
                   // чтоб вошло, или увеличь буффер
          "help - данная справка (flash ver-" VERSION_FLASH ")\n"
          "time - время и прочие состояния\n"
          "drX - режим ленты, где X, только 1...999.\n"
          "spd=A - где A - скорость\n"
          "mode=Y - режим драйвера ленты, где Y, 0-PIN_MODE, SPI_MODE: "
          "1-3bit, 2-4bit, 3-5bit, 4-6bit, 5-7bit, 6-8bit\n"
          "on=Z, где Z - бит для 1 в SPI_MODE, можно менять для эксперементов "
          "от 0 до 8\n"
          "off=V,где V - бит для 0\n"
          "clk=K,где K - частота в Герцах для SPI_MODE\n"
          "upgrade - обновить прошивку по OTA, лезет на " OTA_PATH_FILE "\n");
    }

  return i_len_ret;
}
