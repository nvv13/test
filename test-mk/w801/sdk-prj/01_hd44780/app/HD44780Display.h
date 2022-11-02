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

#ifndef __HD44780DISPLAY__
#define __HD44780DISPLAY__

//#include <inttypes.h>
#include "wm_io.h"
#include "wm_type_def.h"
//#include "wm_gpio.h"

//#ifndef LCD44780_RS
//#warning "LCD Using default pin"
#define LCD44780_RS WM_IO_PB_21
#define LCD44780_RW WM_IO_PB_22
#define LCD44780_EN WM_IO_PB_23
#define LCD44780_D0 WM_IO_PB_24
#define LCD44780_D1 WM_IO_PB_25
#define LCD44780_D2 WM_IO_PB_26
#define LCD44780_D3 WM_IO_PB_18
//#endif

#define LCD44780_COL_COUNT 16
#define LCD44780_ROW_COUNT 2

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

void LCD44780_init (void);

void LCD44780_command (uint8_t command);
void LCD44780_write (uint8_t value);

void LCD44780_on (void);
void LCD44780_off (void);

void LCD44780_clear (void);
void LCD44780_return_home (void);

void LCD44780_enable_blinking (void);
void LCD44780_disable_blinking (void);

void LCD44780_enable_cursor (void);
void LCD44780_disable_cursor (void);

void LCD44780_scroll_left (void);
void LCD44780_scroll_right (void);

void LCD44780_set_left_to_right (void);
void LCD44780_set_right_to_left (void);

void LCD44780_enable_autoscroll (void);
void LCD44780_disable_autoscroll (void);

void LCD44780_create_char (uint8_t location, uint8_t *charmap);

void LCD44780_set_cursor (uint8_t col, uint8_t row);

void LCD44780_puts (char *string);
void LCD44780_printf (char *format, ...);

#endif // __HD44780DISPLAY__
