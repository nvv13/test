// ********************************************************************
// source prg https://github.com/daniel1111/ArduinoCdPlayer.git
// ********************************************************************

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "cplusplus.h"
#include "CShell.hpp"

#include "atapi.h"
#include "ide_io.h"

enum ScanState
{
  RR,
  FF,
  NONE
};

CShell *PoCShell = NULL;
atapi  *P_atapi = NULL;

#define LED_PIN PB7

int
cmd_eject (int argc, char **argv)
{
  P_atapi->eject ();
  printf("=eject\r\n");
  return 0;
};


static const shell_command_t shell_commands[] = {
  //{ "init", "Setup a particular SPI configuration", cmd_init },
  { "eject",  "eject", cmd_eject },
//  { "off", "set LED Off", led_off },
  { NULL, NULL, NULL }
};


extern "C" int
main (void)
{

  /* setup */
  enum ScanState _scan_state;
  ide_io _ide = ide_io ();
  atapi _atapi = atapi (&_ide);
  P_atapi = &_atapi;

  _atapi.reset ();

  if (_atapi.is_atapi_device())
    printf("Found ATAPI Dev.\r\n");
  else
  {
    printf("No ATAPI Device!\r\n");
    while(1);                          // No need to go ahead.
  }

  CShell oCShell = CShell (shell_commands);
  PoCShell = &oCShell;
  printf(" enter help for usage\r\n");
  while (1)
    {
      oCShell.Idle ();
    }

  /* loop */
  while (1)
    {
      _delay_ms (3000); // wait 3 s
    }
}
