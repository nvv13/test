/*
  источник
  https://github.com/openwch/ch32v003.git
*/

#include <cstring>

#include "debug.h"

#include "CShell.hpp"

#include "at24cxxx.h"


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

  puts ("tests at24cxxx\r\n");

  at24cxxx_t at24cxxx_dev;
  int check;

  printf ("EEPROM size: %u byte\r\n", AT24CXXX_EEPROM_SIZE);
  printf ("Page size  : %u byte\r\n", AT24CXXX_PAGE_SIZE);

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
      printf ("[FAILURE] at24cxxx_init: (%d)\r\n", check);
      return 1;
    }
  else
    {
      puts ("[SUCCESS] at24cxxx_init\r\n");
    }

    /* erase EEPROM to exclude side effects from prior test runs */
#if AT24CXXX_ERASE
  check = at24cxxx_erase (&at24cxxx_dev);
  if (check != AT24CXXX_OK)
    {
      printf ("[FAILURE] at24cxxx_erase: %d (EEPROM size = %" PRId32 ")\r\n",
              check, at24cxxx_dev.params.eeprom_size);
      return 1;
    }
  else
    {
      puts ("[SUCCESS] at24cxxx_erase\r\n");
    }
#endif

  /* Test: Write/Read Byte */
  check = at24cxxx_write_byte (&at24cxxx_dev, WRITE_BYTE_POSITION,
                               WRITE_BYTE_CHARACTER);
  if (check != AT24CXXX_OK)
    {
      printf ("[FAILURE] at24cxxx_write_byte: %d\r\n", check);
      return 1;
    }
  else
    {
      puts ("[SUCCESS] at24cxxx_write_byte\r\n");
    }

  uint8_t c;
  check = at24cxxx_read_byte (&at24cxxx_dev, WRITE_BYTE_POSITION, &c);
  if (check < 0)
    {
      printf ("[FAILURE] at24cxxx_read_byte: %d\r\n", check);
      return 1;
    }
  else
    {
      puts ("[SUCCESS] at24cxxx_read_byte\r\n");
    }

  if (c != WRITE_BYTE_CHARACTER)
    {
      printf ("[FAILURE] write_byte/read_byte: (%d != %d)\r\n", c,
              WRITE_BYTE_CHARACTER);
      return 1;
    }
  else
    {
      puts ("[SUCCESS] write_byte/read_byte\r\n");
    }

  /* Test: Write */
  uint8_t expected_write_data[] = WRITE_CHARACTERS;

  check = at24cxxx_write (&at24cxxx_dev, WRITE_POSITION, expected_write_data,
                          sizeof (expected_write_data));
  if (check != AT24CXXX_OK)
    {
      printf ("[FAILURE] at24cxxx_write: %d (size = %u)\r\n", check,
              sizeof (expected_write_data));
      return 1;
    }
  else
    {
      puts ("[SUCCESS] at24cxxx_write\r\n");
    }

  /* Test: Read */
  uint8_t actual_write_data[sizeof (expected_write_data)];

  check = at24cxxx_read (&at24cxxx_dev, WRITE_POSITION, actual_write_data,
                         sizeof (actual_write_data));
  if (check != AT24CXXX_OK)
    {
      printf ("[FAILURE] at24cxxx_read: %d\r\n", check);
      return 1;
    }
  else
    {
      puts ("[SUCCESS] at24cxxx_read\r\n");
    }

  if (memcmp (actual_write_data, expected_write_data,
              sizeof (actual_write_data))
      != 0)
    {
      printf ("[FAILURE] write/read: (%s != %s)\r\n", actual_write_data,
              expected_write_data);
      return 1;
    }
  else
    {
      puts ("[SUCCESS] write/read\r\n");
    }

  /* Test: Set */
  uint8_t expected_set_data[SET_LEN];
  memset (expected_set_data, SET_CHARACTER, SET_LEN);

  uint8_t actual_set_data[sizeof (expected_set_data)];

  check = at24cxxx_set (&at24cxxx_dev, SET_POSITION, SET_CHARACTER, SET_LEN);
  if (check != AT24CXXX_OK)
    {
      printf ("[FAILURE] at24cxxx_set: %d (size = %u)\r\n", check, SET_LEN);
      return 1;
    }
  else
    {
      puts ("[SUCCESS] at24cxxx_set\r\n");
    }

  check
      = at24cxxx_read (&at24cxxx_dev, SET_POSITION, actual_set_data, SET_LEN);
  if (check != AT24CXXX_OK)
    {
      printf ("[FAILURE] set/read: %d\r\n", check);
      return 1;
    }
  else if (memcmp (actual_set_data, expected_set_data, SET_LEN) != 0)
    {
      printf ("[FAILURE] set/read: (%s != %s)\r\n", actual_set_data,
              expected_set_data);
      return 1;
    }
  else
    {
      puts ("[SUCCESS] set/read\r\n");
    }

  puts ("Finished tests at24cxxx\r\n");

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
    }
}
