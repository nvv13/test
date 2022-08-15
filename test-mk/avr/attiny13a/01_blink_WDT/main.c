/**
 * Copyright (c) 2016, Łukasz Marcin Podkalicki <lpodkalicki@gmail.com>
 * ATtiny13/005
 * Simple blinky with Watchdog Timer.
   https://blog.podkalicki.com/attiny13-blinky-with-watchdog-timer/
 */

//              Pinout of ATtiny13A 8-PDIP/SOIJ/SOIC
// ********************************************************************
// (PCINT5/RESET/ADC0/dW) PB5 1 ****8 VCC
//     (PCINT3/CLKI/ADC3) PB3 2*   *7 PB2 (SCK/ADC1/T0/PCINT2)
//          (PCINT4/ADC2) PB4 3*   *6 PB1 (MISO/AIN1/OC0B/INT0/PCINT1)
//                        GND 4*****5 PB0 (MOSI/AIN0/OC0A/PCINT0)
// ********************************************************************


#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define	LED_PIN	PB3

ISR(WDT_vect)
{

	PORTB ^= (1<<LED_PIN); // toggle LED pin
}

int
main(void)
{

	/* setup */
        DDRB |= (1<<LED_PIN); // set LED pin as OUTPUT
	PORTB = 0b00000000; // set all pins to LOW

	cli();//disable interrupt
	wdt_reset();//reset Watchdog timer
	/* Start timed sequence */
	WDTCR |= (1<<WDCE) | (1<<WDE);
	/* Set new prescaler(time-out) value = 64K cycles (~0.5 s) */
	WDTCR = (1<<WDE) | (1<<WDP2) | (1<<WDP0);
	sei(); // enable global interrupts

	/* loop */
	while (1);
}

