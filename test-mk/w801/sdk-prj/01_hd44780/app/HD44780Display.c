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

/*

*/
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

#define HD44780_CMD_WAIT (2000U)
#define HD44780_INIT_WAIT_XXL (50000U)
#define HD44780_INIT_WAIT_LONG (4500U)
#define HD44780_INIT_WAIT_SHORT (150U)
#define HD44780_PULSE_WAIT_SHORT (1U)
#define HD44780_PULSE_WAIT_LONG (100U)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <inttypes.h>

#include "wm_i2c.h"

#include "wm_gpio.h"
#include "wm_gpio_afsel.h"

#include "HD44780Display.h"
#include "n_delay.h"

/*
у PCF8574 и PCF8574A – разная адресация, которую, в дополнение, можно ещё и
менять тремя резисторами на плате.
Резисторы обеспечивают низкий уровень на линиях A0-A2, при их отсутствии
на линиях будет высокий уровень,
что даст для PCF8574A адрес 7Eh для записи и 7Fh для чтения.
Для PCF8574 же это будут адреса 4Eh для записи и 4Fh для чтения.

*/

#define PCF8575_BASE_ADDR                                                     \
  (0x20) /**< PCF8575 I2C slave base address.                                 \
              Addresses are then in range from                                \
              0x20 to 0x27 */
#define PCF8574_BASE_ADDR                                                     \
  (0x4e) /**< PCF8574 I2C slave base address.                                 \
              Addresses are then in range from                                \
              0x20 to 0x27 */
#define PCF8574A_BASE_ADDR                                                    \
  (0x38) /**< PCF8574A I2C slave base address.                                \
              Addresses are then in range from                                \
              0x38 to 0x3f */

#define PCF8575_GPIO_PIN_NUM (16) /**< PCF8575 has 16 I/O pins */
#define PCF8574_GPIO_PIN_NUM (8)  /**< PCF8574 has 8 I/O pins */
#define PCF8574A_GPIO_PIN_NUM (8) /**< PCF8574A has 8 I/O pins */

uint8_t
i2c_send_one_byte (uint8_t addr, uint8_t data)
{
  tls_reg_write32 (HR_I2C_TX_RX,
                   addr); // заносим в регистр данных адрес слейва

  tls_reg_write32 (
      HR_I2C_CR_SR,
      I2C_CR_STA | I2C_CR_WR); // включаем модуль на передачу и выдаем START

  while (tls_reg_read32 (HR_I2C_CR_SR) & I2C_SR_TIP)
    ; // ждем окончания передачи



  if (tls_reg_read32 (HR_I2C_CR_SR)
      & I2C_SR_NAK) // если по окончанию передачи байта слейв не ответил
    {
      tls_reg_write32 (HR_I2C_CR_SR, I2C_CR_STO); // останавливаем обмен,
      while (tls_reg_read32 (HR_I2C_CR_SR) & I2C_SR_BUSY)
        ;       // ожидаем освобождения шины
      return 1; // возвращаем код ошибки "1"
    }           // если есть ответ от слейва

  tls_reg_write32 (HR_I2C_TX_RX,
                   data); // заносим в регистр данных байт на отправку
  tls_reg_write32 (
      HR_I2C_CR_SR,
      I2C_CR_WR | I2C_CR_STO); // передаем байт и по окончании передачи - STOP
  while (tls_reg_read32 (HR_I2C_CR_SR) & I2C_SR_BUSY)
    ;       // ожидаем освобождения шины

  printf ("i2c_send_one_byte data= 0x%.2X \n",data);

  return 0; // возвращаем "0" - передача успешна
}


static enum x_out_mode ModeOut = X_I2C_MODE;
static u8 _backlight = 1;
static u8 u8_addr = PCF8574_BASE_ADDR;

static u8
gpio_pin_to_num_pin_PCF857X (enum tls_io_name gpio_pin)
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
    case LCD44780_D4:
      num_pin = 4;
      break;
    case LCD44780_D5:
      num_pin = 5;
      break;
    case LCD44780_D6:
      num_pin = 6;
      break;
    case LCD44780_D7:
      num_pin = 7;
      break;
    }
  return num_pin;
}

