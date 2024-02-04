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
#include "wm_io.h"
#include "utils.h"

#include "at24cxxx.h"

#ifndef AT24CXXX_ERASE
#define AT24CXXX_ERASE (0)
#endif

#define AT24CXXX_EEPROM_SIZE            (AT24C32_EEPROM_SIZE)
#define AT24CXXX_PAGE_SIZE              (AT24C32_PAGE_SIZE)
#define AT24CXXX_MAX_POLLS              (AT24C32_MAX_POLLS)
#define AT24CXXX_PIN_WP                 (GPIO_UNDEF)
#define AT24CXXX_ADDR                   (AT24CXXX_DEF_DEV_ADDR + 0) // A0=0,A1=0,A2=0  
//#define AT24CXXX_ADDR                   (AT24CXXX_DEF_DEV_ADDR + 7) // A0=1,A1=1,A2=1


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
Test (void)
{
  puts ("Starting tests for module at24cxxx");

  at24cxxx_t at24cxxx_dev;
  int check;

  printf ("EEPROM size: %u byte\n", AT24CXXX_EEPROM_SIZE);
  printf ("Page size  : %u byte\n", AT24CXXX_PAGE_SIZE);


  i2c_param_t user_i2c = 
      {            
      .i2c_freq=400000,             /* частота i2c в герцах */
      .i2c_scl=WM_IO_PA_01, /* WM_IO_PA_01 or WM_IO_PB_20 */
      .i2c_sda=WM_IO_PA_04, /* WM_IO_PA_04 or WM_IO_PB_19 */
      };

  at24cxxx_params_t user_data = 
      {            
     .i2c         = &user_i2c,
     .pin_wp      = AT24CXXX_PIN_WP,                
     .eeprom_size = AT24CXXX_EEPROM_SIZE,      
     .dev_addr    = AT24CXXX_ADDR,                
     .page_size   = AT24CXXX_PAGE_SIZE,          
     .max_polls   = AT24CXXX_MAX_POLLS           
     };

  /* initialize the i2c */
  i2c_init(&user_i2c);

  /* Test: Init */
  check = at24cxxx_init (&at24cxxx_dev, &user_data);
  if (check != AT24CXXX_OK)
    {
      printf ("[FAILURE] at24cxxx_init: (%d)\n", check);
      return 1;
    }
  else
    {
      puts ("[SUCCESS] at24cxxx_init");
    }

    /* erase EEPROM to exclude side effects from prior test runs */
#if AT24CXXX_ERASE
  check = at24cxxx_erase (&at24cxxx_dev);
  if (check != AT24CXXX_OK)
    {
      printf ("[FAILURE] at24cxxx_erase: %d (EEPROM size = %" PRId32 ")\n",
              check, at24cxxx_dev.params.eeprom_size);
      return 1;
    }
  else
    {
      puts ("[SUCCESS] at24cxxx_erase");
    }
#endif

  /* Test: Write/Read Byte */
  check = at24cxxx_write_byte (&at24cxxx_dev, WRITE_BYTE_POSITION,
                               WRITE_BYTE_CHARACTER);
  if (check != AT24CXXX_OK)
    {
      printf ("[FAILURE] at24cxxx_write_byte: %d\n", check);
      return 1;
    }
  else
    {
      puts ("[SUCCESS] at24cxxx_write_byte");
    }

  uint8_t c;
  check = at24cxxx_read_byte (&at24cxxx_dev, WRITE_BYTE_POSITION, &c);
  if (check < 0)
    {
      printf ("[FAILURE] at24cxxx_read_byte: %d\n", check);
      return 1;
    }
  else
    {
      puts ("[SUCCESS] at24cxxx_read_byte");
    }

  if (c != WRITE_BYTE_CHARACTER)
    {
      #define PRId8 "d"
      printf ("[FAILURE] write_byte/read_byte: (%" PRId8 " != %d)\n", c,
              WRITE_BYTE_CHARACTER);
      return 1;
    }
  else
    {
      puts ("[SUCCESS] write_byte/read_byte");
    }

  /* Test: Write */
  uint8_t expected_write_data[] = WRITE_CHARACTERS;

  check = at24cxxx_write (&at24cxxx_dev, WRITE_POSITION, expected_write_data,
                          sizeof (expected_write_data));
  if (check != AT24CXXX_OK)
    {
      printf ("[FAILURE] at24cxxx_write: %d (size = %zu)\n", check,
              sizeof (expected_write_data));
      return 1;
    }
  else
    {
      puts ("[SUCCESS] at24cxxx_write");
    }

  /* Test: Read */
  uint8_t actual_write_data[sizeof (expected_write_data)];

  check = at24cxxx_read (&at24cxxx_dev, WRITE_POSITION, actual_write_data,
                         sizeof (actual_write_data));
  if (check != AT24CXXX_OK)
    {
      printf ("[FAILURE] at24cxxx_read: %d\n", check);
      return 1;
    }
  else
    {
      puts ("[SUCCESS] at24cxxx_read");
    }

  if (memcmp (actual_write_data, expected_write_data,
              sizeof (actual_write_data))
      != 0)
    {
      printf ("[FAILURE] write/read: (%s != %s)\n", actual_write_data,
              expected_write_data);
      return 1;
    }
  else
    {
      puts ("[SUCCESS] write/read");
    }

  /* Test: Set */
  uint8_t expected_set_data[SET_LEN];
  memset (expected_set_data, SET_CHARACTER, SET_LEN);

  uint8_t actual_set_data[sizeof (expected_set_data)];

  check = at24cxxx_set (&at24cxxx_dev, SET_POSITION, SET_CHARACTER, SET_LEN);
  if (check != AT24CXXX_OK)
    {
      printf ("[FAILURE] at24cxxx_set: %d (size = %u)\n", check, SET_LEN);
      return 1;
    }
  else
    {
      puts ("[SUCCESS] at24cxxx_set");
    }

  check
      = at24cxxx_read (&at24cxxx_dev, SET_POSITION, actual_set_data, SET_LEN);
  if (check != AT24CXXX_OK)
    {
      printf ("[FAILURE] set/read: %d\n", check);
      return 1;
    }
  else if (memcmp (actual_set_data, expected_set_data, SET_LEN) != 0)
    {
      printf ("[FAILURE] set/read: (%s != %s)\n", actual_set_data,
              expected_set_data);
      return 1;
    }
  else
    {
      puts ("[SUCCESS] set/read");
    }

  puts ("Finished tests for module at24cxxx");

  #define BUF_LEN 256 
  u8 ReadBuffer[BUF_LEN] = { 0 };
  int iPos=0;
  while(1)
  {
    check = at24cxxx_read (&at24cxxx_dev, iPos, ReadBuffer, BUF_LEN);
    if (check != AT24CXXX_OK)
      {
        printf ("[FAILURE] read iPos: %d\n", iPos);
        return 1;
      }
      else
      printf ("read iPos: %d\n", iPos);
    dumpBuffer("dump", (char*)ReadBuffer, BUF_LEN);
    iPos+=BUF_LEN;
  }

  return 0;
}

void
UserMain (void)
{
  printf ("UserMain start");
  tls_sys_clk_set (CPU_CLK_240M);
  Test ();

}
