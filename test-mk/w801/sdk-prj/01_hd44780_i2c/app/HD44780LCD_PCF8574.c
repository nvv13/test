#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Section : Includes
#include "HD44780LCD_PCF8574.h"
#include "n_delay.h"

static void HD44780LCD_PCF8574_LCDSendCmd (unsigned char cmd);
static void HD44780LCD_PCF8574_LCDSendData (unsigned char data);
static bool HD44780LCD_PCF8574_LCD_I2C_ON (void);

static uint8_t _LCDBackLight = LCD_BACKLIGHTON_MASK;
// I2C  address for I2C module PCF8574 backpack on LCD
static uint8_t _LCDSlaveAddresI2C = 0x27;
static uint8_t _NumRowsLCD = 2;
static uint8_t _NumColsLCD = 16;

// static	TwoWire *wire;
// static	u8 *wire;

static void
wire_begin (void)
{
  wm_i2c_scl_config (LCD44780_I2C_SCL);
  wm_i2c_sda_config (LCD44780_I2C_SDA);
  tls_i2c_init (LCD44780_I2C_FREQ);
}
static u8
wire_beginTransmission (u8 AddresI2C)
{
  tls_reg_write32 (HR_I2C_TX_RX,
                   (AddresI2C << 1)); // заносим в регистр данных адрес слейва

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
        ; // ожидаем освобождения шины
      printf ("wire_beginTransmission error ASK AddresI2C= 0x%.2X \n",
              AddresI2C);
      return 1; // возвращаем код ошибки "1"
    }           // если есть ответ от слейва

  return 0; // возвращаем "0" - передача успешна
}
static u8
wire_write (u8 *cmdI2C, u8 u8_Len)
{
  for (u8 u8_index = 0; u8_index < u8_Len; u8_index++)
    {
      u8 data = cmdI2C[u8_index];

      tls_reg_write32 (HR_I2C_TX_RX,
                       data); // заносим в регистр данных байт на отправку
      if (u8_index == (u8_Len - 1))
        {
          tls_reg_write32 (HR_I2C_CR_SR, // I2C_CR_NAK |
                           I2C_CR_WR
                               | I2C_CR_STO); // передаем байт и по окончании
                                              // передачи - STOP
#ifdef LCD_SERIAL_DEBUG
          printf ("i2c wire_write I2C_CR_STO data= 0x%.2X \n", data);
#endif
        }
      else
        {
          tls_reg_write32 (
              HR_I2C_CR_SR, // I2C_CR_NAK |
              I2C_CR_WR); // передаем байт и по окончании передачи - STOP
#ifdef LCD_SERIAL_DEBUG
          printf ("i2c wire_write data= 0x%.2X \n", data);
#endif
        }

      // while (tls_reg_read32 (HR_I2C_CR_SR) & I2C_SR_BUSY)
      //  ;       // ожидаем освобождения шины
      while (tls_reg_read32 (HR_I2C_CR_SR) & I2C_SR_TIP)
        ; // ждем окончания передачи

      if (tls_reg_read32 (HR_I2C_CR_SR)
          & I2C_SR_NAK) // если по окончанию передачи байта слейв не ответил
        {
          tls_reg_write32 (HR_I2C_CR_SR, I2C_CR_STO); // останавливаем обмен,
          while (tls_reg_read32 (HR_I2C_CR_SR) & I2C_SR_BUSY)
            ; // ожидаем освобождения шины
          printf ("i2c wire_write error ASK data= 0x%.2X \n", data);
          return 1; // возвращаем код ошибки "1"
        }           // если есть ответ от слейва
    }
  return 0; // возвращаем "0" - передача успешна
}

static u8
wire_endTransmission (void)
{
  if (tls_reg_read32 (HR_I2C_CR_SR)
      & I2C_SR_NAK) // если по окончанию передачи байта слейв не ответил
    {
      tls_reg_write32 (HR_I2C_CR_SR, I2C_CR_STO); // останавливаем обмен,
      while (tls_reg_read32 (HR_I2C_CR_SR) & I2C_SR_BUSY)
        ; // ожидаем освобождения шины
      printf ("wire_endTransmission error ASK\n");
      return 1; // возвращаем код ошибки "1"
    }           // если есть ответ от слейва

  return 0; // возвращаем "0" - передача успешна
}

