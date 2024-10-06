/*****************************************************************************
 *
 * File Name : main.c
 *
 * Description: main
 *
 * Date : 2022-12-21
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wm_type_def.h"

#include "wm_cpu.h"
#include "wm_rtc.h"
#include "wm_timer.h"
#include "wm_uart.h"
#include "wm_watchdog.h"

#include "wm_gpio.h"
#include "wm_gpio_afsel.h"
#include "wm_osal.h"

#include "mod1/at24cxxx.h"
#include "mod1/ds3231.h"

#include "max7219c.h"
#include "shell.h"

#define USER_APP1_TASK_SIZE 2048
static OS_STK UserApp1TaskStk[USER_APP1_TASK_SIZE];
#define USER_APP1_TASK_PRIO 32

static at24cxxx_t at24cxxx_dev;
static ds3231_t _dev;

#include "at24c32_util.h"
#include "ds3231_util.h"

static const shell_command_t shell_commands[] = {
  //{ "init", "Setup a particular SPI configuration", cmd_init },
  { "time-get", "get current time", _cmd_get },
  { "time-set", "set time from iso-date-str YYYY-MM-DDTHH:mm:ss", _cmd_set },
  { "int-get", "get current intensity", _int_get },
  { "int-set", "set intensity 0-254", _int_set },
  { NULL, NULL, NULL }
};

#include "console_util.h"

void
user_app1_task (void *sdata)
{

  int res;

  puts ("DS3231 RTC test\n");

  tls_watchdog_init (30 * 1000 * 1000); // u32 usec microseconds, около 30 сек
  tls_os_task_create (NULL, NULL, console_task, NULL,
                      (void *)UserApp2TaskStk, /* task's stack start address */
                      console_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      console_TASK_PRIO, 0);

  i2c_param_t user_i2c = {
    .i2c_freq = 400000,     /* частота i2c в герцах */
    .i2c_scl = WM_IO_PA_01, /* WM_IO_PA_01 or WM_IO_PB_20 */
    .i2c_sda = WM_IO_PA_04, /* WM_IO_PA_04 or WM_IO_PB_19 */
  };

  at24cxxx_params_t user_at24 = { .i2c = &user_i2c,
                                  .pin_wp = AT24CXXX_PIN_WP,
                                  .eeprom_size = AT24CXXX_EEPROM_SIZE,
                                  .dev_addr = AT24CXXX_ADDR,
                                  .page_size = AT24CXXX_PAGE_SIZE,
                                  .max_polls = AT24CXXX_MAX_POLLS };

  /* initialize the i2c */
  i2c_init (&user_i2c);

  /* Test: Init */
  int check = at24cxxx_init (&at24cxxx_dev, &user_at24);
  if (check != AT24CXXX_OK)
    {
      printf ("[FAILURE] at24cxxx_init: (%d)\n", check);
    }
  else
    {
      puts ("[SUCCESS] at24cxxx_init");
    }

#define CFG_intensity_BYTE 1

  u8 u8_value=b_intensity;
  /*
  if (ReadByte (CFG_intensity_BYTE, &u8_value) == 0)
    {
      if (u8_value != 255)
        {
          printf ("Read intensity: (%d)\n", u8_value);
          b_intensity = u8_value;
        }
    }
  */

  ds3231_params_t par
      = { .bus = &user_i2c,
          .opt = DS3231_OPT_BAT_ENABLE /* | DS3231_OPT_INTER_ENABLE */ };

  /* initialize the i2c */
  i2c_init (&user_i2c);
  tls_os_time_delay (HZ);

  /* initialize the device ds3231_init */
  res = ds3231_init (&_dev, &par);
  if (res != 0)
    {
      puts ("error: unable to initialize DS3231 [I2C initialization error]");
      return;
    }

  SPI_cfg_t spi_max7219 = {
    .spi_cs = WM_IO_PB_23, /* */
    .spi_ck = WM_IO_PB_24, /*      ck -> sck Clock pin */
    .spi_di = WM_IO_PB_03, /* master miso di <- miso slave, на макетке board
                              HLK-W801-KIT-V1.1 работает только WM_IO_PB_03  */
    .spi_do = WM_IO_PB_26, /* master mosi do -> mosi slave */

    .mode = TLS_SPI_MODE_0,      // TLS_SPI_MODE_0 ... TLS_SPI_MODE_3
    .cs_active = TLS_SPI_CS_LOW, // TLS_SPI_CS_LOW TLS_SPI_CS_HIGH
    .fclk = 400000,           // between TLS_SPI_FCLK_MIN and TLS_SPI_FCLK_MAX
    .type = SPI_DMA_TRANSFER, // SPI_BYTE_TRANSFER SPI_DMA_TRANSFER
                              // SPI_WORD_TRANSFER
  };
  /* initialize the device SPI */
  res = SPI_Settings (&spi_max7219);
  if (res != 0)
    {
      puts ("error: unable to initialize SPI");
      return;
    }
  clear ();

  struct tm tblock;
  ds3231_get_time (&_dev, &tblock);
  printf (" cur time %d.%02d.%02d %02d:%02d:%02d\r\n", tblock.tm_year + 1900,
          tblock.tm_mon + 1, tblock.tm_mday, tblock.tm_hour, tblock.tm_min,
          tblock.tm_sec);
  puts ("enter help for usage\n");

  for (;;) // цикл(1) с
    {
      refresh ();
      ds3231_get_time (&_dev, &tblock);
      // printf (" cur time %d.%02d.%02d %02d:%02d:%02d\r", tblock.tm_year +
      // 1900,
      //         tblock.tm_mon + 1, tblock.tm_mday, tblock.tm_hour,
      //         tblock.tm_min, tblock.tm_sec);
      h_1 = tblock.tm_hour / 10;
      h_2 = tblock.tm_hour % 10;

      m_2 = tblock.tm_min / 10;
      m_1 = tblock.tm_min % 10;

      if (tblock.tm_sec % 2 == 0)
        c_sec = 1;
      else
        c_sec = 0;

      if (u8_value != b_intensity)
        {
          u8_value = b_intensity;
          if (WriteByte (CFG_intensity_BYTE, b_intensity) == 0)
            {
              printf ("Write New intensity: (%d)\n", u8_value);
            }
        }

      tls_os_time_delay (HZ);
    }
}

void
UserMain (void)
{
  printf ("UserMain start \n");
  tls_sys_clk_set (CPU_CLK_240M);

  tls_os_task_create (NULL, NULL, user_app1_task, NULL,
                      (void *)UserApp1TaskStk, /* task's stack start address */
                      USER_APP1_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      USER_APP1_TASK_PRIO, 0);
}