// write a nibble / halfByte with handshake
void
LiquidCrystal_PCF8574_writeNibble (uint8_t halfByte, bool isData)
{
  if (ModeOut == X_GPIO_MODE)
    {

      // map the data to the given pin connections
      tls_gpio_write (LCD44780_RS, (isData ? 1 : 0));
      // _rw_mask is not used here.
      tls_gpio_write (LCD44780_BACKLIGHT, (_backlight > 0 ? 1 : 0));

      // allow for arbitrary pin configuration
      tls_gpio_write (LCD44780_D4, (halfByte & 0x01 ? 1 : 0));
      tls_gpio_write (LCD44780_D5, (halfByte & 0x02 ? 1 : 0));
      tls_gpio_write (LCD44780_D6, (halfByte & 0x04 ? 1 : 0));
      tls_gpio_write (LCD44780_D7, (halfByte & 0x08 ? 1 : 0));

      tls_gpio_write (LCD44780_EN, 0);
      n_delay_us (HD44780_PULSE_WAIT_SHORT);
      tls_gpio_write (LCD44780_EN, 1);
      n_delay_us (HD44780_PULSE_WAIT_SHORT);
      tls_gpio_write (LCD44780_EN, 0);
      n_delay_us (HD44780_PULSE_WAIT_LONG);
      // delayMicroseconds(37); // commands need > 37us to settle
    }
  else
    {
      // map the data to the given pin connections
      uint8_t data
          = isData ? (0x01 << gpio_pin_to_num_pin_PCF857X (LCD44780_RS)) : 0;
      // _rw_mask is not used here.
      if (_backlight > 0)
        data |= (0x01 << gpio_pin_to_num_pin_PCF857X (LCD44780_BACKLIGHT));

      // allow for arbitrary pin configuration
      if (halfByte & 0x01)
        data |= (0x01 << gpio_pin_to_num_pin_PCF857X (LCD44780_D4));
      if (halfByte & 0x02)
        data |= (0x01 << gpio_pin_to_num_pin_PCF857X (LCD44780_D5));
      if (halfByte & 0x04)
        data |= (0x01 << gpio_pin_to_num_pin_PCF857X (LCD44780_D6));
      if (halfByte & 0x08)
        data |= (0x01 << gpio_pin_to_num_pin_PCF857X (LCD44780_D7));

      // Note that the specified speed of the PCF8574 chip is 100KHz.
      // Transmitting a single byte takes 9 clock ticks at 100kHz -> 90us.
      // The 37us delay is only necessary after sending the second nibble.
      // But in that case we have to restart the transfer using additional
      // >10 clock cycles. Hence, no additional delays are necessary even
      // when the I2C bus is operated beyond the chip's spec in fast mode
      // at 400 kHz.

      // tls_i2c_write_byte (data, 0);
      // tls_i2c_wait_ack ();
      i2c_send_one_byte (u8_addr, data);
      n_delay_us (HD44780_PULSE_WAIT_SHORT);

      //      tls_i2c_write_byte (
      //          data | (0x01 << gpio_pin_to_num_pin_PCF857X (LCD44780_EN)),
      //          0);
      //      tls_i2c_wait_ack ();
      i2c_send_one_byte (
          u8_addr, data | (0x01 << gpio_pin_to_num_pin_PCF857X (LCD44780_EN)));
      n_delay_us (HD44780_PULSE_WAIT_SHORT);
      // delayMicroseconds(1); // enable pulse must be >450ns

      // tls_i2c_write_byte (data, 0);
      // tls_i2c_wait_ack ();
      i2c_send_one_byte (u8_addr, data);
      // delayMicroseconds(37); // commands need > 37us to settle
      // n_delay_us (37);
      n_delay_us (HD44780_PULSE_WAIT_LONG);

      printf ("LiquidCrystal_PCF8574_writeNibble (halfByte=0x%.2X,isData=%d,Data=0x%.2X)\n",halfByte,(isData?1:0),data);
    }
} // _writeNibble

// write either command or data
void
LiquidCrystal_PCF8574_send (uint8_t value, bool isData)
{
  if (ModeOut == X_GPIO_MODE)
    {
      // write high 4 bits
      LiquidCrystal_PCF8574_writeNibble ((value >> 4 & 0x0F), isData);

      // write low 4 bits
      LiquidCrystal_PCF8574_writeNibble ((value & 0x0F), isData);
    }
  else
    {
      // An I2C transmission has a significant overhead of ~10+1 I2C clock
      // cycles. We consequently only perform it only once per _send().

      //      tls_i2c_write_byte (addr, 1);
      //      tls_i2c_wait_ack ();

      // write high 4 bits
      LiquidCrystal_PCF8574_writeNibble ((value >> 4 & 0x0F), isData);

      //      tls_i2c_stop ();
      //      tls_os_time_delay (1);

      //      tls_i2c_write_byte (addr, 1);
      //      tls_i2c_wait_ack ();

      // write low 4 bits
      LiquidCrystal_PCF8574_writeNibble ((value & 0x0F), isData);

      //      tls_i2c_stop ();
      //      tls_os_time_delay (1);
    }
} // _send()

