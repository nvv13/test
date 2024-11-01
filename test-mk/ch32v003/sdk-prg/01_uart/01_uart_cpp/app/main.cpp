/*
  источник
  https://github.com/openwch/ch32v003.git
*/

#include "debug.h"

#include "CShell.hpp"

#include "CBlink.hpp"

CBlink* Pob_Blink1 = NULL;

int
led_on (int argc, char **argv)
{
  Pob_Blink1->On ();
  printf ("ob_Blink1.On()\r\n");
  return 0;
};

int
led_off (int argc, char **argv)
{
  Pob_Blink1->Off ();
  printf ("ob_Blink1.Off()\r\n");
  return 0;
};

static const shell_command_t shell_commands[] = {
  //{ "init", "Setup a particular SPI configuration", cmd_init },
  { "led-on", "set LED On", led_on },
  { "led-off", "set LED Off", led_off },
  { NULL, NULL, NULL }
};

int
main (void)
{
  NVIC_PriorityGroupConfig (NVIC_PriorityGroup_1);
  Delay_Init ();
  //USART_Printf_Init (115000);
  CBlink ob_Blink1 = CBlink (PA_01);
  Pob_Blink1=&ob_Blink1;
  CShell oCShell = CShell (shell_commands);
  printf ("SystemClk1:%d\r\n", SystemCoreClock);
  printf ("enter help for usage\r\n");



  while (1)
    {
      oCShell.Idle ();
    }
}
