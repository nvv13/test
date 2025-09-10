// ********************************************************************
// source prg https://github.com/daniel1111/ArduinoCdPlayer.git
// ********************************************************************

#include "cplusplus.h"

#include "atapi.h"
#include "ide_io.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

enum ScanState
{
  RR,
  FF,
  NONE
};

#define LED_PIN PB7

extern "C" int
main (void)
{

  /* setup */
  enum ScanState _scan_state;
  ide_io _ide = ide_io ();
  atapi _atapi = atapi (&_ide);


  _atapi.reset ();

  /* loop */
  while (1)
    {
      _atapi.eject ();
      _delay_ms (3000); // wait 3 s
    }
}
