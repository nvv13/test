#define DEBUG

#include "debug.h"




int
main (void)
{
#if(defined(DCDC_ENABLE)) && (DCDC_ENABLE == TRUE)
    PWR_DCDCCfg(ENABLE);
#endif
    SetSysClock(CLK_SOURCE_PLL_60MHz);
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

  SetSysClock (CLK_SOURCE_PLL_60MHz);
  Delay_Init();
  GPIOB_ModeCfg(GPIO_Pin_4,GPIO_ModeOut_PP_20mA);
  GPIOB_ResetBits(GPIO_Pin_4);
  int iCount=0;
  while (1)
    {
        GPIOB_SetBits(GPIO_Pin_4);
        Delay_Ms(200);
        GPIOB_ResetBits(GPIO_Pin_4);
        Delay_Ms(500);
        PRINT("iCount=%d\r\n",iCount++);
    }
}
