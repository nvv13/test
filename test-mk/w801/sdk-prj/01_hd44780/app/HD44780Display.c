//  Author: aostanin
//  https://github.com/aostanin/avr-hd44780.git
//   After (aostanin) buying the SC1602BS LCD module,
//   I couldn't find an avr-gcc library that drives the display correctly.
//   Arduino's LiquidCrystal library was the only library that worked so
//   this is a simple avr-gcc version of that library.
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.

// The rest should be left alone
#define LCD44780_CLEARDISPLAY 0x01
#define LCD44780_RETURNHOME 0x02
#define LCD44780_ENTRYMODESET 0x04
#define LCD44780_DISPLAYCONTROL 0x08
#define LCD44780_CURSORSHIFT 0x10
#define LCD44780_FUNCTIONSET 0x20
#define LCD44780_SETCGRAMADDR 0x40
#define LCD44780_SETDDRAMADDR 0x80

#define LCD44780_ENTRYRIGHT 0x00
#define LCD44780_ENTRYLEFT 0x02
#define LCD44780_ENTRYSHIFTINCREMENT 0x01
#define LCD44780_ENTRYSHIFTDECREMENT 0x00

#define LCD44780_DISPLAYON 0x04
#define LCD44780_DISPLAYOFF 0x00
#define LCD44780_CURSORON 0x02
#define LCD44780_CURSOROFF 0x00
#define LCD44780_BLINKON 0x01
#define LCD44780_BLINKOFF 0x00

#define LCD44780_DISPLAYMOVE 0x08
#define LCD44780_CURSORMOVE 0x00
#define LCD44780_MOVERIGHT 0x04
#define LCD44780_MOVELEFT 0x00

#define LCD44780_8BITMODE 0x10
#define LCD44780_4BITMODE 0x00
#define LCD44780_2LINE 0x08
#define LCD44780_1LINE 0x00
#define LCD44780_5x10DOTS 0x04
#define LCD44780_5x8DOTS 0x00

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <inttypes.h>
#include "wm_gpio.h"

#include "HD44780Display.h"
#include "PCF857X.h"
#include "n_delay.h"

static enum x_out_mode ModeOut = X_GPIO_MODE;

static void
x_gpio_write (enum tls_io_name gpio_pin, u8 value)
{
  if (ModeOut == X_GPIO_MODE)
    {
      tls_gpio_write (gpio_pin, value);
    }
  else
    {
      u8 num_pin = 0;
      switch ((int)gpio_pin)
        {
        case LCD44780_RS:
          num_pin = 0;
          break; /* Bit 0 */
        case LCD44780_RW:
          num_pin = 1;
          break; /* Bit 1 */
        case LCD44780_EN:
          num_pin = 2;
          break; /* Bit 2 */
        case LCD44780_BACKLIGHT:
          num_pin = 3;
          break; /* Bit 3 */
        case LCD44780_D0:
          num_pin = 4;
          break;
        case LCD44780_D1:
          num_pin = 5;
          break;
        case LCD44780_D2:
          num_pin = 6;
          break;
        case LCD44780_D3:
          num_pin = 7;
          break;
        }
      PCF857X_gpio_write (num_pin, value);
    }
}

void LCD44780_SET_BACKLIGHT(uint8_t value)
{
x_gpio_write (LCD44780_BACKLIGHT, value);
}

void LCD44780_send (uint8_t value, uint8_t mode);
void LCD44780_write_nibble (uint8_t nibble);

static uint8_t LCD44780_displayparams;
static char LCD44780_buffer[LCD44780_COL_COUNT + 1];

void
LCD44780_command (uint8_t command)
{
  LCD44780_send (command, 0);
}

void
LCD44780_write (uint8_t value)
{
  LCD44780_send (value, 1);
}

