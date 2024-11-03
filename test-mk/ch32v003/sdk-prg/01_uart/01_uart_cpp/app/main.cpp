/*
  источник
  https://github.com/openwch/ch32v003.git
*/

#include "debug.h"

#include "CShell.hpp"

#include "CBlink.hpp"

CBlink *Pob_Blink1 = NULL;
CShell *PoCShell = NULL;

int
led_on (int argc, char **argv)
{
  Pob_Blink1->On ();
  PoCShell->oUsart->SendPSZstring ("ob_Blink1.On()\r\n");
  return 0;
};

int
led_off (int argc, char **argv)
{
  Pob_Blink1->Off ();
  PoCShell->oUsart->SendPSZstring ("ob_Blink1.Off()\r\n");
  return 0;
};

static const shell_command_t shell_commands[] = {
  //{ "init", "Setup a particular SPI configuration", cmd_init },
  { "on", "set LED On", led_on },
  { "off", "set LED Off", led_off },
  { NULL, NULL, NULL }
};

int
main (void)
{
  NVIC_PriorityGroupConfig (NVIC_PriorityGroup_1);
  Delay_Init ();
  CBlink ob_Blink1 = CBlink (PA_01);
  Pob_Blink1 = &ob_Blink1;
  CShell oCShell = CShell (shell_commands);
  PoCShell = &oCShell;
  oCShell.oUsart->SendPSZstring (" enter help for usage\r\n");
  while (1)
    {
      oCShell.Idle ();
    }
}
