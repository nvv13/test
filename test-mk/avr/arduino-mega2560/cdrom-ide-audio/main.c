// ********************************************************************
// test
// ********************************************************************


#include <avr/io.h>
#include <util/delay.h>

#define LED_PIN PB7

int
main(void)
{

    /* setup */
    //DDRB |= _BV(LED_PIN); // set LED pin as OUTPUT
      DDRB |= (1<<LED_PIN); // set LED pin as OUTPUT

    /* loop */
    while (1) {
        //PORTB ^= _BV(LED_PIN); // toggle LED pin
	  PORTB ^= (1<<LED_PIN); // toggle LED pin
        _delay_ms(500); // wait 0.5s
    }
}
