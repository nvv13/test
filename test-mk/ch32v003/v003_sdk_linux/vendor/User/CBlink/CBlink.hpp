
#pragma once

#include "port.h"

class CBlink
{
private:
  GPIO_TypeDef *port;
  u16 GPIO_Pin = 0;
  u8 u8_led_state = 0;

public:
  CBlink (pin_t Pin)
  {
    GPIOSpeed_TypeDef GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Pin = 0;
    port = GPIOA;
    switch (Pin)
      {
      case PA_00:
        {
          GPIO_Pin = GPIO_Pin_0;
          port = GPIOA;
        };
        break;
      case PA_01:
        {
          GPIO_Pin = GPIO_Pin_1;
          port = GPIOA;
        };
        break;
      case PA_02:
        {
          GPIO_Pin = GPIO_Pin_2;
          port = GPIOA;
        };
        break;
      case PA_03:
        {
          GPIO_Pin = GPIO_Pin_3;
          port = GPIOA;
        };
        break;
      case PA_04:
        {
          GPIO_Pin = GPIO_Pin_4;
          port = GPIOA;
        };
        break;
      case PA_05:
        {
          GPIO_Pin = GPIO_Pin_5;
          port = GPIOA;
        };
        break;
      case PA_06:
        {
          GPIO_Pin = GPIO_Pin_6;
          port = GPIOA;
        };
        break;
      case PA_07:
        {
          GPIO_Pin = GPIO_Pin_7;
          port = GPIOA;
        };
        break;
      case PC_00:
        {
          GPIO_Pin = GPIO_Pin_0;
          port = GPIOC;
        };
        break;
      case PC_01:
        {
          GPIO_Pin = GPIO_Pin_1;
          port = GPIOC;
        };
        break;
      case PC_02:
        {
          GPIO_Pin = GPIO_Pin_2;
          port = GPIOC;
        };
        break;
      case PC_03:
        {
          GPIO_Pin = GPIO_Pin_3;
          port = GPIOC;
        };
        break;
      case PC_04:
        {
          GPIO_Pin = GPIO_Pin_4;
          port = GPIOC;
        };
        break;
      case PC_05:
        {
          GPIO_Pin = GPIO_Pin_5;
          port = GPIOC;
        };
        break;
      case PC_06:
        {
          GPIO_Pin = GPIO_Pin_6;
          port = GPIOC;
        };
        break;
      case PC_07:
        {
          GPIO_Pin = GPIO_Pin_7;
          port = GPIOC;
        };
        break;
      case PD_00:
        {
          GPIO_Pin = GPIO_Pin_0;
          port = GPIOD;
        };
        break;
      case PD_01:
        {
          GPIO_Pin = GPIO_Pin_1;
          port = GPIOD;
        };
        break;
      case PD_02:
        {
          GPIO_Pin = GPIO_Pin_2;
          port = GPIOD;
        };
        break;
      case PD_03:
        {
          GPIO_Pin = GPIO_Pin_3;
          port = GPIOD;
        };
        break;
      case PD_04:
        {
          GPIO_Pin = GPIO_Pin_4;
          port = GPIOD;
        };
        break;
      case PD_05:
        {
          GPIO_Pin = GPIO_Pin_5;
          port = GPIOD;
        };
        break;
      case PD_06:
        {
          GPIO_Pin = GPIO_Pin_6;
          port = GPIOD;
        };
        break;
      case PD_07:
        {
          GPIO_Pin = GPIO_Pin_7;
          port = GPIOD;
        };
        break;
      }

    u8_led_state = 0;
    if (port == GPIOA)
      RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE);
    if (port == GPIOC)
      RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC, ENABLE);
    if (port == GPIOD)
      RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure = { 0 };
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
    //GPIO_WriteBit (port, GPIO_InitStructure.GPIO_Pin, Bit_SET);
    port->BSHR = GPIO_Pin; /* write high */
  }
  void
  Off (void)
  {
    //GPIO_WriteBit (port, GPIO_InitStructure.GPIO_Pin, Bit_RESET);
    port->BCR = GPIO_Pin; /* write low */
  }
};
