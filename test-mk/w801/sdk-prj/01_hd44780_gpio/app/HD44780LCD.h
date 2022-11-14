/*
 * File: HD44780_LCD_PCF8574.h
 * Description:
 * HD44780-based character LCD 16x02 I2C(PCF8574) library header file for
 * arduino eco system Author: Gavin Lyons. Created : March 2022 Description:
 * See URL for full details. URL:
 * https://github.com/gavinlyonsrepo/HD44780_LCD_PCF8574
 */

#ifndef __HD44780LCD__
#define __HD44780LCD__

#include "wm_type_def.h"

#include "wm_gpio.h"
#include "wm_gpio_afsel.h"

// Section: Defines

//#define LCD_SERIAL_DEBUG // comment in for serial debug errors and flow

#define LCD44780_RS WM_IO_PB_21
#define LCD44780_RW WM_IO_PB_22
#define LCD44780_EN WM_IO_PB_23
#define LCD44780_D4 WM_IO_PB_24
#define LCD44780_D5 WM_IO_PB_25
#define LCD44780_D6 WM_IO_PB_26
#define LCD44780_D7 WM_IO_PB_18
#define LCD44780_BACKLIGHT WM_IO_PB_17

#define DISPLAY_DELAY_INIT 50 // mS

// Command Byte Codes See  URL : dinceraydin.com/lcd/commands.htm for HD44780
// CMDs

#define LCD_MODE_8BIT                                                         \
  0x38 // Function set (8-bit interface, 2 lines, 5*7 Pixels)
#define LCD_MODE_4BIT                                                         \
  0x28 // Function set (4-bit interface, 2 lines, 5*7 Pixels)

#define LCD_SCROLL_RIGHT 0x1E // Scroll display one character right (all lines)
#define LCD_SCROLL_LEFT 0x18  // Scroll display one character left (all lines)
#define LCD_HOME 0x02 // Home (move cursor to top/left character position)

#define LCD_MOV_CURSOR_LEFT 0x10  // Move cursor one character left
#define LCD_MOV_CURSOR_RIGHT 0x14 // Move cursor one character right

#define LCD_DISPLAY_ON 0x0C  // Restore the display (with cursor hidden)
#define LCD_DISPLAY_OFF 0x08 // Blank the display (without clearing)
#define LCD_CLRSCR 0x01      // clear screen

#define LCD_LINE_ADR1 0x80 // Set cursor position (DDRAM address) 80+ addr
#define LCD_LINE_ADR2                                                         \
  0xC0 // Set cursor position line 2 (DDRAM address) C0+ addr
#define LCD_LINE_ADR3_20 0x94 // line 3 untested, no part, for 20x04
#define LCD_LINE_ADR4_20 0xD4 // line 4 untested, no part, for 20x04
#define LCD_LINE_ADR3_16 0x90 // line 3 untested, no part, for 16x04
#define LCD_LINE_ADR4_16 0xD0 // line 4 untested, no part, for 16x04
#define LCD_CG_RAM                                                            \
  0x40 // Set pointer in character-generator RAM (CG RAM address)

// Codes for I2C byte,
// Byte = DATA-led-en-rw-rs (en=enable rs = reg select)(led always on rw always
// write)
#define LCD_DATA_BYTE_ON 0x0D     // enable=1 and rs =1 1101  DATA-led-en-rw-rs
#define LCD_DATA_BYTE_OFF 0x09    // enable=0 and rs =1 1001 DATA-led-en-rw-rs
#define LCD_CMD_BYTE_ON 0x0C      // enable=1 and rs =0 1100 COMD-led-en-rw-rs
#define LCD_CMD_BYTE_OFF 0x08     // enable=0 and rs =0 1000 COMD-led-en-rw-rs
#define LCD_BACKLIGHTON_MASK 0x0F // XXXX-1111 , XXXX = don't care
#define LCD_BACKLIGHTOFF_MASK 0x07 // XXXX-0111

// Section :: enums

typedef enum
{
  LCDEntryModeOne = 0x04, // Display Shift :OFF Decrement Address Counter
  LCDEntryModeTwo = 0x05, // Display Shift :ON  Decrement Address Counter
  LCDEntryModeThree
  = 0x06, // Display Shift :OFF Increment Address Counter, default
  LCDEntryModeFour = 0x07, // Display Shift :ON Increment Address Counter
} LCDEntryMode_e;          // Entry mode  set command

typedef enum
{
  LCDCursorTypeOff = 0x0C,   // Make cursor invisible
  LCDCursorTypeBlink = 0x0D, // Turn on blinking-block cursor
  LCDCursorTypeOn = 0x0E,    // Turn on visible  underline cursor
  LCDCursorTypeOnBlink
  = 0x0F,          // Turn on blinking-block cursor + visible underline cursor
} LCDCursorType_e; // Cursor mode

typedef enum
{
  LCDMoveRight = 1,   // move right
  LCDMoveLeft = 2,    // move left
} LCDDirectionType_e; // Direction mode for scroll and move

typedef enum
{
  LCDLineNumberOne = 1,   // row 1
  LCDLineNumberTwo = 2,   // row 2
  LCDLineNumberThree = 3, // row 3
  LCDLineNumberFour = 4,  // row 4
} LCDLineNumber_e;        // line number

// Section: Class's

void HD44780LCD_HD44780LCD (uint8_t NumRow, uint8_t NumCol);

void HD44780LCD_LCDInit (LCDCursorType_e);
void HD44780LCD_LCDDisplayON (bool);
void HD44780LCD_LCDResetScreen (LCDCursorType_e);
void HD44780LCD_LCDBackLightSet (bool);

void HD44780LCD_LCDSendString (char *str);
void HD44780LCD_LCDSendChar (char data);
void HD44780LCD_LCDCreateCustomChar (uint8_t location, uint8_t *charmap);
void HD44780LCD_LCDPrintCustomChar (uint8_t location);

void HD44780LCD_LCDMoveCursor (LCDDirectionType_e, uint8_t moveSize);
void HD44780LCD_LCDScroll (LCDDirectionType_e, uint8_t ScrollSize);
void HD44780LCD_LCDGOTO (LCDLineNumber_e lineNo, uint8_t col);
void HD44780LCD_LCDClearLine (LCDLineNumber_e lineNo);
void HD44780LCD_LCDClearScreen (void);
void HD44780LCD_LCDClearScreenCmd (void);
void HD44780LCD_LCDHome (void);
void HD44780LCD_LCDChangeEntryMode (LCDEntryMode_e mode);
void HD44780LCD_printf (char *format, ...);

#endif // __HD44780LCD__
