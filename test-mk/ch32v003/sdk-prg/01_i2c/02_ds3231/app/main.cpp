/*
  источник
  https://github.com/openwch/ch32v003.git
*/

#include <cstring>

#include "debug.h"

#include "CShell.hpp"

#include "at24cxxx.h"

#include "ds3231.h"

static ds3231_t ds3231_dev;
static i2c_param_t user_i2c;

int init_at24c32 (int argc, char **argv);

int init_ds3231 (int argc, char **argv);

int ds3231_cmd_get (int argc, char **argv);

static const shell_command_t shell_commands[] = {
  //{ "init", "Setup a particular SPI configuration", cmd_init },
  { "time-get", "get cur time", ds3231_cmd_get },
  //  { "time-set", "set time from iso-date-str YYYY-MM-DDTHH:mm:ss",
  //    ds3231_cmd_set },
  { "init", "init ds3231", init_ds3231 },
  { NULL, NULL, NULL }
};

#ifndef AT24CXXX_ERASE
#define AT24CXXX_ERASE (0)
#endif

#define AT24CXXX_EEPROM_SIZE (AT24C32_EEPROM_SIZE)
#define AT24CXXX_PAGE_SIZE (AT24C32_PAGE_SIZE)
#define AT24CXXX_MAX_POLLS (AT24C32_MAX_POLLS)
#define AT24CXXX_PIN_WP (GPIO_UNDEF)
#define AT24CXXX_ADDR (AT24CXXX_DEF_DEV_ADDR + 7)

#define WRITE_BYTE_POSITION (12U)
#define WRITE_BYTE_CHARACTER 'A'

#define WRITE_POSITION (AT24CXXX_EEPROM_SIZE - 3 * AT24CXXX_PAGE_SIZE - 4)
#define WRITE_CHARACTERS                                                      \
  {                                                                           \
    'B', 'E', 'E', 'R', '4', 'F', 'R', 'E', 'E', '\0'                         \
  }

#define SET_POSITION (AT24CXXX_EEPROM_SIZE - 7 * AT24CXXX_PAGE_SIZE - 4)
#define SET_CHARACTER 'G'
#define SET_LEN (20U)

int
init_at24c32 (int argc, char **argv)
{

  puts ("init at24cxxx\r\n");

  at24cxxx_t at24cxxx_dev;
  int check;

  printf ("EEPROM size: %u byte\r\n", AT24CXXX_EEPROM_SIZE);
  printf ("Page size  : %u byte\r\n", AT24CXXX_PAGE_SIZE);

  user_i2c = {
    .i2c_scl = PC_02,   /* */
    .i2c_sda = PC_01,   /* */
    .i2c_freq = 400000, /* частота i2c в герцах */
    .hw_wire = false,
  };

  at24cxxx_params_t user_data = { .i2c = &user_i2c,
                                  .pin_wp = AT24CXXX_PIN_WP,
                                  .eeprom_size = AT24CXXX_EEPROM_SIZE,
                                  .dev_addr = AT24CXXX_ADDR,
                                  .page_size = AT24CXXX_PAGE_SIZE,
                                  .max_polls = AT24CXXX_MAX_POLLS };

  /* initialize the i2c */
  i2c_init (&user_i2c);

  /* Test: Init */
  check = at24cxxx_init (&at24cxxx_dev, &user_data);
  if (check != AT24CXXX_OK)
    {
      printf ("[FAILURE] at24cxxx_init: (%d)\r\n", check);
      return 1;
    }
  else
    {
      puts ("[SUCCESS] at24cxxx_init\r\n");
    }

  return 0;
}

int
init_ds3231 (int argc, char **argv)
{

  init_at24c32 (argc, argv);

  int res;

  puts ("DS3231 init\r\n");

  user_i2c = {
    .i2c_scl = PC_02,   /* */
    .i2c_sda = PC_01,   /* */
    .i2c_freq = 400000, /* частота i2c в герцах */
    .hw_wire = false,
  };

  ds3231_params_t par
      = { .bus = &user_i2c,
          .opt = DS3231_OPT_BAT_ENABLE /* | DS3231_OPT_INTER_ENABLE */ };

  /* initialize the i2c */
  i2c_init (&user_i2c);
  Delay_Ms (500);

  /* initialize the device */
  res = ds3231_init (&ds3231_dev, &par);
  if (res != 0)
    {
      puts ("error: unable to init DS3231 [I2C initialization error]\r\n");
      return -1;
    }
  else
    {
      puts ("[SUCCESS] ds3231_init\r\n");
    }

  Delay_Ms (500);

  ds3231_cmd_get (argc, argv);

  return 0;
}

int
ds3231_cmd_get (int argc, char **argv)
{
  struct tm tblock;

  ds3231_get_time (&ds3231_dev, &tblock);
  // получаем текущее время
  if (tblock.tm_year > 80)
    printf ("cur time %d.%d.%d %d:%d:%d \r\n", tblock.tm_year + 1900,
            tblock.tm_mon + 1, tblock.tm_mday, tblock.tm_hour, tblock.tm_min,
            tblock.tm_sec);

  return 0;
}

int
main (void)
{
  NVIC_PriorityGroupConfig (NVIC_PriorityGroup_1);
  SystemCoreClockUpdate ();
  Delay_Init ();
  // USART_Printf_Init (115000);
  CShell oCShell = CShell (shell_commands);
  printf ("SystemClk1:%d\r\n", SystemCoreClock);
  printf ("enter help for usage\r\n");

  while (1)
    {
      oCShell.Idle ();
      Delay_Ms (1000);
      ds3231_cmd_get (0, NULL);
    }
}