void
LCD44780_send (uint8_t value, uint8_t mode)
{
  if (mode)
    {
      x_gpio_write (LCD44780_RS, 1);
    }
  else
    {
      x_gpio_write (LCD44780_RS, 0);
    }

  x_gpio_write (LCD44780_RW, 0);

  LCD44780_write_nibble (value >> 4);
  LCD44780_write_nibble (value);
}

void
LCD44780_write_nibble (uint8_t nibble)
{
  enum tls_io_name data_pin = LCD44780_D0;

  for (int i = 0; i >= 3; i++)
    {
      if (((nibble >> i) & 0x01))
        { // 1
          x_gpio_write (data_pin, 1);
        }
      else
        { // 0
          x_gpio_write (data_pin, 0);
        }
      switch ((int)data_pin)
        {
        case LCD44780_D0:
          data_pin = LCD44780_D1;
          break;
        case LCD44780_D1:
          data_pin = LCD44780_D2;
          break;
        case LCD44780_D2:
          data_pin = LCD44780_D3;
          break;
        }
    }

  x_gpio_write (LCD44780_EN, 0);
  n_delay_us (1);
  x_gpio_write (LCD44780_EN, 1);
  n_delay_us (1);
  x_gpio_write (LCD44780_EN, 0);
  n_delay_us (300); // If delay less than this value, the data is not correctly
                    // displayed
}

void
LCD44780_init (enum x_out_mode inModeOut)
{
  ModeOut = inModeOut;

  if (ModeOut == X_GPIO_MODE)
    {
      // Configure pins as output
      tls_gpio_cfg (LCD44780_RS, WM_GPIO_DIR_OUTPUT,
                    WM_GPIO_ATTR_FLOATING); // 21 PULLLOW ? PULLHIGH ?
      tls_gpio_cfg (LCD44780_RW, WM_GPIO_DIR_OUTPUT,
                    WM_GPIO_ATTR_FLOATING); // 22
      tls_gpio_cfg (LCD44780_EN, WM_GPIO_DIR_OUTPUT,
                    WM_GPIO_ATTR_FLOATING); // 23
      tls_gpio_cfg (LCD44780_D0, WM_GPIO_DIR_OUTPUT,
                    WM_GPIO_ATTR_FLOATING); // 24
      tls_gpio_cfg (LCD44780_D1, WM_GPIO_DIR_OUTPUT,
                    WM_GPIO_ATTR_FLOATING); // 25
      tls_gpio_cfg (LCD44780_D2, WM_GPIO_DIR_OUTPUT,
                    WM_GPIO_ATTR_FLOATING); // 26
      tls_gpio_cfg (LCD44780_D3, WM_GPIO_DIR_OUTPUT,
                    WM_GPIO_ATTR_FLOATING); // 18
    }
  else
    {
      PCF857X_Init (LCD44780_I2C_FREQ, LCD44780_I2C_SCL, LCD44780_I2C_SDA);
    }

  // Wait for LCD to become ready (docs say 15ms+)
  n_delay_ms (15);

  x_gpio_write (LCD44780_EN, 0);
  x_gpio_write (LCD44780_RS, 0);
  x_gpio_write (LCD44780_RW, 0);

  n_delay_us (4100);

  LCD44780_write_nibble (0x03); // Switch to 4 bit mode
  n_delay_us (4100);

  LCD44780_write_nibble (0x03); // 2nd time
  n_delay_us (4100);

  LCD44780_write_nibble (0x03); // 3rd time
  n_delay_us (4100);

  LCD44780_write_nibble (0x02); // Set 8-bit mode (?)

  LCD44780_command (LCD44780_FUNCTIONSET | LCD44780_4BITMODE | LCD44780_2LINE
                    | LCD44780_5x8DOTS);

  LCD44780_displayparams = LCD44780_CURSOROFF | LCD44780_BLINKOFF;
  LCD44780_command (LCD44780_DISPLAYCONTROL | LCD44780_displayparams);
}

