
#include "debug.h"

#include "CUsart.hpp"

#define TxSize1 63

static u8 RxBuffer1[TxSize1 + 1] = { 0 };
static volatile u8 RxCnt1 = 0, RxLineLen = 0;
static volatile bool l_flag_enter = false;

CUsart *CUsart::instance = NULL;
CUsart *
CUsart::GetInstance ()
{
  if (instance == NULL)
    {
      CUsart oCUsart = CUsart ();
      instance = &oCUsart;
    }
  return instance;
}

CUsart::CUsart (void)
{
  GPIO_InitTypeDef GPIO_InitStructure = { 0 };
  USART_InitTypeDef USART_InitStructure = { 0 };
  NVIC_InitTypeDef NVIC_InitStructure = { 0 };

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
  USART_ITConfig (USART1, USART_IT_RXNE, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init (&NVIC_InitStructure);

  USART_Cmd (USART1, ENABLE);
}

// void USART1_IRQHandler(void)
// __attribute__((interrupt("WCH-Interrupt-fast")));
extern "C" void USART1_IRQHandler (void)
    __attribute__ ((interrupt ("machine")));
extern "C" void
USART1_IRQHandler (void)
{
  if (USART_GetITStatus (USART1, USART_IT_RXNE) != RESET)
    {
      u8 bRecive = USART_ReceiveData (USART1);
      RxBuffer1[RxCnt1] = bRecive;
      /*
      Клавиши - стрелки, кода:
      uart0  Left: 1B, 5B, 44,
      uart0    Up: 1B, 5B, 41,
      uart0  Down: 1B, 5B, 42,
      uart0 Right: 1B, 5B, 43,
        backspace: 7F
      */
      if (bRecive == 0x7F)
        {
          if (RxCnt1 != 0)
            RxCnt1--;
        }
      else
        {
          if (bRecive == '\n' || bRecive == '\r')
            {
              if (RxCnt1 != 0)
                {
                  RxBuffer1[RxCnt1] = 0x00;
                  RxLineLen = RxCnt1 - 1;
                  l_flag_enter = true;
                  RxCnt1 = 0;
                }
            }
          else
            {
              RxCnt1++;
              if (RxCnt1 == TxSize1)
                {
                  // USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
                  RxCnt1 = 0;
                }
            }
        }
    }
}

bool
CUsart::is_recive_line (void)
{
  if (l_flag_enter)
    {
      l_flag_enter = false;
      return true;
    }
  return false;
}

u8 *
CUsart::line_cstr (void)
{
  return RxBuffer1;
}

u8
CUsart::line_len (void)
{
  return RxLineLen;
}
