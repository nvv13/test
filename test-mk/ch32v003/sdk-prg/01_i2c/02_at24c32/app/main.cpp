/*
  источник
  https://github.com/openwch/ch32v003.git
*/

#include <cstring>
#include <stdlib.h>

#include "debug.h"

#include "CShell.hpp"

#include "at24cxxx.h"

CShell *PoCShell = NULL;

int test_at24c32 (int argc, char **argv);

static const shell_command_t shell_commands[] = {
  //{ "init", "Setup a particular SPI configuration", cmd_init },
  { "test", "test EPPROM at24c32", test_at24c32 },
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
test_at24c32 (int argc, char **argv)
{

  PoCShell->oUsart->SendPSZstring ("tests at24cxxx\r\n");

  at24cxxx_t at24cxxx_dev;
  int check;

  PoCShell->oUsart->SendPSZstring ("EEPROM size:");
  PoCShell->oUsart->SendIntToStr (AT24CXXX_EEPROM_SIZE);
  PoCShell->oUsart->SendPSZstring ("\r\n");
  PoCShell->oUsart->SendPSZstring ("Page size  :");
  PoCShell->oUsart->SendIntToStr (AT24CXXX_PAGE_SIZE);
  PoCShell->oUsart->SendPSZstring ("\r\n");

  i2c_param_t user_i2c = {
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
      PoCShell->oUsart->SendPSZstring ("[FAILURE] at24cxxx_init");
      PoCShell->oUsart->SendIntToStr (check);
      PoCShell->oUsart->SendPSZstring ("\r\n");
      return 1;
    }
  else
    {
      PoCShell->oUsart->SendPSZstring ("[SUCCESS] at24cxxx_init\r\n");
    }

    /* erase EEPROM to exclude side effects from prior test runs */
#if AT24CXXX_ERASE
  check = at24cxxx_erase (&at24cxxx_dev);
  if (check != AT24CXXX_OK)
    {
      //      printf ("[FAILURE] at24cxxx_erase: %d (EEPROM size = %" PRId32
      //      ")\r\n",
      //              check, at24cxxx_dev.params.eeprom_size);
      PoCShell->oUsart->SendPSZstring ("[FAILURE] at24cxxx_erase:\r\n");
      return 1;
    }
  else
    {
      PoCShell->oUsart->SendPSZstring ("[SUCCESS] at24cxxx_erase\r\n");
    }
#endif

  /* Test: Write/Read Byte */
  check = at24cxxx_write_byte (&at24cxxx_dev, WRITE_BYTE_POSITION,
                               WRITE_BYTE_CHARACTER);
  if (check != AT24CXXX_OK)
    {
      PoCShell->oUsart->SendPSZstring ("[FAILURE] at24cxxx_write_byte: ");
      PoCShell->oUsart->SendIntToStr (check);
      PoCShell->oUsart->SendPSZstring ("\r\n");
      return 1;
    }
  else
    {
      PoCShell->oUsart->SendPSZstring ("[SUCCESS] at24cxxx_write_byte\r\n");
    }

  uint8_t c;
  check = at24cxxx_read_byte (&at24cxxx_dev, WRITE_BYTE_POSITION, &c);
  if (check < 0)
    {
      PoCShell->oUsart->SendPSZstring ("[FAILURE] at24cxxx_read_byte:");
      PoCShell->oUsart->SendIntToStr (check);
      PoCShell->oUsart->SendPSZstring ("\r\n");
      return 1;
    }
  else
    {
      PoCShell->oUsart->SendPSZstring ("[SUCCESS] at24cxxx_read_byte\r\n");
    }

  if (c != WRITE_BYTE_CHARACTER)
    {
      PoCShell->oUsart->SendPSZstring ("[FAILURE] write_byte/read_byte: ");
      PoCShell->oUsart->SendIntToStr (c);
      PoCShell->oUsart->SendPSZstring (" != ");
      PoCShell->oUsart->SendIntToStr (WRITE_BYTE_CHARACTER);
      PoCShell->oUsart->SendPSZstring ("\r\n");
      return 1;
    }
  else
    {
      PoCShell->oUsart->SendPSZstring ("[SUCCESS] write_byte/read_byte\r\n");
    }

  /* Test: Write */
  uint8_t expected_write_data[] = WRITE_CHARACTERS;

  check = at24cxxx_write (&at24cxxx_dev, WRITE_POSITION, expected_write_data,
                          sizeof (expected_write_data));
  if (check != AT24CXXX_OK)
    {
      //      printf ("[FAILURE] at24cxxx_write: %d (size = %u)\r\n", check,
      //              sizeof (expected_write_data));
      PoCShell->oUsart->SendPSZstring ("[FAILURE] at24cxxx_write");
      PoCShell->oUsart->SendIntToStr (check);
      PoCShell->oUsart->SendPSZstring ("\r\n");
      return 1;
    }
  else
    {
      PoCShell->oUsart->SendPSZstring ("[SUCCESS] at24cxxx_write\r\n");
    }

  /* Test: Read */
  uint8_t actual_write_data[sizeof (expected_write_data)];

  check = at24cxxx_read (&at24cxxx_dev, WRITE_POSITION, actual_write_data,
                         sizeof (actual_write_data));
  if (check != AT24CXXX_OK)
    {
      PoCShell->oUsart->SendPSZstring ("[FAILURE] at24cxxx_read: ");
      PoCShell->oUsart->SendIntToStr (check);
      PoCShell->oUsart->SendPSZstring ("\r\n");
      return 1;
    }
  else
    {
      PoCShell->oUsart->SendPSZstring ("[SUCCESS] at24cxxx_read\r\n");
    }

  if (memcmp (actual_write_data, expected_write_data,
              sizeof (actual_write_data))
      != 0)
    {
      //      printf ("[FAILURE] write/read: (%s != %s)\r\n",
      //      actual_write_data,
      //              expected_write_data);
      PoCShell->oUsart->SendPSZstring ("[FAILURE] write/read\r\n");
      return 1;
    }
  else
    {
      PoCShell->oUsart->SendPSZstring ("[SUCCESS] write/read\r\n");
    }

  /* Test: Set */
  uint8_t expected_set_data[SET_LEN];
  memset (expected_set_data, SET_CHARACTER, SET_LEN);

  uint8_t actual_set_data[sizeof (expected_set_data)];

  check = at24cxxx_set (&at24cxxx_dev, SET_POSITION, SET_CHARACTER, SET_LEN);
  if (check != AT24CXXX_OK)
    {
      // printf ("[FAILURE] at24cxxx_set: %d (size = %u)\r\n", check, SET_LEN);
      PoCShell->oUsart->SendPSZstring ("[FAILURE] at24cxxx_set: ");
      PoCShell->oUsart->SendIntToStr (check);
      PoCShell->oUsart->SendPSZstring ("\r\n");
      return 1;
    }
  else
    {
      PoCShell->oUsart->SendPSZstring ("[SUCCESS] at24cxxx_set\r\n");
    }

  check
      = at24cxxx_read (&at24cxxx_dev, SET_POSITION, actual_set_data, SET_LEN);
  if (check != AT24CXXX_OK)
    {
      PoCShell->oUsart->SendPSZstring ("[FAILURE 1] set/read:\r\n");
      PoCShell->oUsart->SendIntToStr (check);
      PoCShell->oUsart->SendPSZstring ("\r\n");
      return 1;
    }
  else if (memcmp (actual_set_data, expected_set_data, SET_LEN) != 0)
    {
      //      printf ("[FAILURE] set/read: (%s != %s)\r\n", actual_set_data,
      //              expected_set_data);
      PoCShell->oUsart->SendPSZstring ("[FAILURE 2] set/read:\r\n");
      return 1;
    }
  else
    {
      PoCShell->oUsart->SendPSZstring ("[SUCCESS] set/read\r\n");
    }

  PoCShell->oUsart->SendPSZstring ("Finished tests at24cxxx\r\n");

  return 0;
}

int
main (void)
{
  NVIC_PriorityGroupConfig (NVIC_PriorityGroup_1);
  SystemCoreClockUpdate ();
  Delay_Init ();
  CShell oCShell = CShell (shell_commands);
  PoCShell = &oCShell;
  oCShell.oUsart->SendPSZstring ("SystemClk1:");
  oCShell.oUsart->SendIntToStr (SystemCoreClock);
  oCShell.oUsart->SendPSZstring ("\r\n");

  oCShell.oUsart->SendPSZstring ("enter help for usage\r\n");

  while (1)
    {
      oCShell.Idle ();
    }
}
