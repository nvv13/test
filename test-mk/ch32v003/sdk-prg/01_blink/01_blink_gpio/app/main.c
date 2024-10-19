/********************************** (C) COPYRIGHT
 ******************************** File Name          : main.c Author : WCH
 * Version            : V1.0.0
 * Date               : 2021/06/06
 * Description        : Main program body.
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

#include "debug.h"

/* Global define */

/* Global Variable */

/*********************************************************************
 * @fn      GPIO_Toggle_init
 *
 * @brief   Initializes GPIOA.0
 *
 * @return  none
 */
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

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int
main (void)
{
  u8 i = 0;
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
  GPIO_Toggle_init ();

  while (1)
    {
      Delay_Ms (1000);
      GPIO_WriteBit (GPIOA, GPIO_Pin_1,
                     (i == 0) ? (i = Bit_SET) : (i = Bit_RESET));
    }
}
