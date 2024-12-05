/**
 * @ingroup     drivers_ws2812b
 * @{
 *
 * @file
 * @brief       WS2812B RGB LED driver implementation
 *
 *
 * @}
 */

#include "ws2812b.h"
#include "debug.h"

#define BLUE (0xff0000)
#define GREEN (0x00ff00)
#define RED (0x0000ff)
#define BLUE_SHIFT (16U)
#define GREEN_SHIFT (8U)

/*
Data transfer time( TH+TL=1.25 μs ±600n s )

0:
T0H 0 code ,high voltage time 0.4us ±150ns
T0L 0 code , low voltage time 0.85us ±150ns

1:
T1H 1 code ,high voltage time 0.85us ±150ns
T1L 1 code ,low voltage time 0.4us ±150ns

RES low voltage time Above 50μs
*/

static GPIO_TypeDef *port;
static u16 GPIO_Pin = 0;

void
ws2812b_init (ws2812b_t *dev, const ws2812b_params_t *params)
{
  *dev = *params;

  GPIOSpeed_TypeDef GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Pin = 0;
  port = GPIOA;
  switch (dev->data_pin)
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
  port->BCR = GPIO_Pin; /* write low */
}

void
ws2812b_load_rgba (const ws2812b_t *dev, const color_rgba_t vals[])
{
  __disable_irq ();
  for (int i = 0; i < dev->led_numof; i++)
    {
      uint32_t data = 0; // HEAD;
      /* we scale the 8-bit alpha value to a 5-bit value by cutting off the
       * 3 leas significant bits */
      data |= (((uint32_t)vals[i].color.b & (uint32_t)vals[i].alpha)
               << BLUE_SHIFT);
      data |= (((uint32_t)vals[i].color.g & (uint32_t)vals[i].alpha)
               << GREEN_SHIFT);
      data |= vals[i].color.r & (uint32_t)vals[i].alpha;

      for (int i = 23; i >= 0; i--)
        {
          port->BSHR = GPIO_Pin; /* write high GPIO_SetBits(port, GPIO_Pin) */
          if (((data >> i) & 0x01))
            { // 1
              volatile int ic = 3;
              while (ic--)
                ;
              //
              // half period 0.85 us
              port->BCR
                  = GPIO_Pin; /* write low GPIO_ResetBits(port, GPIO_Pin)  */
              // half period 0.4 us
              ic = 0;
              ic = 1;
              ic = 0;
              ic = 1;
              ic = 0;
              ic = 1;
            }
          else
            {
              //  half period 0.4 us
              volatile int ic = 1;
              ic = 0;
              ic = 1;
              ic = 0;
              ic = 1;
              ic = 0;
              ic = 1;
              ic = 0;
              ic = 1;
              port->BCR
                  = GPIO_Pin; /* write low GPIO_ResetBits(port, GPIO_Pin)  */
              ic = 2;
              while (ic--)
                ;
              ic = 1;
              ic = 0;
              // half period 0.85 us
            }
        }
    }
  __enable_irq ();

  // RES above 50μs
  Delay_Us (60);
}
