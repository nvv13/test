
//  Author: avishorp@gmail.com
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
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <TM1637Display.h>
#include "xtimer.h"

   void bitDelay(void);

   void start(void);

   void stop(void);

   uint8_t writeByte(uint8_t b);

   void showDots(uint8_t dots, uint8_t* digits);
  
   void showNumberBaseEx(int8_t base, uint16_t num, uint8_t dots , uint8_t leading_zero , uint8_t length , uint8_t pos );

#define TM1637_I2C_COMM1    0x40
#define TM1637_I2C_COMM2    0xC0
#define TM1637_I2C_COMM3    0x80

//
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D
const uint8_t digitToSegment[] = {
 // XGFEDCBA
  0b00111111,    // 0
  0b00000110,    // 1
  0b01011011,    // 2
  0b01001111,    // 3
  0b01100110,    // 4
  0b01101101,    // 5
  0b01111101,    // 6
  0b00000111,    // 7
  0b01111111,    // 8
  0b01101111,    // 9
  0b01110111,    // A
  0b01111100,    // b
  0b00111001,    // C
  0b01011110,    // d
  0b01111001,    // E
  0b01110001     // F
  };

static const uint8_t minusSegments = 0b01000000;

static 	gpio_t m_pinClk;
static 	gpio_t m_pinDIO;
static 	uint8_t m_brightness;
static 	unsigned int m_bitDelay;


void TM1637Display(gpio_t pinClk,gpio_t pinDIO, unsigned int bitDelay)
{
	// Copy the pin numbers
	m_pinClk = pinClk;
	m_pinDIO = pinDIO;
	m_bitDelay = bitDelay;

	// Set the pin direction and default value.
	// Both pins are set as inputs, allowing the pull-up resistors to pull them up
        gpio_init(m_pinClk,GPIO_IN);
        gpio_init(m_pinDIO,GPIO_IN);
        gpio_write(m_pinClk,0);	
        gpio_write(m_pinDIO,0);	
}

void setBrightness(uint8_t brightness, uint8_t on)
{
	m_brightness = (brightness & 0x7) | (on? 0x08 : 0x00);
}

void setSegments(const uint8_t segments[], uint8_t length, uint8_t pos)
{
    // Write COMM1
	start();
	writeByte(TM1637_I2C_COMM1);
	stop();

	// Write COMM2 + first digit address
	start();
	writeByte(TM1637_I2C_COMM2 + (pos & 0x03));

	// Write the data bytes
	for (uint8_t k=0; k < length; k++)
	  writeByte(segments[k]);

	stop();

	// Write COMM3 + brightness
	start();
	writeByte(TM1637_I2C_COMM3 + (m_brightness & 0x0f));
	stop();
}

void clear(void)
{
    uint8_t data[] = { 0, 0, 0, 0 };
	setSegments(data, 4, 0);
}

void showNumberDec(int num, uint8_t leading_zero, uint8_t length, uint8_t pos)
{
  showNumberDecEx(num, 0, leading_zero, length, pos);
}

void showNumberDecEx(int num, uint8_t dots, uint8_t leading_zero,
                                    uint8_t length, uint8_t pos)
{
  showNumberBaseEx(num < 0? -10 : 10, num < 0? -num : num, dots, leading_zero, length, pos);
}

void showNumberHexEx(uint16_t num, uint8_t dots, uint8_t leading_zero,
                                    uint8_t length, uint8_t pos)
{
  showNumberBaseEx(16, num, dots, leading_zero, length, pos);
}

void showNumberBaseEx(int8_t base, uint16_t num, uint8_t dots, uint8_t leading_zero,
                                    uint8_t length, uint8_t pos)
{
    uint8_t negative = false;
	if (base < 0) {
	    base = -base;
		negative = true;
	}


    uint8_t digits[4];

	if (num == 0 && !leading_zero) {
		// Singular case - take care separately
		for(uint8_t i = 0; i < (length-1); i++)
			digits[i] = 0;
		digits[length-1] = encodeDigit(0);
	}
	else {
		//uint8_t i = length-1;
		//if (negative) {
		//	// Negative number, show the minus sign
		//    digits[i] = minusSegments;
		//	i--;
		//}
		
		for(int i = length-1; i >= 0; --i)
		{
		    uint8_t digit = num % base;
			
			if (digit == 0 && num == 0 && leading_zero == false)
			    // Leading zero is blank
				digits[i] = 0;
			else
			    digits[i] = encodeDigit(digit);
				
			if (digit == 0 && num == 0 && negative) {
			    digits[i] = minusSegments;
				negative = false;
			}

			num /= base;
		}
    }
	
	if(dots != 0)
	{
		showDots(dots, digits);
	}
    
    setSegments(digits, length, pos);
}

void bitDelay(void)
{
	xtimer_usleep(m_bitDelay);
}

void start(void)
{
  gpio_init(m_pinDIO,GPIO_OUT);
  bitDelay();
}


void stop(void)
{
        gpio_init(m_pinDIO,GPIO_OUT);
	bitDelay();
        gpio_init(m_pinClk,GPIO_IN);
	bitDelay();
        gpio_init(m_pinDIO,GPIO_IN);
	bitDelay();
}


uint8_t writeByte(uint8_t b)
{
  uint8_t data = b;

  // 8 Data Bits
  for(uint8_t i = 0; i < 8; i++) {
    // CLK low
    gpio_init(m_pinClk,GPIO_OUT);
    bitDelay();

	// Set data bit
    if (data & 0x01)
      gpio_init(m_pinDIO,GPIO_IN);
    else
      gpio_init(m_pinDIO,GPIO_OUT);

    bitDelay();

	// CLK high
    gpio_init(m_pinClk,GPIO_IN);
    bitDelay();
    data = data >> 1;
  }

  // Wait for acknowledge
  // CLK to zero
  gpio_init(m_pinClk,GPIO_OUT);
  gpio_init(m_pinDIO,GPIO_IN);
  bitDelay();

  // CLK to high
  gpio_init(m_pinClk,GPIO_IN);
  bitDelay();
  uint8_t ack = gpio_read(m_pinDIO);
  if (ack == 0)
    gpio_init(m_pinDIO,GPIO_OUT);


  bitDelay();
  gpio_init(m_pinClk,GPIO_OUT);
  bitDelay();

  return ack;
}

void showDots(uint8_t dots, uint8_t* digits)
{
    for(int i = 0; i < 4; ++i)
    {
        digits[i] |= (dots & 0x80);
        dots <<= 1;
    }
}

uint8_t encodeDigit(uint8_t digit)
{
	return digitToSegment[digit & 0x0f];
}
