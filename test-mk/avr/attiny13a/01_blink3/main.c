//              Pinout of ATtiny13A 8-PDIP/SOIJ/SOIC
// ********************************************************************
// (PCINT5/RESET/ADC0/dW) PB5 1 ****8 VCC
//     (PCINT3/CLKI/ADC3) PB3 2*   *7 PB2 (SCK/ADC1/T0/PCINT2)
//          (PCINT4/ADC2) PB4 3*   *6 PB1 (MISO/AIN1/OC0B/INT0/PCINT1)
//                        GND 4*****5 PB0 (MOSI/AIN0/OC0A/PCINT0)
// ********************************************************************


#include <avr/io.h>
#include <util/delay.h>


int
main(void)
{

  /* setup */
  DDRB |= (1 << PB4); // set pin as OUTPUT
  DDRB |= (1 << PB3);

  int i_cnt=0;
  /* loop */
  while (1)
    {
      PORTB |= (1 << PB4); 
      PORTB &= ~(1 << PB3);
      if(i_cnt<500)
        _delay_ms (1);       
        else
        _delay_ms (3);       
      PORTB |= (1 << PB3); 
      PORTB &= ~(1 << PB4); 
      if(i_cnt<500)
        _delay_ms (3);       
        else
        _delay_ms (1);       
      if(i_cnt++>1000)
         i_cnt=0;
    }


}