// Section : constructor

void
HD44780LCD_HD44780LCD (uint8_t NumRow, uint8_t NumCol, uint8_t I2Caddress)
{
  _NumRowsLCD = NumRow;
  _NumColsLCD = NumCol;
  _LCDSlaveAddresI2C = I2Caddress;
  //	  wire = twi;

  n_delay_ms (DISPLAY_DELAY_INIT);
}

// Section : Functions

//  Func Desc: Send data byte to  LCD via I2C
//  Param1: data byte
static void
HD44780LCD_PCF8574_LCDSendData (unsigned char data)
{
  unsigned char dataLower, dataUpper;
  uint8_t dataI2C[4];
  uint8_t TransmissionCode = 0;

  dataLower = (data << 4) & 0xf0; // select lower nibble by moving it to the
                                  // upper nibble position
  dataUpper = data & 0xf0;        // select upper nibble
  dataI2C[0] = dataUpper
               | (LCD_DATA_BYTE_ON
                  & _LCDBackLight); // enable=1 and rs =1 1101  YYYY-X-en-X-rs
  dataI2C[1] = dataUpper
               | (LCD_DATA_BYTE_OFF
                  & _LCDBackLight); // enable=0 and rs =1 1001 YYYY-X-en-X-rs
  dataI2C[2] = dataLower
               | (LCD_DATA_BYTE_ON
                  & _LCDBackLight); // enable=1 and rs =1 1101  YYYY-X-en-X-rs
  dataI2C[3] = dataLower
               | (LCD_DATA_BYTE_OFF
                  & _LCDBackLight); // enable=0 and rs =1 1001 YYYY-X-en-X-rs

  wire_beginTransmission (_LCDSlaveAddresI2C);
  wire_write (dataI2C, 4);
  TransmissionCode = wire_endTransmission ();
  if (TransmissionCode != 0)
    {
#ifdef LCD_SERIAL_DEBUG
      printf ("1203 : ");
      printf ("I2C error  wire.endTransmission : ");
      printf ("Tranmission code : ");
      printf ("0x%.2X\n", TransmissionCode);
      n_delay_ms (100);
#endif
    }
}

//  Func Desc: Send command byte to  lcd
//  Param1: command byte

static void
HD44780LCD_PCF8574_LCDSendCmd (unsigned char cmd)
{
  unsigned char cmdLower, cmdupper;
  uint8_t cmdI2C[4];
  uint8_t TransmissionCode = 0;

  cmdLower = (cmd << 4) & 0xf0; // select lower nibble by moving it to the
                                // upper nibble position
  cmdupper = cmd & 0xf0;        // select upper nibble
  cmdI2C[0]
      = cmdupper
        | (LCD_CMD_BYTE_ON
           & _LCDBackLight); // YYYY-1100 YYYY-led-en-rw-rs ,enable=1 and rs =0
  cmdI2C[1]
      = cmdupper
        | (LCD_CMD_BYTE_OFF
           & _LCDBackLight); // YYYY-1000 YYYY-led-en-rw-rs ,enable=0 and rs =0
  cmdI2C[2]
      = cmdLower
        | (LCD_CMD_BYTE_ON
           & _LCDBackLight); // YYYY-1100 YYYY-led-en-rw-rs ,enable=1 and rs =0
  cmdI2C[3]
      = cmdLower
        | (LCD_CMD_BYTE_OFF
           & _LCDBackLight); // YYYY-1000 YYYY-led-en-rw-rs ,enable=0 and rs =0

  wire_beginTransmission (_LCDSlaveAddresI2C);
  wire_write (cmdI2C, 4);
  TransmissionCode = wire_endTransmission ();
  if (TransmissionCode != 0)
    {
#ifdef LCD_SERIAL_DEBUG
      printf ("1202 : ");
      printf ("I2C error  wire.endTransmission : ");
      printf ("Tranmission code : ");
      printf ("0x%.2X\n", TransmissionCode);
      n_delay_ms (100);
#endif
    }
}

