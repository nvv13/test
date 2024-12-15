/********************************** (C) COPYRIGHT *******************************
 * File Name          : CH57x_clk.h
 * Author             : WCH
 * Version            : V1.2
 * Date               : 2021/11/17
 * Description
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#ifndef __CH58x_CLK_H__
#define __CH58x_CLK_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  System frequency definition
 */
typedef enum
{
    CLK_SOURCE_LSI = 0x00,
    CLK_SOURCE_LSE,

    CLK_SOURCE_HSE_16MHz = 0x22,
    CLK_SOURCE_HSE_8MHz = 0x24,
    CLK_SOURCE_HSE_6_4MHz = 0x25,
    CLK_SOURCE_HSE_4MHz = 0x28,
    CLK_SOURCE_HSE_2MHz = (0x20 | 16),
    CLK_SOURCE_HSE_1MHz = (0x20 | 0),

    CLK_SOURCE_PLL_80MHz = 0x46,
    CLK_SOURCE_PLL_60MHz = 0x48,
    CLK_SOURCE_PLL_48MHz = (0x40 | 10),
    CLK_SOURCE_PLL_40MHz = (0x40 | 12),
    CLK_SOURCE_PLL_36_9MHz = (0x40 | 13),
    CLK_SOURCE_PLL_32MHz = (0x40 | 15),
    CLK_SOURCE_PLL_30MHz = (0x40 | 16),
    CLK_SOURCE_PLL_24MHz = (0x40 | 20),
    CLK_SOURCE_PLL_20MHz = (0x40 | 24),
    CLK_SOURCE_PLL_15MHz = (0x40 | 0),
} SYS_CLKTypeDef;

/**
 * @brief  32K clock selection
 */
typedef enum
{
    Clk32K_LSI = 0,
    Clk32K_LSE,

} LClk32KTypeDef;

/**
 * @brief  32M crystal oscillator current range
 */
typedef enum
{
    HSE_RCur_75 = 0,
    HSE_RCur_100,
    HSE_RCur_125,
    HSE_RCur_150

} HSECurrentTypeDef;

/**
 * @brief  32M crystal oscillator internal capacitance gear
 */
typedef enum
{
    HSECap_10p = 0,
    HSECap_12p,
    HSECap_14p,
    HSECap_16p,
    HSECap_18p,
    HSECap_20p,
    HSECap_22p,
    HSECap_24p

} HSECapTypeDef;

/**
 * @brief  32K crystal current range
 */
typedef enum
{
    LSE_RCur_70 = 0,
    LSE_RCur_100,
    LSE_RCur_140,
    LSE_RCur_200

} LSECurrentTypeDef;

/**
 * @brief  32K crystal internal capacitance position
 */
typedef enum
{
    LSECap_2p = 0,  //Actually 12p
    LSECap_12p = 0,
    LSECap_13p,
    LSECap_14p,
    LSECap_15p,
    LSECap_16p,
    LSECap_17p,
    LSECap_18p,
    LSECap_19p,
    LSECap_20p,
    LSECap_21p,
    LSECap_22p,
    LSECap_23p,
    LSECap_24p,
    LSECap_25p,
    LSECap_26p,
    LSECap_27p

} LSECapTypeDef;

#define MAX_DAY                   0x00004000
#define MAX_2_SEC                 0x0000A8C0
//#define	 MAX_SEC		0x545FFFFF

#define BEGYEAR                   2020
#define IsLeapYear(yr)            (!((yr) % 400) || (((yr) % 100) && !((yr) % 4)))
#define YearLength(yr)            (IsLeapYear(yr) ? 366 : 365)
#define monthLength(lpyr, mon)    (((mon) == 1) ? (28 + (lpyr)) : (((mon) > 6) ? (((mon) & 1) ? 31 : 30) : (((mon) & 1) ? 30 : 31)))

/**
 * @brief  rtc timer mode period define
 */
typedef enum
{
    Period_0_125_S = 0, // 0.125s cycle
    Period_0_25_S,      // 0.25s cycle
    Period_0_5_S,       // 0.5s cycle
    Period_1_S,         // 1s cycle
    Period_2_S,         // 2s cycle
    Period_4_S,         // 4s cycle
    Period_8_S,         // 8s cycle
    Period_16_S,        // 16s cycle
} RTC_TMRCycTypeDef;

/**
 * @brief  rtc interrupt event define
 */
typedef enum
{
    RTC_TRIG_EVENT = 0, // RTC Triggering Events
    RTC_TMR_EVENT,      // RTC cycle Timed events

} RTC_EVENTTypeDef;

/**
 * @brief  rtc interrupt event define
 */
