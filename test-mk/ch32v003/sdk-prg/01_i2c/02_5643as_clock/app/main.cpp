/*
  источник
  https://github.com/openwch/ch32v003.git
*/

#include <stdlib.h>

#include <cstring>

#include "debug.h"

#include "CShell.hpp"

#include "lcd5643.h"

#include "at24cxxx.h"

#include "ds3231.h"

static ds3231_t ds3231_dev;
static i2c_param_t user_i2c;
static CShell *PoCShell = NULL;
static at24cxxx_t at24c32_dev;

#include "at24c32_util.h"
#include "ds3231_util.h"

int init_at24c32 (int argc, char **argv);
int init_ds3231 (int argc, char **argv);


static const shell_command_t shell_commands[] = {
  //{ "init", "Setup a particular SPI configuration", cmd_init },
  { "tget", "get cur time", ds3231_cmd_get },
  { "tset", "set time from iso-date-str YYYY-MM-DDTHH:mm:ss", ds3231_cmd_set },
  { "aget", "get aging offset", clock_aging_get },
  { "aset", "set aging offset", clock_aging_set },
  { NULL, NULL, NULL }
};

#define AT24CXXX_EEPROM_SIZE (AT24C32_EEPROM_SIZE)
#define AT24CXXX_PAGE_SIZE (AT24C32_PAGE_SIZE)
#define AT24CXXX_MAX_POLLS (AT24C32_MAX_POLLS)
#define AT24CXXX_PIN_WP (GPIO_UNDEF)
#define AT24CXXX_ADDR (AT24CXXX_DEF_DEV_ADDR + 7)

int
init_at24c32 (int argc, char **argv)
{

  PoCShell->oUsart->SendPSZstring ("init at24c32\r\n");

  int check;

  PoCShell->oUsart->SendPSZstring ("EEPROM size:");
  PoCShell->oUsart->SendIntToStr (AT24CXXX_EEPROM_SIZE);
  PoCShell->oUsart->SendPSZstring ("\r\n");
  PoCShell->oUsart->SendPSZstring ("Page size  :");
  PoCShell->oUsart->SendIntToStr (AT24CXXX_PAGE_SIZE);
  PoCShell->oUsart->SendPSZstring ("\r\n");

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
  check = at24cxxx_init (&at24c32_dev, &user_data);
  if (check != AT24CXXX_OK)
    {
      PoCShell->oUsart->SendPSZstring ("[FAILURE] at24c32 init");
      PoCShell->oUsart->SendIntToStr (check);
      PoCShell->oUsart->SendPSZstring ("\r\n");
      return 1;
    }
  else
    {
      PoCShell->oUsart->SendPSZstring ("[SUCCESS] at24c32 init\r\n");
    }

  return 0;
}

int
init_ds3231 (int argc, char **argv)
{

  init_at24c32 (argc, argv);

  int res;

  PoCShell->oUsart->SendPSZstring ("DS3231 init\r\n");

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
      PoCShell->oUsart->SendPSZstring (
          "error: unable to init DS3231 [I2C initialization error]\r\n");
      return -1;
    }
  else
    {
      PoCShell->oUsart->SendPSZstring ("[SUCCESS] ds3231_init\r\n");
    }

  Delay_Ms (500);

  ds3231_cmd_get (argc, argv);

  return 0;
}

static volatile bool l_flag = false;

int
main (void)
{
  NVIC_PriorityGroupConfig (NVIC_PriorityGroup_1);
  SystemCoreClockUpdate ();
  Delay_Init ();
  // USART_Printf_Init (115000);
  CShell oCShell = CShell (shell_commands);
  PoCShell = &oCShell;
  PoCShell->oUsart->SendPSZstring ("SystemClk1:");
  PoCShell->oUsart->SendIntToStr (SystemCoreClock);
  PoCShell->oUsart->SendPSZstring ("\r\n");
  init_at24c32 (0, NULL);
  init_ds3231 (0, NULL);
  lcd5643_init_pin (MODE_COMMON_CATHODE);
  PoCShell->oUsart->SendPSZstring ("enter help for usage\r\n");
  int i_cnt_disp = 0;
  while (1)
    {
      oCShell.Idle ();
      Delay_Ms (1);
      if (i_cnt_disp++ > 999 && i_5643_out<5)
        {
          i_cnt_disp = 0;
          struct tm tblock;
          ds3231_get_time (&ds3231_dev, &tblock);
          // получаем текущее время
          if (tblock.tm_year > 80)
            {
              i_5643_hour = tblock.tm_hour;
              i_5643_min = tblock.tm_min;
              i_5643_sec_state = tblock.tm_sec % 2;
            }
        }
      lcd5643_update_disp ();
    }
}
