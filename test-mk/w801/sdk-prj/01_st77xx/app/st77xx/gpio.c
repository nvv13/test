#include "wm_gpio.h"
#include "wm_gpio_afsel.h"

#include "wm_hostspi.h"

#include "gpio.h"

void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)
{
      tls_gpio_cfg (GPIO_InitStruct->GPIO_Pin, WM_GPIO_DIR_OUTPUT,   WM_GPIO_ATTR_PULLHIGH);

}

void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal)
{
          tls_gpio_write (GPIO_Pin,   BitVal); //(arg_int==0?1:0) );

}

void gpio_PortClockStart(GPIO_TypeDef *GPIOx)
{

}

