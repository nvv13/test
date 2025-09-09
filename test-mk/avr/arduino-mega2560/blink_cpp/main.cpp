// ********************************************************************
// test
// ********************************************************************

#include "cplusplus.h"

#include <avr/io.h>
#include <util/delay.h>

#define LED_PIN PB7 // build in led D13

class CBlink
{

public:
  CBlink ()
  {
    /* setup */
    DDRB |= (1<<LED_PIN); // set LED pin as OUTPUT
  }
  void
  Toggle (void)
  {
	  PORTB ^= (1<<LED_PIN); // toggle LED pin
  }
};



//extern "C" int
int main(void)
{

    /* setup */
    CBlink* x1=new CBlink();

    /* loop */
    while (1) {
        x1->Toggle();
        _delay_ms(500); // wait 0.5s
    }

   delete x1;
}
