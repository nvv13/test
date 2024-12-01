/*
  источник
  https://github.com/openwch/ch32v20x.git
*/


#include "debug.h"

#include "CBlink.hpp"

extern "C" int
main (void)
{

  //NVIC_PriorityGroupConfig (NVIC_PriorityGroup_1);
  Delay_Init ();
  Delay_Ms (100);

  SystemCoreClockUpdate ();
  CBlink ob_Blink1 = CBlink (PA_01);
  CBlink ob_Blink2 = CBlink (PB_02);

  while (1)
    {
      ob_Blink1.On ();
      Delay_Ms (100);
      ob_Blink1.Off ();
      ob_Blink2.Toggle ();
      Delay_Ms (500);
      ob_Blink2.Toggle ();
    }
}
