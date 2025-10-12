// ********************************************************************
// test
// ********************************************************************

#include <avr/io.h>
#include <util/delay.h>

#define LED_PIN PB5 // build in led D13

int
main (void)
{

  /* setup */
  DDRD |= (1 << PD2); // set LED pin as OUTPUT
  DDRD |= (1 << PD3);

  int i_cnt=0;
  /* loop */
  while (1)
    {
      PORTD |= (1 << PD2); 
      PORTD &= ~(1 << PD3);
      if(i_cnt<500)
        _delay_ms (1);       // 520Hz
        else
        _delay_ms (3);       // 520Hz
      PORTD |= (1 << PD3); 
      PORTD &= ~(1 << PD2); 
      if(i_cnt<500)
        _delay_ms (3);       
        else
        _delay_ms (1);       // 520Hz
      if(i_cnt++>1000)
         i_cnt=0;
    }
}
