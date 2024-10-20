/*

*/

#include "debug.h"

#include "CBlink.hpp"

extern "C" int
main (void)
{

  NVIC_PriorityGroupConfig (NVIC_PriorityGroup_1);
  Delay_Init ();
  Delay_Ms (100);
  USART_Printf_Init (115200);
  printf ("SystemClk:%ld\r\n", SystemCoreClock);

  SystemCoreClockUpdate ();
  printf ("SystemClk:%d\r\n", SystemCoreClock);

  printf ("GPIO Toggle TEST\r\n");
  CBlink ob_Blink1 = CBlink (GPIOA, GPIO_Pin_1, GPIO_Speed_50MHz);
  CBlink ob_Blink2 = CBlink (GPIOA, GPIO_Pin_2, GPIO_Speed_50MHz);

  while (1)
    {
      Delay_Ms (500);
      ob_Blink1.Toggle ();
      ob_Blink2.Toggle ();
      Delay_Ms (200);
      ob_Blink2.Toggle ();
    }
}