// Func Desc: Clear a line by writing spaces to every position
// Param1: enum LCDLineNumber_e lineNo  1-4

void
HD44780LCD_PCF8574_LCDClearLine (LCDLineNumber_e lineNo)
{

  switch (lineNo)
    {
    case LCDLineNumberOne:
      HD44780LCD_PCF8574_LCDSendCmd (LCD_LINE_ADR1);
      break;
    case LCDLineNumberTwo:
      HD44780LCD_PCF8574_LCDSendCmd (LCD_LINE_ADR2);
      break;
    case LCDLineNumberThree:
      if (_NumColsLCD == 20)
        HD44780LCD_PCF8574_LCDSendCmd (LCD_LINE_ADR3_20);
      else
        HD44780LCD_PCF8574_LCDSendCmd (LCD_LINE_ADR3_16);
      break;
    case LCDLineNumberFour:
      if (_NumColsLCD == 20)
        HD44780LCD_PCF8574_LCDSendCmd (LCD_LINE_ADR4_20);
      else
        HD44780LCD_PCF8574_LCDSendCmd (LCD_LINE_ADR4_16);
      break;
    }

  for (uint8_t i = 0; i < _NumColsLCD; i++)
    {
      HD44780LCD_PCF8574_LCDSendData (' ');
    }
}

// Func Desc: Clear screen by writing spaces to every position
// Note : See also LCDClearScreenCmd for software command  clear alternative.

void
HD44780LCD_PCF8574_LCDClearScreen (void)
{
  if (_NumRowsLCD < 1 || _NumRowsLCD > 4)
    {
      return;
    }

  HD44780LCD_PCF8574_LCDClearLine (LCDLineNumberOne);

  if (_NumRowsLCD >= 2)
    HD44780LCD_PCF8574_LCDClearLine (LCDLineNumberTwo);
  if (_NumRowsLCD >= 3)
    HD44780LCD_PCF8574_LCDClearLine (LCDLineNumberThree);
  if (_NumRowsLCD == 4)
    HD44780LCD_PCF8574_LCDClearLine (LCDLineNumberFour);
}

// Func Desc: Reset screen
// Param1: enum LCDCursorType_e cursor type, 4 choices

void
HD44780LCD_PCF8574_LCDResetScreen (LCDCursorType_e CursorType)
{
  HD44780LCD_PCF8574_LCDSendCmd (LCD_MODE_4BIT);
  HD44780LCD_PCF8574_LCDSendCmd (LCD_DISPLAY_ON);
  HD44780LCD_PCF8574_LCDSendCmd (CursorType);
  HD44780LCD_PCF8574_LCDSendCmd (LCD_CLRSCR);
  HD44780LCD_PCF8574_LCDSendCmd (LCDEntryModeThree);
  n_delay_ms (5);
}

// Func Desc: Turn Screen on and off
// Param1: passed bool, True = display on , false = display off

void
HD44780LCD_PCF8574_LCDDisplayON (bool OnOff)
{
  OnOff ? HD44780LCD_PCF8574_LCDSendCmd (LCD_DISPLAY_ON)
        : HD44780LCD_PCF8574_LCDSendCmd (LCD_DISPLAY_OFF);
  n_delay_ms (5);
}

// Func Desc: Initialise LCD
// Param1: enum LCDCursorType_e cursor type, 4 choices.

