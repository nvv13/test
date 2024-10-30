/*
  источник
  https://github.com/openwch/ch32v003.git
*/

#include <stdbool.h>

#include "debug.h"

#include "CBlink.hpp"

/* Global Variable */
static volatile bool l_flag = false;

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
extern "C" int
main (void)
{
  SystemCoreClockUpdate ();
  Delay_Init ();
  USART_Printf_Init (115200);
  printf ("SystemClk:%d\r\n", SystemCoreClock);
  printf ("ChipID:%08x\r\n", DBGMCU_GetCHIPID ());

  NVIC_EnableIRQ (SysTicK_IRQn);
  SysTick->SR &= ~(1 << 0);
  SysTick->CMP = SystemCoreClock-1; // 1 секунда
  SysTick->CNT = 0;
  SysTick->CTLR = 0xF;

  CBlink ob_Blink1 = CBlink (GPIOA, GPIO_Pin_1, GPIO_Speed_50MHz);

  int i_count = 0;
  while (1)
    {
      if (l_flag)
        {
          __disable_irq ();
          l_flag = false;
          __enable_irq ();
          printf ("tic=%d\r\n", i_count++);
          ob_Blink1.Toggle ();
        }
    };
}

// void SysTick_Handler (void) __attribute__ ((interrupt
// ("WCH-Interrupt-fast")));
extern "C" void SysTick_Handler (void) __attribute__ ((interrupt ("machine")));
extern "C" void
SysTick_Handler (void)
{
  l_flag = true;
  SysTick->SR = 0;
}
