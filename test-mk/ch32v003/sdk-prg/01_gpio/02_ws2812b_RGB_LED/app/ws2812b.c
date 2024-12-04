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

//#include <string.h>

#include "debug.h"

//#include "assert.h"

#include "ws2812b.h"

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
static GPIO_InitTypeDef GPIO_InitStructure = { 0 };
static u8 GPIO_Pin = 0;

static inline void
shift (uint32_t data)
{
  for (int i = 23; i >= 0; i--)
    {

      if (((data >> i) & 0x01))
        { // 1
          GPIO_WriteBit (port, GPIO_InitStructure.GPIO_Pin,
                         Bit_SET); /* write high */
          Delay_Us (
              170); // freg 571.420  KHz CPU_CLK_240M     half period 0.85 us,
                    // + - значения чуть подравлены с учетом операторов в цикле
          GPIO_WriteBit (port, GPIO_InitStructure.GPIO_Pin,
                         Bit_RESET); /* write low */
          Delay_Us (
              61); // freg 1.250009  MHz CPU_CLK_240M     half period 0.4 us
        }
      else
        { // 0
          GPIO_WriteBit (port, GPIO_InitStructure.GPIO_Pin,
                         Bit_SET); /* write high */
          Delay_Us (
              60); // freg 1.250009  MHz CPU_CLK_240M     half period 0.4 us
          GPIO_WriteBit (port, GPIO_InitStructure.GPIO_Pin,
                         Bit_RESET); /* write low */
          Delay_Us (
              163); // freg 571.420  KHz CPU_CLK_240M     half period 0.85 us
        }
    }
}

void
ws2812b_init (ws2812b_t *dev, const ws2812b_params_t *params)
{
  //assert (dev && params);

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
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed;
  GPIO_Init (port, &GPIO_InitStructure);
  GPIO_WriteBit (port, GPIO_InitStructure.GPIO_Pin, Bit_RESET); /* write low */
}

void
ws2812b_load_rgba (const ws2812b_t *dev, const color_rgba_t vals[])
{
  //assert (dev && vals);

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
      shift (data);
    }

  GPIO_WriteBit (port, GPIO_InitStructure.GPIO_Pin, Bit_RESET); /* write low */

  __enable_irq ();

  // RES above 50μs
  Delay_Us (13000);
}
