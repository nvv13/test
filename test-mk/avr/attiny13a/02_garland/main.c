//              Pinout of ATtiny13A 8-PDIP/SOIJ/SOIC
// ********************************************************************
// (PCINT5/RESET/ADC0/dW) PB5 1 ****8 VCC
//     (PCINT3/CLKI/ADC3) PB3 2*   *7 PB2 (SCK/ADC1/T0/PCINT2)
//          (PCINT4/ADC2) PB4 3*   *6 PB1 (MISO/AIN1/OC0B/INT0/PCINT1)
//                        GND 4*****5 PB0 (MOSI/AIN0/OC0A/PCINT0)
// ********************************************************************

#include "eeprom_attiny.h"
#include <avr/io.h>
#include <util/delay.h>

#define OUT_LED1 PB1
#define OUT_LED2 PB4
#define IN_BUTTON PB3

#define EEPROM_ADDR_SWITCH 0

static void
my_delay_us (int i_cnt)
{
  while (i_cnt--)
    _delay_us (1);
}

#define MAX_SWITCH 3
int
main (void)
{

  /* setup */
  DDRB |= (1 << OUT_LED1); // set pin as OUTPUT
  DDRB |= (1 << OUT_LED2);

  DDRB &= ~(1 << IN_BUTTON); // set pin as INPUT
  PORTB |= (1 << IN_BUTTON); // HiGh IN

  int i_cnt = 0;
  int i_but_delay = 0;
  int i_write_delay = 0;

  unsigned char i_switch = read_from_internal_eeprom (EEPROM_ADDR_SWITCH);
  if (i_switch > MAX_SWITCH)
    i_switch = 0;

  /* loop */
  while (1)
    {
      if ((PINB & (1 << IN_BUTTON)) == 0 && i_but_delay == 0) // Down!
        {
          i_but_delay = 100;
          i_cnt = 0;
          if (++i_switch > MAX_SWITCH)
            i_switch = 0;
          i_write_delay = 10000;
        }

      switch (i_switch)
        {

        case 0:
          {
            if (++i_cnt > 500)
              {
                PORTB |= (1 << OUT_LED1);
                PORTB &= ~(1 << OUT_LED2);
                my_delay_us (1000 - i_cnt);
                PORTB |= (1 << OUT_LED2);
                PORTB &= ~(1 << OUT_LED1);
                my_delay_us (i_cnt - 500);
                if (i_cnt >= 1000)
                  i_cnt = 0;
              }
            else
              {
                PORTB |= (1 << OUT_LED1);
                PORTB &= ~(1 << OUT_LED2);
                my_delay_us (i_cnt);
                PORTB |= (1 << OUT_LED2);
                PORTB &= ~(1 << OUT_LED1);
                my_delay_us (500 - i_cnt);
              }
          };
          break;

        case 1:
          {
            PORTB |= (1 << OUT_LED1);
            PORTB &= ~(1 << OUT_LED2);
            if (i_cnt < 100)
              _delay_ms (0);
            else
              _delay_ms (2);
            PORTB |= (1 << OUT_LED2);
            PORTB &= ~(1 << OUT_LED1);
            if (i_cnt < 100)
              _delay_ms (2);
            else
              _delay_ms (0);
            if (i_cnt++ > 200)
              i_cnt = 0;
          };
          break;

        case 2:
          {
            PORTB |= (1 << OUT_LED1);
            PORTB &= ~(1 << OUT_LED2);
            if (i_cnt < 500)
              _delay_ms (1);
            else
              _delay_ms (3);
            PORTB |= (1 << OUT_LED2);
            PORTB &= ~(1 << OUT_LED1);
            if (i_cnt < 500)
              _delay_ms (3);
            else
              _delay_ms (1);
            if (i_cnt++ > 1000)
              i_cnt = 0;
          };
          break;

        case 3:
          {
            PORTB |= (1 << OUT_LED1);
            PORTB &= ~(1 << OUT_LED2);
            if (i_cnt < 500)
              _delay_ms (0);
            else
              _delay_ms (2);
            PORTB |= (1 << OUT_LED2);
            PORTB &= ~(1 << OUT_LED1);
            if (i_cnt < 500)
              _delay_ms (2);
            else
              _delay_ms (0);
            if (i_cnt++ > 1000)
              i_cnt = 0;
          };
          break;

        } // end switch

      if (i_but_delay > 0)
        i_but_delay--;

      if (i_write_delay > 0)
        {
          i_write_delay--;
          if (i_write_delay == 0)
            {
              if (read_from_internal_eeprom (EEPROM_ADDR_SWITCH) != i_switch)
                write_internal_eeprom (EEPROM_ADDR_SWITCH, i_switch);
            }
        }
    }
}
