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

#include "wm_gpio.h"
#include "wm_gpio_afsel.h"
#include "wm_osal.h"

#include "mod1/UTFT.h"
#include "mod1/u_jpeg.h"

extern uint8_t SmallFont[]; // подключаем маленький шрифт
extern uint8_t BigFont[];   // подключаем большой шрифт
extern uint8_t
    SevenSegNumFont[]; // подключаем шрифт имитирующий семисегментный индикатор
extern uint8_t SmallSymbolFont[];

#include "ds3231.h"
#define ISOSTR_LEN (20U)

#define USER_APP1_TASK_SIZE 2048
static OS_STK UserApp1TaskStk[USER_APP1_TASK_SIZE];
#define USER_APP1_TASK_PRIO 32

static ds3231_t _dev;

/* parse ISO date string (YYYY-MM-DDTHH:mm:ss) to struct tm */
static int
_tm_from_str (const char *str, struct tm *time)
{
  char tmp[5];

  if (strlen (str) != ISOSTR_LEN - 1)
    {
      return -1;
    }
  if ((str[4] != '-') || (str[7] != '-') || (str[10] != 'T')
      || (str[13] != ':') || (str[16] != ':'))
    {
      return -1;
    }

  memset (time, 0, sizeof (struct tm));

  memcpy (tmp, str, 4);
  tmp[4] = '\0';
  str += 5;
  time->tm_year = atoi (tmp) - 1900;

  memcpy (tmp, str, 2);
  tmp[2] = '\0';
  str += 3;
  time->tm_mon = atoi (tmp) - 1;

  memcpy (tmp, str, 2);
  str += 3;
  time->tm_mday = atoi (tmp);

  memcpy (tmp, str, 2);
  str += 3;
  time->tm_hour = atoi (tmp);

  memcpy (tmp, str, 2);
  str += 3;
  time->tm_min = atoi (tmp);

  memcpy (tmp, str, 2);
  time->tm_sec = atoi (tmp);

  return 0;
}

static int
_cmd_get (int argc, char **argv)
{
  (void)argc;
  (void)argv;
  char dstr[ISOSTR_LEN];

  struct tm time;
  ds3231_get_time (&_dev, &time);

  size_t pos = strftime (dstr, ISOSTR_LEN, "%Y-%m-%dT%H:%M:%S", &time);
  dstr[pos] = '\0';
  printf ("The current time is: %s\n", dstr);

  return 0;
}

static int
_cmd_set (int argc, char **argv)
{
  if (argc != 2)
    {
      printf ("usage: %s <iso-date-str YYYY-MM-DDTHH:mm:ss>\n", argv[0]);
      return 1;
    }

  if (strlen (argv[1]) != (ISOSTR_LEN - 1))
    {
      puts ("error: input date string has invalid length");
      return 1;
    }

  struct tm target_time;
  int res = _tm_from_str (argv[1], &target_time);
  if (res != 0)
    {
      puts ("error: unable do parse input date string");
      return 1;
    }

  ds3231_set_time (&_dev, &target_time);

  printf ("success: time set to %s\n", argv[1]);
  return 0;
}

void
user_app1_task (void *sdata)
{

  int res;

  puts ("DS3231 RTC test\n");

  i2c_param_t user_i2c = {
    .i2c_freq = 100000,     /* частота i2c в герцах */
    .i2c_scl = WM_IO_PA_01, /* WM_IO_PA_01 or WM_IO_PB_20 */
    .i2c_sda = WM_IO_PA_04, /* WM_IO_PA_04 or WM_IO_PB_19 */
  };

  ds3231_params_t par = { .bus = &user_i2c, .opt = DS3231_OPT_BAT_ENABLE /* | DS3231_OPT_INTER_ENABLE */ };

  /* initialize the device */
  res = ds3231_init (&_dev, &par);
  if (res != 0)
    {
      puts ("error: unable to initialize DS3231 [I2C initialization error]");
      return;
    }

  struct tm tblock;
  ds3231_get_time (&_dev, &tblock);
  //            tls_get_rtc (&tblock); // получаем текущее время
  printf (" cur time %d.%02d.%02d %02d:%02d:%02d\n",
                      tblock.tm_year + 1900, tblock.tm_mon + 1, tblock.tm_mday,
                      tblock.tm_hour, tblock.tm_min, tblock.tm_sec);

  printf ("init TFT01_18SP 128x160\n");

  // подключаем библиотеку UTFT
  UTFT_UTFT (TFT01_18SP,
             (u8)NO_GPIO_PIN // WM_IO_PB_17  //RS  SDA
             ,
             (u8)NO_GPIO_PIN // WM_IO_PB_15  //WR  SCL
             ,
             WM_IO_PB_23 //(u8)NO_GPIO_PIN // WM_IO_PB_14  //CS  CS
             ,
             (u8)WM_IO_PB_21 // RST reset RES
             ,
             (u8)WM_IO_PB_22 // SER => DC !
             ,
             // 120000000
             60000000
             /* spi_freq(Герц) для 5 контактных SPI дисплеев
                (где отдельно ножка комманда/данные)
             програмируеться HW SPI на ножки (предопред)
                 wm_spi_cs_config (WM_IO_PB_14);
                 wm_spi_ck_config (WM_IO_PB_15);
                 wm_spi_di_config (WM_IO_PB_16);
                 wm_spi_do_config (WM_IO_PB_17);
             но, можно отказаться от HW SPI в пользу Soft SPI
             установив spi_freq=0
             эмуляции SPI, это удобно для разных ножек

    максимально, частота spi_freq = 20000000 (20MHz)
        но!      если spi_freq > 20000000 тогда работает spi SDIO
        частоту можно ставить от 21000000 до 120000000 герц (работает при
    240Mhz тактовой) контакты: WM_IO_PB_06 CK   -> SCL
                               WM_IO_PB_07 CMD  -> MOSI
           */
  );
  /*
  ---- ------
  W801 LCD
  ---- ------
  3.3v bl    подсветка
  3.3v vcc
  gnd  gnd
  PB21 res   RESET сброс
  PB23 cs    CS выбор чипа
  PB22 dc    комманда/данные
  PB07 sda   данные
  PB06 sck   синхросигнал
  ---- ------
  W801 LCD
  ---- ------


  */

  UTFT_InitLCD (LANDSCAPE); // инициируем дисплей
  // UTFT_InitLCD (PORTRAIT);

  UTFT_clrScr (); // стираем всю информацию с дисплея
  UTFT_setFont (SmallFont); // устанавливаем шрифт
  char msg[100];
  msg[0] = 0;

  while (1)
    { //
      ds3231_get_time (&_dev, &tblock);
      //            tls_get_rtc (&tblock); // получаем текущее время
      printf (" cur time %d.%02d.%02d %02d:%02d:%02d\n",
                      tblock.tm_year + 1900, tblock.tm_mon + 1, tblock.tm_mday,
                      tblock.tm_hour, tblock.tm_min, tblock.tm_sec);
      tls_os_time_delay (HZ);

    } //
}

void
UserMain (void)
{
  printf ("UserMain start");
  tls_sys_clk_set (CPU_CLK_240M);

  tls_os_task_create (NULL, NULL, user_app1_task, NULL,
                      (void *)UserApp1TaskStk, /* task's stack start address */
                      USER_APP1_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      USER_APP1_TASK_PRIO, 0);
}