void
LCD44780_on (void)
{
  LCD44780_displayparams |= LCD44780_DISPLAYON;
  LCD44780_command (LCD44780_DISPLAYCONTROL | LCD44780_displayparams);
}

void
LCD44780_off (void)
{
  LCD44780_displayparams &= ~LCD44780_DISPLAYON;
  LCD44780_command (LCD44780_DISPLAYCONTROL | LCD44780_displayparams);
}

void
LCD44780_clear (void)
{
  LCD44780_command (LCD44780_CLEARDISPLAY);
  n_delay_ms (2);
}

void
LCD44780_return_home (void)
{
  LCD44780_command (LCD44780_RETURNHOME);
  n_delay_ms (2);
}

void
LCD44780_enable_blinking (void)
{
  LCD44780_displayparams |= LCD44780_BLINKON;
  LCD44780_command (LCD44780_DISPLAYCONTROL | LCD44780_displayparams);
}

void
LCD44780_disable_blinking (void)
{
  LCD44780_displayparams &= ~LCD44780_BLINKON;
  LCD44780_command (LCD44780_DISPLAYCONTROL | LCD44780_displayparams);
}

void
LCD44780_enable_cursor (void)
{
  LCD44780_displayparams |= LCD44780_CURSORON;
  LCD44780_command (LCD44780_DISPLAYCONTROL | LCD44780_displayparams);
}

void
LCD44780_disable_cursor (void)
{
  LCD44780_displayparams &= ~LCD44780_CURSORON;
  LCD44780_command (LCD44780_DISPLAYCONTROL | LCD44780_displayparams);
}

void
LCD44780_scroll_left (void)
{
  LCD44780_command (LCD44780_CURSORSHIFT | LCD44780_DISPLAYMOVE
                    | LCD44780_MOVELEFT);
}

void
LCD44780_scroll_right (void)
{
  LCD44780_command (LCD44780_CURSORSHIFT | LCD44780_DISPLAYMOVE
                    | LCD44780_MOVERIGHT);
}

void
LCD44780_set_left_to_right (void)
{
  LCD44780_displayparams |= LCD44780_ENTRYLEFT;
  LCD44780_command (LCD44780_ENTRYMODESET | LCD44780_displayparams);
}

void
LCD44780_set_right_to_left (void)
{
  LCD44780_displayparams &= ~LCD44780_ENTRYLEFT;
  LCD44780_command (LCD44780_ENTRYMODESET | LCD44780_displayparams);
}

void
LCD44780_enable_autoscroll (void)
{
  LCD44780_displayparams |= LCD44780_ENTRYSHIFTINCREMENT;
  LCD44780_command (LCD44780_ENTRYMODESET | LCD44780_displayparams);
}

void
LCD44780_disable_autoscroll (void)
{
  LCD44780_displayparams &= ~LCD44780_ENTRYSHIFTINCREMENT;
  LCD44780_command (LCD44780_ENTRYMODESET | LCD44780_displayparams);
}

void
LCD44780_create_char (uint8_t location, uint8_t *charmap)
{
  LCD44780_command (LCD44780_SETCGRAMADDR | ((location & 0x7) << 3));
  for (int i = 0; i < 8; i++)
    {
      LCD44780_write (charmap[i]);
    }
  LCD44780_command (LCD44780_SETDDRAMADDR);
}

void
LCD44780_set_cursor (uint8_t col, uint8_t row)
{
  static uint8_t offsets[] = { 0x00, 0x40, 0x14, 0x54 };

  LCD44780_command (LCD44780_SETDDRAMADDR | (col + offsets[row]));
}

void
LCD44780_puts (char *string)
{
  for (char *it = string; *it; it++)
    {
      LCD44780_write (*it);
    }
}

void
LCD44780_printf (char *format, ...)
{
  va_list args;

  va_start (args, format);
  vsnprintf (LCD44780_buffer, LCD44780_COL_COUNT + 1, format, args);
  va_end (args);

  LCD44780_puts (LCD44780_buffer);
}