// write a nibble / halfByte with handshake
void
LiquidCrystal_PCF8574_sendNibble (uint8_t halfByte, bool isData)
{
  if (ModeOut == X_GPIO_MODE)
    {
      LiquidCrystal_PCF8574_writeNibble (halfByte, isData);
    }
  else
    {
      //      tls_i2c_write_byte (addr, 1);
      //      tls_i2c_wait_ack ();
      LiquidCrystal_PCF8574_writeNibble (halfByte, isData);
      //      tls_i2c_stop ();
      //      tls_os_time_delay (1);
    }
} // _sendNibble

// private function to change the PCF8574 pins to the given value
void
LiquidCrystal_PCF8574_write2Wire (uint8_t data, bool isData, bool enable)
{
  if (ModeOut == X_GPIO_MODE)
    {
    }
  else
    {
      if (isData)
        data |= (0x01 << gpio_pin_to_num_pin_PCF857X (LCD44780_RS));
      // _rw_mask is not used here.
      if (enable)
        data |= (0x01 << gpio_pin_to_num_pin_PCF857X (LCD44780_EN));
      if (_backlight > 0)
        data |= (0x01 << gpio_pin_to_num_pin_PCF857X (LCD44780_BACKLIGHT));

      i2c_send_one_byte (u8_addr, data);

      //      tls_i2c_write_byte (addr, 1);
      //      tls_i2c_wait_ack ();
      //      tls_i2c_write_byte (data, 0);
      //      tls_i2c_wait_ack ();
      //      tls_i2c_stop ();
      //      tls_os_time_delay (1);
    }
} // write2Wire

void
LCD44780_SET_BACKLIGHT (uint8_t value)
{
  _backlight = value;
  if (ModeOut == X_GPIO_MODE)
    {
      tls_gpio_write (LCD44780_BACKLIGHT, (_backlight > 0 ? 1 : 0));
    }
  else
    {
      // send no data but set the background-pin right;
      LiquidCrystal_PCF8574_write2Wire (0x00, true, false);
    }
}

static uint8_t LCD44780_displayparams;
static char LCD44780_buffer[LCD44780_COL_COUNT + 1];

void
LCD44780_command (uint8_t command)
{
  LiquidCrystal_PCF8574_send (command, false);
}

void
LCD44780_write (uint8_t value)
{
  LiquidCrystal_PCF8574_send (value, true);
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
      tls_gpio_cfg (LCD44780_D4, WM_GPIO_DIR_OUTPUT,
                    WM_GPIO_ATTR_FLOATING); // 24
      tls_gpio_cfg (LCD44780_D5, WM_GPIO_DIR_OUTPUT,
                    WM_GPIO_ATTR_FLOATING); // 25
      tls_gpio_cfg (LCD44780_D6, WM_GPIO_DIR_OUTPUT,
                    WM_GPIO_ATTR_FLOATING); // 26
      tls_gpio_cfg (LCD44780_D7, WM_GPIO_DIR_OUTPUT,
                    WM_GPIO_ATTR_FLOATING); // 18

      tls_gpio_write (LCD44780_EN, 0);
      tls_gpio_write (LCD44780_RS, 0);
      tls_gpio_write (LCD44780_RW, 0);
    }

  else
    {
      wm_i2c_scl_config (LCD44780_I2C_SCL);
      wm_i2c_sda_config (LCD44780_I2C_SDA);
      tls_i2c_init (LCD44780_I2C_FREQ);

      LiquidCrystal_PCF8574_write2Wire (0x00, false, false);
    }

  // Wait for LCD to become ready (docs say 15ms+)
  /* see hitachi HD44780 datasheet pages 45/46 for init specs */
  n_delay_us (HD44780_INIT_WAIT_XXL);

  LiquidCrystal_PCF8574_sendNibble (0x03, true); // Switch to 4 bit mode
  n_delay_us (HD44780_INIT_WAIT_LONG);

  LiquidCrystal_PCF8574_sendNibble (0x03, true); // 2nd time
  n_delay_us (HD44780_INIT_WAIT_LONG);

  LiquidCrystal_PCF8574_sendNibble (0x03, true); // 3rd time
  n_delay_us (HD44780_INIT_WAIT_SHORT);

  LiquidCrystal_PCF8574_sendNibble (
      0x02, true); // // finally, set to 4-bit interface

  LCD44780_command (LCD44780_FUNCTIONSET | LCD44780_4BITMODE | LCD44780_2LINE
                    | LCD44780_5x8DOTS);

  LCD44780_on ();
  LCD44780_clear ();

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
  n_delay_us (HD44780_CMD_WAIT);
}

void
LCD44780_return_home (void)
{
  LCD44780_command (LCD44780_RETURNHOME);
  n_delay_us (HD44780_CMD_WAIT);
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
