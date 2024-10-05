#ifndef MAX7219C_H
#define MAX7219C_H

#include "SPI_n.h"

#ifdef __cplusplus
extern "C"
{
#endif

  static const uint8_t sprite8x16[10][16] = {
    { 0b00000000, 0b01111000, 0b10000100, 0b10000100, 0b10000100, 0b10000100,
      0b10000100, 0b10000100, 0b10000100, 0b10000100, 0b10000100, 0b10000100,
      0b10000100, 0b01111000, 0b00000000, 0b00000000 } // 0
    ,
    { 0b00000000, 0b00000100, 0b00010100, 0b01000100, 0b00000100, 0b00000100,
      0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100,
      0b00000100, 0b00000100, 0b00000000, 0b00000000 } // 1
    ,
    { 0b00000000, 0b01111000, 0b00000100, 0b00000100, 0b00000100, 0b00000100,
      0b00000100, 0b01111000, 0b10000000, 0b10000000, 0b10000000, 0b10000000,
      0b10000000, 0b01111000, 0b00000000, 0b00000000 } // 2
    ,
    { 0b00000000, 0b11111000, 0b00000100, 0b00000100, 0b00000100, 0b00000100,
      0b00000100, 0b11111100, 0b00000100, 0b00000100, 0b00000100, 0b00000100,
      0b00000100, 0b11111000, 0b00000000, 0b00000000 } // 3
    ,
    { 0b00000000, 0b10000100, 0b10000100, 0b10000100, 0b10000100, 0b10000100,
      0b10000100, 0b01111100, 0b00000100, 0b00000100, 0b00000100, 0b00000100,
      0b00000100, 0b00000100, 0b00000000, 0b00000000 } // 4
    ,
    { 0b00000000, 0b01111000, 0b10000000, 0b10000000, 0b10000000, 0b10000000,
      0b10000000, 0b01111000, 0b00000100, 0b00000100, 0b00000100, 0b00000100,
      0b00000100, 0b01111000, 0b00000000, 0b00000000 } // 5
    ,
    { 0b00000000, 0b01111000, 0b10000000, 0b10000000, 0b10000000, 0b10000000,
      0b10000000, 0b11111000, 0b10000100, 0b10000100, 0b10000100, 0b10000100,
      0b10000100, 0b01111000, 0b00000000, 0b00000000 } // 6
    ,
    { 0b00000000, 0b11111000, 0b00000100, 0b00000100, 0b00000100, 0b00000100,
      0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100,
      0b00000100, 0b00000100, 0b00000000, 0b00000000 } // 7
    ,
    { 0b00000000, 0b01111000, 0b10000100, 0b10000100, 0b10000100, 0b10000100,
      0b10000100, 0b01111000, 0b10000100, 0b10000100, 0b10000100, 0b10000100,
      0b10000100, 0b01111000, 0b00000000, 0b00000000 } // 8
    ,
    { 0b00000000, 0b01111000, 0b10000100, 0b10000100, 0b10000100, 0b10000100,
      0b10000100, 0b01111100, 0b00000100, 0b00000100, 0b00000100, 0b00000100,
      0b00000100, 0b01111000, 0b00000000, 0b00000000 } // 9
  };

#define max7219_reg_noop 0x00
#define max7219_reg_digit0 0x01
#define max7219_reg_digit1 0x02
#define max7219_reg_digit2 0x03
#define max7219_reg_digit3 0x04
#define max7219_reg_digit4 0x05
#define max7219_reg_digit5 0x06
#define max7219_reg_digit6 0x07
#define max7219_reg_digit7 0x08
#define max7219_reg_decodeMode 0x09
#define max7219_reg_intensity 0x0a
#define max7219_reg_scanLimit 0x0b
#define max7219_reg_shutdown 0x0c
#define max7219_reg_displayTest 0x0f

  uint8_t
  reverse_bit_in_byte (uint8_t b)
  {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
  }

  static int const num_use_module = 8;
  static enum tls_io_name pin_cs;

  int
  setCommand (uint8_t command, uint8_t value)
  {
    tls_gpio_write (pin_cs, 0);
    for (int j = 0; j < num_use_module; j++)
      {
        // printf("set Command  = %i , value= %i.\n", (int)command,
        // (int)value);
        SPI_write (command);
        if (command >= max7219_reg_digit0 && command <= max7219_reg_digit7
            && j > 3)
          SPI_write (reverse_bit_in_byte (value));
        else
          SPI_write (value);
        // if(command>=max7219_reg_digit0 &&
        // command<=max7219_reg_digit7)buffer_matrix[j*8+command-1]=value;
      }
    tls_gpio_write (pin_cs, 1);
    return 0;
  }

  static uint8_t h_1 = 0;
  static uint8_t h_2 = 0;
  static uint8_t m_1 = 0;
  static uint8_t m_2 = 0;

  static uint8_t c_sec = 0;
  static uint8_t b_intensity = 0x06;

  int
  setCommand2 (uint8_t command)
  {
    if (!(command >= max7219_reg_digit0 && command <= max7219_reg_digit7))
      return 1;

    tls_gpio_write (pin_cs, 0);

    uint8_t value1 = 0;
    uint8_t value2 = 0;
    uint8_t value01 = 0;
    uint8_t value02 = 0;
    for (int j = 0; j < num_use_module; j++)
      {
        SPI_write (command); // позиция

        // Декодер, что же в эту позицию поставить!
        switch (j)
          {
          case 0:
          case 7:
            {
              value1 = sprite8x16[h_1][7 + command]; // botton
              value2 = sprite8x16[h_1][8 - command]; // top
            };
            break;
          case 1:
          case 6:
            {
              value1 = sprite8x16[h_2][7 + command]; // botton
              value2 = sprite8x16[h_2][8 - command]; // top
            };
            break;
          case 3:
          case 4:
            {
              value1 = sprite8x16[m_1][7 + command]; // botton
              value2 = sprite8x16[m_1][8 - command]; // top
            };
            break;
          case 2:
          case 5:
            {
              value1 = sprite8x16[m_2][7 + command]; // botton
              value2 = sprite8x16[m_2][8 - command]; // top
            };
            break;
          }

        // отодвинуть две последнии цифры, минуты, на цеферблате
        if ((j == 2 || j == 3 || j == 4 || j == 5))
          {
            value01 = value1 >> 2;
            value02 = value2 >> 2;
          }
        else
          {
            value01 = value1;
            value02 = value2;
          }

        // Делаем точечки :
        if (c_sec == 1)
          {
            if ((j == 2 || j == 5) && (command == 2 || command == 3))
              {
                value01 = value01 | 0b10000000;
                value02 = value02 | 0b10000000;
              }
            if ((j == 1 || j == 6) && (command == 2 || command == 3))
              {
                value01 = value01 | 0b00000001;
                value02 = value02 | 0b00000001;
              }
          }

        if (b_intensity > 15)
          {
            value01 = ~value01;
            value02 = ~value02;
          }

        // смотрим, нижную или верхнюю часть выводить...
        if (j > 3)
          SPI_write (reverse_bit_in_byte (value02));
        else
          SPI_write ((value01));
      }

    tls_gpio_write (pin_cs, 1);
    return 0;
  }

  void
  clear (void)
  {
    for (int i = 0; i < max7219_reg_digit7; i++)
      {                          // 8*8 dots
        setCommand (i + 1, 0x0); //
      }
  }

  int
  refresh (void)
  {
    setCommand (max7219_reg_scanLimit, 0x07);
    setCommand (max7219_reg_decodeMode,
                0x00); // using an led matrix (not digits)
    //
    if (true)
      setCommand (max7219_reg_shutdown, 0x01); // not in shutdown mode
    else
      setCommand (max7219_reg_shutdown, 0x00); // shutdown mode
    //
    setCommand (max7219_reg_displayTest, 0x00); // no display test
    setCommand (max7219_reg_intensity, b_intensity);
    // clear();
    for (int i = 0; i < max7219_reg_digit7; i++)
      { // 8*8 dots
        setCommand2 (i + 1);
      }
    return 0;
  }

  //-----------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif /* MAX7219C_H */
