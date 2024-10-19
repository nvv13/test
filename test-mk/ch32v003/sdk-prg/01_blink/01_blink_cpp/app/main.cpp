/********************************** (C) COPYRIGHT
 ******************************** File Name          : main.c Author : WCH
 * Version            : V1.0.0
 * Date               : 2021/06/06
 * Description        : Main program body.
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

#include "debug.h"

class CBlink
{
private:
  GPIO_TypeDef *port;
  GPIO_InitTypeDef GPIO_InitStructure = { 0 };
  u8 u8_led_state;

public:
  CBlink (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIOSpeed_TypeDef GPIO_Speed)
  {
    port = GPIOx;
    u8_led_state = 0;
    if (port == GPIOA)
      RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE);
    if (port == GPIOC)
      RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC, ENABLE);
    if (port == GPIOD)
      RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed;
    GPIO_Init (port, &GPIO_InitStructure);
    Off ();
  }
  void
  Toggle (void)
  {
    u8_led_state = ~u8_led_state;
    if (u8_led_state)
      On ();
    else
      Off ();
  }
  void
  On (void)
  {
    GPIO_WriteBit (port, GPIO_InitStructure.GPIO_Pin, Bit_SET);
  }
  void
  Off (void)
  {
    GPIO_WriteBit (port, GPIO_InitStructure.GPIO_Pin, Bit_RESET);
  }
};

extern "C" int
main (void)
{
  RCC_ClocksTypeDef RCC_ClocksStatus = { 0 };

  NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2);
  Delay_Init ();
  Delay_Ms (100);
  USART_Printf_Init (115200);
  printf ("SystemClk:%ld\r\n", SystemCoreClock);

  SystemCoreClockUpdate ();
  printf ("SystemClk:%d\r\n", SystemCoreClock);

#if 1
  RCC_GetClocksFreq (&RCC_ClocksStatus);
  printf ("SYSCLK_Frequency-%d\r\n", RCC_ClocksStatus.SYSCLK_Frequency);
  printf ("HCLK_Frequency-%d\r\n", RCC_ClocksStatus.HCLK_Frequency);
  printf ("PCLK1_Frequency-%d\r\n", RCC_ClocksStatus.PCLK1_Frequency);
  printf ("PCLK2_Frequency-%d\r\n", RCC_ClocksStatus.PCLK2_Frequency);

#endif

  union
  {
    short s;
    char c[sizeof (short)];
  } un;
  un.s = 0x0102;
  printf (" CPU: - ");
  if (sizeof (short) == 2)
    {
      if (un.c[0] == 1 && un.c[1] == 2)
        printf ("big-endiann \n");
      else if (un.c[0] == 2 && un.c[1] == 1)
        printf ("little-endiann \n");
      else
        printf ("unknownn \n");
    }
  else
    printf ("sizeof(short) = %d \n", sizeof (short));

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
