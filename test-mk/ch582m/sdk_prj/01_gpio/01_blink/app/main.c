#define DEBUG

#include "debug.h"


#define LED_PIN GPIO_Pin_4
//#define LED_PIN GPIO_Pin_1

int
main (void)
{
#if(defined(DCDC_ENABLE)) && (DCDC_ENABLE == TRUE)
    PWR_DCDCCfg(ENABLE);
#endif
    //SetSysClock(CLK_SOURCE_PLL_60MHz); // 1.2 MHz without Delay 
    SetSysClock(CLK_SOURCE_PLL_80MHz); // 1.23 MHz without Delay 
#if(defined(HAL_SLEEP)) && (HAL_SLEEP == TRUE)
    GPIOA_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
    GPIOB_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
#endif
#ifdef DEBUG
    GPIOA_SetBits(bTXD1);
    GPIOA_ModeCfg(bTXD1, GPIO_ModeOut_PP_5mA);
    UART1_DefInit();
#endif

  PRINT("Start\r\n");

  Delay_Init();
  GPIOB_ModeCfg(LED_PIN,GPIO_ModeOut_PP_20mA);
  GPIOB_ResetBits(LED_PIN);
  int iCount=0;
  while (1)
    {
        GPIOB_SetBits(LED_PIN);
        Delay_Ms(100);
        GPIOB_ResetBits(LED_PIN);
        Delay_Ms(900);
        PRINT("iCount=%d\r\n",iCount++);
    }
}
