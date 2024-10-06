
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wm_type_def.h"

#include "wm_cpu.h"
#include "wm_rtc.h"
#include "wm_timer.h"

#include "utils.h"
#include "wm_gpio.h"
#include "wm_gpio_afsel.h"
#include "wm_io.h"
#include "wm_osal.h"

#include "mod1/at24cxxx.h"

#ifndef AT24CXXX_ERASE
#define AT24CXXX_ERASE (0)
#endif

#define AT24CXXX_EEPROM_SIZE (AT24C32_EEPROM_SIZE)
#define AT24CXXX_PAGE_SIZE (AT24C32_PAGE_SIZE)
#define AT24CXXX_MAX_POLLS (AT24C32_MAX_POLLS)
#define AT24CXXX_PIN_WP (GPIO_UNDEF)
#define AT24CXXX_ADDR (AT24CXXX_DEF_DEV_ADDR + 0) // A0=0,A1=0,A2=0
// #define AT24CXXX_ADDR                   (AT24CXXX_DEF_DEV_ADDR + 7) //
// A0=1,A1=1,A2=1

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
ReadByte (int iPosition, u8 *u8_value)
{
  int check = at24cxxx_read_byte (&at24cxxx_dev, iPosition, u8_value);
  if (check < 0)
    {
      printf ("[FAILURE] at24cxxx_read_byte: %d\n", check);
      return 1;
    }
  else
    {
      puts ("[SUCCESS] at24cxxx_read_byte");
    }
  return 0;
}

int
WriteByte (int iPosition, u8 u8_value)
{
  /* Test: Write/Read Byte */
  int check = at24cxxx_write_byte (&at24cxxx_dev, iPosition, u8_value);
  if (check != AT24CXXX_OK)
    {
      printf ("[FAILURE] at24cxxx_write_byte: %d\n", check);
      return 1;
    }
  else
    {
      puts ("[SUCCESS] at24cxxx_write_byte");
    }
  return 0;
}
