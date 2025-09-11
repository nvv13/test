// ********************************************************************
// test
// ********************************************************************

#include "cplusplus.h"
#include "CShell.hpp"
#include <avr/io.h>

#define LED_PIN PB7 // build in led D13

CShell *PoCShell = NULL;

int
led_on (int argc, char **argv)
{
  PORTB |= (1 << LED_PIN); // off
  printf("=LED On\r\n");
  return 0;
};

int
led_off (int argc, char **argv)
{
  PORTB &= ~(1 << LED_PIN); // off
  printf("=LED Off\r\n");
  return 0;
};


static const shell_command_t shell_commands[] = {
  //{ "init", "Setup a particular SPI configuration", cmd_init },
  { "on", "set LED On", led_on },
  { "off", "set LED Off", led_off },
  { NULL, NULL, NULL }
};


extern "C" int
main (void)
{

  /* setup */
  DDRB |= (1 << LED_PIN); // set LED pin as OUTPUT
  PORTB &= ~(1 << LED_PIN); // off

  CShell oCShell = CShell (shell_commands);
  PoCShell = &oCShell;
  printf(" enter help for usage\r\n");
  while (1)
    {
      oCShell.Idle ();
    }

}
