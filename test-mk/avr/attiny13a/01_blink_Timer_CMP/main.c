/**
   https://blog.podkalicki.com/attiny13-blinky-with-timer-ovf-overflow/
 */

//              Pinout of ATtiny13A 8-PDIP/SOIJ/SOIC
// ********************************************************************
// (PCINT5/RESET/ADC0/dW) PB5 1 ****8 VCC
//     (PCINT3/CLKI/ADC3) PB3 2*   *7 PB2 (SCK/ADC1/T0/PCINT2)
//          (PCINT4/ADC2) PB4 3*   *6 PB1 (MISO/AIN1/OC0B/INT0/PCINT1)
//                        GND 4*****5 PB0 (MOSI/AIN0/OC0A/PCINT0)
// ********************************************************************



#include <avr/io.h>
#include <avr/interrupt.h>

#define	LED_PIN	PB3

ISR(TIM0_COMPA_vect)
{

	PORTB ^= (1<<LED_PIN); // toggle LED pin
}

int
main(void)
{

	/* setup */
        DDRB |= (1<<LED_PIN); // set LED pin as OUTPUT
	PORTB = 0b00000000; // set all pins to LOW

        TCCR0A |= (1<<WGM01); // set timer counter mode to CTC
        TCCR0B |= (1<<CS02)|(1<<CS00); // set prescaler to 1024 (CLK=1200000Hz/1024/256=4.57Hz, 0.22s)
        OCR0A = 255; // set Timer's counter max value
        TIMSK0 |= (1<<OCIE0A); // enable Timer CTC interrupt

        sei(); // enable global interrupts


	/* loop */
	while (1);
}

