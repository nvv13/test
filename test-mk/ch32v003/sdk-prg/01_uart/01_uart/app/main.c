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
vu8 val;

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
 * @fn      USARTx_CFG
 *
 * @brief   Initializes the USART2 & USART3 peripheral.
 *
 * @return  none
 */
void
USARTx_CFG (void)
{
  GPIO_InitTypeDef GPIO_InitStructure = { 0 };
  USART_InitTypeDef USART_InitStructure = { 0 };

  RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1,
                          ENABLE);

  /* USART1 TX-->D.5   RX-->D.6 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init (GPIOD, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init (GPIOD, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl
      = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

  USART_Init (USART1, &USART_InitStructure);
  USART_Cmd (USART1, ENABLE);
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
  NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2);
  Delay_Init ();
  // USART_Printf_Init(115200);

  USARTx_CFG ();
  printf ("SystemClk1:%d\r\n", SystemCoreClock);

  GPIO_Toggle_init ();

  u8 i = 0;
  uint8_t p_us = SystemCoreClock / 8000000;
  uint16_t p_ms = (uint16_t)p_us * 1000;
  uint32_t n = 1000;
  uint32_t i_c = (uint32_t)n * p_ms;
  SysTick->SR &= ~(1 << 0);
  SysTick->CMP = i_c;
  SysTick->CNT = 0;
  SysTick->CTLR |= (1 << 0);
  while (1)
    {

      if (USART_GetFlagStatus (USART1, USART_FLAG_RXNE) != RESET)
        {
          /* waiting for receiving finish */
          val = (USART_ReceiveData (USART1));
          USART_SendData (USART1, val);
          while (USART_GetFlagStatus (USART1, USART_FLAG_TXE) == RESET)
            {
              /* waiting for sending finish */
            }
          if (val >= 0x30 && val <= 0x39)
            {
              i_c = (uint32_t)(val - 0x30 + 1) * p_ms * 100;
              printf (" set ms delay:%d\r\n", (val - 0x30 + 1) * 100);
            }
        }

      if (!((SysTick->SR & (1 << 0)) != (1 << 0)))
        {
          SysTick->CTLR &= ~(1 << 0);
          SysTick->SR &= ~(1 << 0);
          SysTick->CMP = i_c;
          SysTick->CNT = 0;
          SysTick->CTLR |= (1 << 0);
          GPIO_WriteBit (GPIOA, GPIO_Pin_1,
                         (i == 0) ? (i = Bit_SET) : (i = Bit_RESET));
        }
    }
}
