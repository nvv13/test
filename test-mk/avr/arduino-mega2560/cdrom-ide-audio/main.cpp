// ********************************************************************
// source prg https://github.com/daniel1111/ArduinoCdPlayer.git
// ********************************************************************


#include "cplusplus.h"

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "ide_io.h"
#include "atapi.h"

enum ScanState
{
  RR,
  FF,
  NONE
};

enum ScanState _scan_state;
ide_io *_ide = new ide_io();
atapi  *_atapi = new atapi(_ide);

#define LED_PIN PB7

extern "C" int
main(void)
{

    /* setup */


  _atapi->reset();

    /* loop */
    while (1) {
        _atapi->eject();
        _delay_ms(3000); // wait 3 s
    }
}
