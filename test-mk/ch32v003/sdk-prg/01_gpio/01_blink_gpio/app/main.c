/*
  источник
  https://github.com/openwch/ch32v003.git
*/


#include "debug.h"

void
GPIO_Toggle_init (void)
{
  GPIO_InitTypeDef GPIO_InitStructure = { 0 };

  RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init (GPIOA, &GPIO_InitStructure);
}


int
main (void)
{
  u8 i = 0;

  NVIC_PriorityGroupConfig (NVIC_PriorityGroup_1);
  Delay_Init ();
  Delay_Ms (100);
  SystemCoreClockUpdate ();

  GPIO_Toggle_init ();

  while (1)
    {
      Delay_Ms (1000);
      GPIO_WriteBit (GPIOA, GPIO_Pin_1,
                     (i == 0) ? (i = Bit_SET) : (i = Bit_RESET));
    }
}
