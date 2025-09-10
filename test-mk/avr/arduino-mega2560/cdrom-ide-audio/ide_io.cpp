/* #######################################################################

    IDE ATAPI controller and DAC for an Arduino Pro-Mini

    Copyright (C) 2020  Daniel Swann

    Heavily based on the ATAPIDUINO project:
    Copyright (C) 2012  Carlos Durandal

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

 * #######################################################################


IDE	           	      IDE             IDE
/reset   	   PL5 D44 -  1 2  - GND
Data7	 	   PA7 D29 -  3 4  - PC0 D37  Data8
    .	 	   PA6 D28 -  5 6  - PC1 D36  .
    .	 	   PA5 D27 -  7 8  - PC2 D35  .
    .	 	   PA4 D26 -  9 10 - PC3 D34  .
    .	 	   PA3 D25 - 11 12 - PC4 D33  .
    .	 	   PA2 D24 - 13 14 - PC5 D32  .
    .    	   PA1 D23 - 15 16 - PC6 D31  .
Data0	           PA0 D22 - 17 18 - PC7 D30  Data15
                       GND - 19 20 -x          KEY
DMARQ	                  x- 21 22 - GND
Write Strobe /DIOW PL6 D43 - 23 24 - GND
Read Strobe  /DIOR PL7 D42 - 25 26 - GND
I/O Ready                 x- 27 28 -x         SPSYNC:CSEL
/DMACK                    x- 29 30 - GND
INTRQ                     x- 31 32 -x
Address 1  	   PL1 D48 - 33 34 -x
Address 0          PL0 D49 - 35 36 - PL2 D47  Address 2
(1F0-1F7) /IDE_CS0 PL3 D46 - 37 38 - PL4 D45  /IDE_CS1 (3F6-3F7)
          /ACTIVE         x- 39 40 - GND
IDE	           	      IDE             IDE



*/

#include "ide_io.h"
#include <util/delay.h>

#define IO_EXP_DATALINES 0x22
#define IO_EXP_REG_SEL 0x20

#define CTL_MASK 0xE0

#define NXP_INPUT 0
#define NXP_OUTPUT 2
#define NXP_INVERT 4
#define NXP_CONFIG 6

// Corresponds to the bit position on the IO_EXP_REG_SEL port expander
#define CTL_A0 0
#define CTL_A1 1
#define CTL_A2 2
#define CTL_CS1FX 3
#define CTL_CS3FX 4
#define CTL_RST 5
#define CTL_DIOW 6
#define CTL_DIOR 7

ide_io::ide_io ()
{
  _control_reg = 0xF8;
  _debug_ide_rw = 0;
  DDRA = 0x00;  // set all 8 pin portA as INPUT
  PORTA = 0xff; // pull-up inputs
  DDRC = 0x00;  // set all 8 pin portC as INPUT
  PORTC = 0xff; // pull-up inputs
  DDRL = 0x00;  // set all 8 pin portL as INPUT
  PORTL = 0xff; // pull-up inputs
}

ide_io::data16
ide_io::read (uint8_t regval)
{
  data16 retval;

  if (_debug_ide_rw)
    {
      // Serial.print(F("--- IDE READ regval="));
      // Serial.print(regval);
    }

  // Set datalines to inputs
  DDRA = 0x00;  // set all 8 pin portA as INPUT
  PORTA = 0xff; // pull-up inputs
  DDRC = 0x00;  // set all 8 pin portC as INPUT
  PORTC = 0xff; // pull-up inputs

  regval = regval & ~CTL_MASK; // Don't allow regval to set DIOR/DIOW/RST
  _control_reg = _control_reg & CTL_MASK; // Clear CS3FX/CS1FX/A2/A1/A0
  _control_reg = _control_reg
                 | regval; // Set CS3FX/CS1FX/A2/A1/A0 from regval passed in
  _control_reg = _control_reg & ~(1 << CTL_DIOR); // Set DIOR low

  DDRL = 0xff; // set all 8 pin portL as OUTPUT
  PORTL = _control_reg;

  retval.low = PINA;
  retval.high = PINC;

  _control_reg
      = _control_reg | (1 << CTL_DIOR); // Set DIOR high to signal end of read
  PORTL = _control_reg;

  if (_debug_ide_rw)
    {
      // Serial.print(F(". Finished IDE READ"));
      // Serial.print(F(", dataLval="));
      // Serial.print(retval.low);
      // Serial.print(F(", dataHval = "));
      // Serial.println(retval.high);
    }

  return retval;
}

void
ide_io::write (uint8_t regval, uint8_t dataLval, uint8_t dataHval)
{
  uint8_t reg_inital = _control_reg;

  regval = regval & ~CTL_MASK; // Don't allow regval to set DIOR/DIOW/RST
  _control_reg = _control_reg & CTL_MASK; // Clear CS3FX/CS1FX/A2/A1/A0
  _control_reg = _control_reg
                 | regval; // Set CS3FX/CS1FX/A2/A1/A0 from regval passed in

  // Make sure DIOW is high to start with
  _control_reg = _control_reg | (1 << CTL_DIOW);
  DDRL = 0xff;          // set all 8 pin portL as OUTPUT
  PORTL = _control_reg; // sets register address and DIOW low

  if (_debug_ide_rw)
    {
      // Serial.print(F("--- IDE WRITE _control_reg inital="));
      // Serial.print(reg_inital, BIN);
      // Serial.print(F(", regval="));
      // Serial.print(regval, BIN);
      // Serial.print(F(", NEW _control_reg="));
      // Serial.print(_control_reg, BIN);
      // Serial.print(F(", dataLval="));
      // Serial.print(dataLval);
      // Serial.print(F(", dataHval = "));
      // Serial.print(dataHval);
    }

  // set datalines as outputs
  DDRA = 0xff; // set all 8 pin portA as OUTPUT
  DDRC = 0xff; // set all 8 pin portC as OUTPUT

  // write out data
  PORTA = dataLval;
  PORTC = dataHval;

  // Set DIOW low to signal write
  _control_reg = _control_reg & ~(1 << CTL_DIOW);
  PORTL = _control_reg;

  // Set DIOW high to signal end of write
  _control_reg = _control_reg | (1 << CTL_DIOW);
  PORTL = _control_reg;

  // set datalines back to inputs (highz)
  DDRA = 0x00;  // set all 8 pin portA as INPUT
  PORTA = 0xff; // pull-up inputs
  DDRC = 0x00;  // set all 8 pin portC as INPUT
  PORTC = 0xff; // pull-up inputs

  if (_debug_ide_rw)
    {
      // Serial.print(F(". IDE WRITE Done, set control reg = "));
      // Serial.println(_control_reg, BIN);
    }
}

void
ide_io::reset ()
{
  DDRL = 0xff; // set all 8 pin portL as OUTPUT

  // Set RST low
  PORTL = (uint8_t)0b11011111;

  _delay_ms (40);

  // Set RST high
  _control_reg = 0xFF;
  PORTL = _control_reg;

  _delay_ms (20);
}