void
HD44780LCD_PCF8574_LCDInit (LCDCursorType_e CursorType)
{

  if (HD44780LCD_PCF8574_LCD_I2C_ON () != true)
    {
      n_delay_ms (2000);
      return;
    }

  n_delay_ms (15);
  HD44780LCD_PCF8574_LCDSendCmd (LCD_HOME);
  n_delay_ms (5);
  HD44780LCD_PCF8574_LCDSendCmd (LCD_HOME);
  n_delay_ms (5);
  HD44780LCD_PCF8574_LCDSendCmd (LCD_HOME);
  n_delay_ms (5);
  HD44780LCD_PCF8574_LCDSendCmd (LCD_MODE_4BIT);
  HD44780LCD_PCF8574_LCDSendCmd (LCD_DISPLAY_ON);
  HD44780LCD_PCF8574_LCDSendCmd (CursorType);
  HD44780LCD_PCF8574_LCDSendCmd (LCDEntryModeThree);
  HD44780LCD_PCF8574_LCDSendCmd (LCD_CLRSCR);
  n_delay_ms (5);
}

// Func Desc: Send string to LCD
// Param1: Pointer to the char array

void
HD44780LCD_PCF8574_LCDSendString (char *str)
{
  while (*str)
    HD44780LCD_PCF8574_LCDSendData (*str++);
}

// Func Desc: Sends a character to screen , simply wraps SendData command.
// Param1: Character to display
void
HD44780LCD_PCF8574_LCDSendChar (char data)
{
  HD44780LCD_PCF8574_LCDSendData (data);
}

// Func Desc: Moves cursor
// Param1. enum LCDDirectionType_e left or right
// Param2. uint8_t number of spaces to move

void
HD44780LCD_PCF8574_LCDMoveCursor (LCDDirectionType_e direction,
                                  uint8_t moveSize)
{
  uint8_t i = 0;
  if (direction == LCDMoveRight)
    {
      for (i = 0; i < moveSize; i++)
        {
          HD44780LCD_PCF8574_LCDSendCmd (LCD_MOV_CURSOR_RIGHT);
        }
    }
  else
    {
      for (i = 0; i < moveSize; i++)
        {
          HD44780LCD_PCF8574_LCDSendCmd (LCD_MOV_CURSOR_LEFT);
        }
    }
}

// Func Desc: Scrolls screen
// Param1. enum LCDDirectionType_e , left or right
// Param2. uint8_t number of spaces to scroll

void
HD44780LCD_PCF8574_LCDScroll (LCDDirectionType_e direction, uint8_t ScrollSize)
{
  uint8_t i = 0;
  if (direction == LCDMoveRight)
    {
      for (i = 0; i < ScrollSize; i++)
        {
          HD44780LCD_PCF8574_LCDSendCmd (LCD_SCROLL_RIGHT);
        }
    }
  else
    {
      for (i = 0; i < ScrollSize; i++)
        {
          HD44780LCD_PCF8574_LCDSendCmd (LCD_SCROLL_LEFT);
        }
    }
}
// Func Desc: moves cursor to an x , y position on display.
// Param1: enum LCDLineNumber_e  row 1-4
// Param2: uint8_t col 0-15
void
HD44780LCD_PCF8574_LCDGOTO (LCDLineNumber_e line, uint8_t col)
{
  switch (line)
    {
    case LCDLineNumberOne:
      HD44780LCD_PCF8574_LCDSendCmd (LCD_LINE_ADR1 | col);
      break;
    case LCDLineNumberTwo:
      HD44780LCD_PCF8574_LCDSendCmd (LCD_LINE_ADR2 | col);
      break;
    case LCDLineNumberThree:
      if (_NumColsLCD == 20)
        HD44780LCD_PCF8574_LCDSendCmd (LCD_LINE_ADR3_20 + col);
      else
        HD44780LCD_PCF8574_LCDSendCmd (LCD_LINE_ADR3_16 | col);
      break;
    case LCDLineNumberFour:
      if (_NumColsLCD == 20)
        HD44780LCD_PCF8574_LCDSendCmd (LCD_LINE_ADR4_20 + col);
      else
        HD44780LCD_PCF8574_LCDSendCmd (LCD_LINE_ADR4_16 | col);
      ;
      break;
    }
}

