
#pragma once

#include "port.h"

class CBlink
{
private:
  GPIO_TypeDef *port;
  GPIO_InitTypeDef GPIO_InitStructure = { 0 };
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



      case PB_00:
        {
          GPIO_Pin = GPIO_Pin_0;
          port = GPIOB;
        };
        break;
      case PB_01:
        {
          GPIO_Pin = GPIO_Pin_1;
          port = GPIOB;
        };
        break;
      case PB_02:
        {
          GPIO_Pin = GPIO_Pin_2;
          port = GPIOB;
        };
        break;
      case PB_03:
        {
          GPIO_Pin = GPIO_Pin_3;
          port = GPIOB;
        };
        break;
      case PB_04:
        {
          GPIO_Pin = GPIO_Pin_4;
          port = GPIOB;
        };
        break;
      case PB_05:
        {
          GPIO_Pin = GPIO_Pin_5;
          port = GPIOB;
        };
        break;
      case PB_06:
        {
          GPIO_Pin = GPIO_Pin_6;
          port = GPIOB;
        };
        break;
      case PB_07:
        {
          GPIO_Pin = GPIO_Pin_7;
          port = GPIOB;
        };
        break;


      case PB_08:
        {
          GPIO_Pin = GPIO_Pin_8;
          port = GPIOB;
        };
        break;
      case PB_09:
        {
          GPIO_Pin = GPIO_Pin_9;
          port = GPIOB;
        };
        break;
      case PB_10:
        {
          GPIO_Pin = GPIO_Pin_10;
          port = GPIOB;
        };
        break;
      case PB_11:
        {
          GPIO_Pin = GPIO_Pin_11;
          port = GPIOB;
        };
        break;
      case PB_12:
        {
          GPIO_Pin = GPIO_Pin_12;
          port = GPIOB;
        };
        break;
      case PB_13:
        {
          GPIO_Pin = GPIO_Pin_13;
          port = GPIOB;
        };
        break;
      case PB_14:
        {
          GPIO_Pin = GPIO_Pin_14;
          port = GPIOB;
        };
        break;
      case PB_15:
        {
          GPIO_Pin = GPIO_Pin_15;
          port = GPIOB;
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
    if (port == GPIOB)
      RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB, ENABLE);
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
