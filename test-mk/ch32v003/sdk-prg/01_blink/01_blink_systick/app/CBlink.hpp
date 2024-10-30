
#pragma once

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
