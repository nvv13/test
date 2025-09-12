/*
src
https://stackoverflow.com/questions/40705348/mega2560-timer-and-usec

supposed! #define F_CPU 16000000L

*/

#pragma once


#include <avr/interrupt.h>
#include <stdio.h>

class SystemTime
{
private:
  unsigned long ovfCount = 0;
  unsigned long ovfCount250 = 0;

public:
  SystemTime ();
  void Overflow ();
  uint32_t Millis ();
};

/*
 *   Constructor, Initializes the System Timer for keeping track
 * of the time since start.
 */
SystemTime::SystemTime ()
{
  //TCCR4B |= (1 << CS41);  //Set Prescale to 8
  TCCR4B |= (1 << CS40) | (1 << CS41); // Set Prescale to 64
  //TCCR4B |= (1 << CS42);  //Set Prescale to 256
  //TCCR4B |= (1 << CS42) | (1 << CS40);  //Set Prescale to 1024
  TIMSK4 |= (1 << TOIE4); // Enable the Overflow Interrupt

  TCNT4 = 0;

  // Enable Interrupts
  sei ();
}

/**
 * Increase the Overflow count
     TCNT4=65535 - Overflow!
 */
void
SystemTime::Overflow ()
{
  this->ovfCount++;
}

/**
 * Returns the number of Miliseconds since start
 */
uint32_t
SystemTime::Millis ()
{
  uint32_t t;

  // Set Prescale to 64 -> 16000000/64=>250000 sec,
  // div to 250 = 1 ms (1000 sec)
  t = (TCNT4 + this->ovfCount*65535) / 250;
  return t;
}

SystemTime sysTime;

ISR (TIMER4_OVF_vect) { sysTime.Overflow (); }