// Func Desc: Saves a custom character to a location in CG_RAM
// Param1: CG_RAM location 0-7 we only have 8 locations 0-7
// Param2: An array of 8 bytes representing a custom character data
void
HD44780LCD_PCF8574_LCDCreateCustomChar (uint8_t location, uint8_t *charmap)
{
  if (location >= 8)
    {
      return;
    }
  HD44780LCD_PCF8574_LCDSendCmd (LCD_CG_RAM | (location << 3));
  for (uint8_t i = 0; i < 8; i++)
    {
      HD44780LCD_PCF8574_LCDSendData (charmap[i]);
    }
}

// Func Desc: Turn LED backlight on and off
// Param1: passed bool True = LED on , false = display LED off
// Note: another data or command must be issued before it takes effect.
void
HD44780LCD_PCF8574_LCDBackLightSet (bool OnOff)
{
  OnOff ? (_LCDBackLight = LCD_BACKLIGHTON_MASK)
        : (_LCDBackLight = LCD_BACKLIGHTOFF_MASK);
}

// Func Desc: Setup I2C settings
// Returns false if PCF8574 fails to appear on I2C bus
// Returns true if ok
// Note if LCD_SERIAL_DEBUG enabled will print I2C error code to screen
//    wire.endTransmission() return:
//    0 : Success
//    1 : Data length error
//     2 : NACK on transmit of the address.
//     3 : NACK on transmit of the data.
//    4 :  Some other error
static bool
HD44780LCD_PCF8574_LCD_I2C_ON ()
{
  uint8_t TransmissionCode = 0;

  wire_begin ();
  // Wire.setClock(100000UL); // V 1.2.0
  wire_beginTransmission (_LCDSlaveAddresI2C);
  TransmissionCode = wire_endTransmission ();
  if (TransmissionCode != 0)
    {
#ifdef LCD_SERIAL_DEBUG
      printf ("1201 : ");
      printf ("I2C error  wire.endTransmission : ");
      printf ("Tranmission code : ");
      printf ("0x%.2X\n", TransmissionCode);
#endif
      return false; // Check if the PCF8574 is connected
    }
  else
    {
#ifdef LCD_SERIAL_DEBUG
      printf ("I2C Success Init : ");
#endif
      return true;
    }
}

// Print out a customer character from CGRAM
// Param1 CGRAM location 0-7
void
HD44780LCD_PCF8574_LCDPrintCustomChar (uint8_t location)
{
  if (location >= 8)
    {
      return;
    }
  HD44780LCD_PCF8574_LCDSendData (location);
}

// Called by print class, used to print out numerical data types etc
size_t
HD44780LCD_write (uint8_t c)
{
  HD44780LCD_PCF8574_LCDSendChar (c);
  return 1;
}

// Clear display using software command , set cursor position to zero
// See also LCDClearScreen for manual clear

void
HD44780LCD_PCF8574_LCDClearScreenCmd (void)
{
  HD44780LCD_PCF8574_LCDSendCmd (LCD_CLRSCR);
  n_delay_ms (3); // Requires a delay
}

// Set Cursor position to zero

void
HD44780LCD_PCF8574_LCDHome (void)
{
  HD44780LCD_PCF8574_LCDSendCmd (LCD_HOME);
  n_delay_ms (3); // Requires a delay
}

// Change entry mode
// Param1 enum LCDEntryMode_e 1-4 , 4 choices.

void
HD44780LCD_PCF8574_LCDChangeEntryMode (LCDEntryMode_e newEntryMode)
{
  HD44780LCD_PCF8574_LCDSendCmd (newEntryMode);
  n_delay_ms (3); // Requires a delay
}

static char LCD44780_buffer[21];
void
HD44780LCD_PCF8574_printf (char *format, ...)
{
  va_list args;

  va_start (args, format);
  vsnprintf (LCD44780_buffer, _NumColsLCD + 1, format, args);
  va_end (args);

  HD44780LCD_PCF8574_LCDSendString (LCD44780_buffer);

#ifdef LCD_SERIAL_DEBUG
  printf ("HD44780LCD_PCF8574_printf  %s \n", LCD44780_buffer);
#endif
}

// **** EOF ****
