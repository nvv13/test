//              Pinout of ATtiny13A 8-PDIP/SOIJ/SOIC
// ********************************************************************
// (PCINT5/RESET/ADC0/dW) PB5 1 ****8 VCC
//     (PCINT3/CLKI/ADC3) PB3 2*   *7 PB2 (SCK/ADC1/T0/PCINT2)
//          (PCINT4/ADC2) PB4 3*   *6 PB1 (MISO/AIN1/OC0B/INT0/PCINT1)
//                        GND 4*****5 PB0 (MOSI/AIN0/OC0A/PCINT0)
// ********************************************************************


#include <avr/io.h>
#include <util/delay.h>

#define LED_PIN PB3

int
main(void)
{

    /* setup */
    DDRB |= _BV(LED_PIN); // set LED pin as OUTPUT

    /* loop */
    while (1) {
        PORTB ^= _BV(LED_PIN); // toggle LED pin
        _delay_ms(500); // wait 0.5s
    }
}