typedef enum
{
    RTC_TRIG_MODE = 0, // RTC Trigger Mode
    RTC_TMR_MODE,      // RTC cycle Timing Mode

} RTC_MODETypeDef;

typedef enum
{
    /* The higher the calibration accuracy, the longer it takes */
    Level_32 = 3, // Time 1.2ms 1000ppm (32M Frequency)  1100ppm (60M Frequency)
    Level_64,     // Time 2.2ms 800ppm  (32M Frequency)  1000ppm (60M Frequency)
    Level_128,    // Time 4.2ms 600ppm  (32M Frequency)  800ppm  (60M Frequency)

} Cali_LevelTypeDef;

/**
 * @brief   32K Low frequency clock source
 *
 * @param   hc  - Choose whether to use internal or external 32K
 */
void LClk32K_Select(LClk32KTypeDef hc);

/**
 * @brief   HSE Crystal Bias Current Configuration
 *
 * @param   c   - 75%,100%,125%,150%
 */
void HSECFG_Current(HSECurrentTypeDef c);

/**
 * @brief   HSE Crystal Load Capacitor Configuration
 *
 * @param   c   - refer to HSECapTypeDef
 */
void HSECFG_Capacitance(HSECapTypeDef c);

/**
 * @brief   LSE Crystal Bias Current Configuration
 *
 * @param   c   - 70%,100%,140%,200%
 */
void LSECFG_Current(LSECurrentTypeDef c);

/**
 * @brief   LSE Crystal Load Capacitor Configuration
 *
 * @param   c   - refer to LSECapTypeDef
 */
void LSECFG_Capacitance(LSECapTypeDef c);

void Calibration_LSI(Cali_LevelTypeDef cali_Lv); /* Use Frequency to calibrate the internal 32K clock */

/**
 * @brief   RTC Initialize the clock to the current time
 *
 * @param   y       - Configuration Year£¬MAX_Y = BEGYEAR + 44
 * @param   mon     - Configuration month£¬MAX_MON = 12
 * @param   d       - Configuration day£¬MAX_D = 31
 * @param   h       - Configuration Hour£¬MAX_H = 23
 * @param   m       - Configuration Minute£¬MAX_M = 59
 * @param   s       - Configuration Second£¬MAX_S = 59
 */
void RTC_InitTime(uint16_t y, uint16_t mon, uint16_t d, uint16_t h, uint16_t m, uint16_t s);

/**
 * @brief   Get the current time
 *
 * @param   py      - Obtained Year£¬MAX_Y = BEGYEAR + 44
 * @param   pmon    - Obtained Month£¬MAX_MON = 12
 * @param   pd      - Obtained Day£¬MAX_D = 31
 * @param   ph      - Obtained Hour£¬MAX_H = 23
 * @param   pm      - Obtained Minute£¬MAX_M = 59
 * @param   ps      - Obtained Second£¬MAX_S = 59
 */
void RTC_GetTime(uint16_t *py, uint16_t *pmon, uint16_t *pd, uint16_t *ph, uint16_t *pm, uint16_t *ps);

/**
 * @brief   Based on LSE/LSI clock, configure the current RTC cycle number
 *
 * @param   cyc     - Configuration cycle Initial count value £¬MAX_CYC = 0xA8BFFFFF = 2831155199
 */
void RTC_SetCycle32k(uint32_t cyc);

/**
 * @brief   Based on the LSE/LSI clock, get the current RTC cycle number
 *
 * @return  Current cycle number£¬MAX_CYC = 0xA8BFFFFF = 2831155199
 */
uint32_t RTC_GetCycle32k(void);

/**
 * @brief   RTC timing mode Configuration (note that the timing base is fixed to 32768Hz)
 *
 * @param   t   - refer to RTC_TMRCycTypeDef
 */
void RTC_TRIGFunCfg(uint32_t cyc);

/**
 * @brief   RTC timing mode Configuration (note that the timing base is fixed to 32768Hz)
 *
 * @param   t   - refer to RTC_TMRCycTypeDef
 */
void RTC_TMRFunCfg(RTC_TMRCycTypeDef t);

/**
 * @brief   RTC Mode function off
 *
 * @param   m   - The current mode that needs to be closed
 */
void RTC_ModeFunDisable(RTC_MODETypeDef m);

/**
 * @brief   Get RTC interrupt flag
 *
 * @param   f   - refer to RTC_EVENTTypeDef
 *
 * @return  Interrupt flag status
 */
uint8_t RTC_GetITFlag(RTC_EVENTTypeDef f);

/**
 * @brief   Clear the RTC interrupt flag
 *
 * @param   f   - refer to RTC_EVENTTypeDef
 */
void RTC_ClearITFlag(RTC_EVENTTypeDef f);

#ifdef __cplusplus
}
#endif

#endif // __CH58x_CLK_H__
