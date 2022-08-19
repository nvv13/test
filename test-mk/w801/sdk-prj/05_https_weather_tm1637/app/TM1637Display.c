
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
//#include <inttypes.h>
#include "wm_gpio.h"

#include <TM1637Display.h>

   static void bitDelay(void);

   static void start(void);

   static void stop(void);

   static uint8_t writeByte(uint8_t b);

   static void showDots(uint8_t dots, uint8_t* digits);
  
   static void showNumberBaseEx(int8_t base, uint16_t num, uint8_t dots , uint8_t leading_zero , uint8_t length , uint8_t pos );

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
static const uint8_t digitToSegment[] = {
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

static 	enum tls_io_name m_pinClk;
static 	enum tls_io_name m_pinDIO;
static 	uint8_t m_brightness;
static 	unsigned int m_bitDelay;


void TM1637Display(enum tls_io_name pinClk,enum tls_io_name pinDIO, unsigned int bitDelay)
{
        m_bitDelay = bitDelay;

	// Copy the pin numbers
	m_pinClk = pinClk;
	m_pinDIO = pinDIO;
	m_bitDelay = bitDelay;

	// Set the pin direction and default value.
	// Both pins are set as inputs, allowing the pull-up resistors to pull them up
	tls_gpio_cfg(m_pinClk, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);// 1
	tls_gpio_cfg(m_pinDIO, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);// 1

	//tls_gpio_write(m_pinClk, 0);
	//tls_gpio_write(m_pinDIO, 0);
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

static void showNumberBaseEx(int8_t base, uint16_t num, uint8_t dots, uint8_t leading_zero,
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


static void start(void)
{
  tls_gpio_cfg(m_pinDIO, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);// 0
  tls_gpio_write(m_pinDIO, 0);
  bitDelay();
}


static void stop(void)
{
        tls_gpio_cfg(m_pinDIO, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);// 0
        tls_gpio_write(m_pinDIO, 0);
	bitDelay();
	tls_gpio_cfg(m_pinClk, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);// 1
	bitDelay();
        tls_gpio_cfg(m_pinDIO, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);// 1
	bitDelay();
}


static uint8_t writeByte(uint8_t b)
{
  uint8_t data = b;

  // 8 Data Bits
  for(uint8_t i = 0; i < 8; i++) {
    // CLK low
    tls_gpio_cfg(m_pinClk, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);// 0
    tls_gpio_write(m_pinClk, 0);
    bitDelay();

	// Set data bit
    if (data & 0x01)
      tls_gpio_cfg(m_pinDIO, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);// 1
    else
      {
      tls_gpio_cfg(m_pinDIO, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);// 0
      tls_gpio_write(m_pinDIO, 0);
      }

    bitDelay();

	// CLK high
    tls_gpio_cfg(m_pinClk, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);// 1
    bitDelay();
    data = data >> 1;
  }

  // Wait for acknowledge
  // CLK to zero
  tls_gpio_cfg(m_pinClk, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);// 0
  tls_gpio_write(m_pinClk, 0);
  tls_gpio_cfg(m_pinDIO, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);// 1
  bitDelay();

  // CLK to high
  tls_gpio_cfg(m_pinClk, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);// 1
  bitDelay();
  uint8_t ack = tls_gpio_read(m_pinDIO);
  if (ack == 0)
    { 
    tls_gpio_cfg(m_pinDIO, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);// 0
    tls_gpio_write(m_pinDIO, 0);
    }


  bitDelay();
  tls_gpio_cfg(m_pinClk, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);// 0
  tls_gpio_write(m_pinClk, 0);
  bitDelay();

  return ack;
}

static void showDots(uint8_t dots, uint8_t* digits)
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

uint8_t encodeSign(uint8_t sign)
{
if(sign & 0x01)
  return minusSegments;
  else
  return 0x00;
}


#include "csi_core.h"
#include "wm_cpu.h"
extern uint32_t csi_coret_get_load(void);
extern uint32_t csi_coret_get_value(void);
static void bitDelay(void)
{
    uint32_t load = csi_coret_get_load();
    uint32_t start = csi_coret_get_value();
    uint32_t cur;
    uint32_t cnt;
    tls_sys_clk sysclk;

    tls_sys_clk_get(&sysclk);
    cnt = sysclk.cpuclk * m_bitDelay;

    while (1) {
        cur = csi_coret_get_value();

        if (start > cur) {
            if (start - cur >= cnt) {
                return;
            }
        } else {
            if (load - cur + start > cnt) {
                return;
            }
        }
    }
}

