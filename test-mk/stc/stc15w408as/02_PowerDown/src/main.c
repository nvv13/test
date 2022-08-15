/*----------------------------------------------------------------------*/
/* --- STC MCU International Limited -----------------------------------*/
/* --- STC 15 Series  --------------------------------------------------*/
/* --- Web: www.STCMCU.com ------- www.GXWMCU.com ----------------------*/
/*----------------------------------------------------------------------*/


//битовые операции  (напоминалка) 
// & bitwise AND 
// ^ bitwise exclusive OR 
// | bitwise inclusive OR
//пример
// Bit value	Results of
// E1	E2	E1 & E2		E1 ^ E2		E1 | E2
// 0	0	0		0		0
// 1	0	0		1		1
// 0	1	0		1		1
// 1	1	1		0		1
// 
// ~	bitwise complement; inverts each bit.
// >>	bitwise shift right; moves the bits to the right, discards the far right bit and if unsigned assigns 0 to the left most bit, otherwise sign extends.
// <<	bitwise shift left; moves the bits to the left, it discards the far left bit and assigns 0 to the right most bit.


// MK = stc15w408as        DIP20/SOP20:
//   --------------------------------------------------------
// 1 P1.2/ADC2/SS/ECI/CMPO                  CCP0/ADC1/P1.1 20
// 2 P1.3/ADC3/MOSI                         CCP1/ADC0/P1.0 19
// 3 P1.4/ADC4/MISO            CCP2_2/CCP2/TxD_2/INT3/P3.7 18
// 4 P1.5/ADC5/SCLK                 CCP1_2/RxD_2/INT2/P3.6 17
// 5 P1.6/ADC6/RxD_3/XTAL2/MCLKO_2      CCP0_2/T0CLKO/P3.5 16
// 6 P1.7/ADC7/TxD_3/XTAL1                   ECI_2/T0/P3.4 15
// 7 P5.4/RST/MCLKO/CMP-                         INT1/P3.3 14
// 8 Vcc                                         INT0/P3.2 13
// 9 P5.5/CMP+                                 T2/TxD/P3.1 12
//10 Gnd                               T2CLKO/INT4RxD/P3.0 11
//   --------------------------------------------------------

// MK = stc15w408as        DIP28/SOP28:
//   ---------------------------------------------------------
// 1 P2.6/CCP1_3                                P2.5/CCP0_3 28
// 2 P2.7/CCP2_3                            P2.4/ECI_3/SS_2 27
// 3 P1.0/ADC0/CCP1			        P2.3/MOSI_2 26
// 4 P1.1/ADC1/CCP0			        P2.2/MISO_2 25
// 5 P1.2/ADC2/SS/ECI/CMPO 		        P2.1/SCLK_2 24
// 6 P1.3/ADC3/MOSI                         P2.0/RSTOUT_LOW 23
// 7 P1.4/ADC4/MISO             CCP2_2/CCP2/TxD_2/INT3/P3.7 22
// 8 P1.5/ADC5/SCLK                  CCP1_2/RxD_2/INT2/P3.6 21
// 9 P1.6/ADC6/RxD_3/XTAL2/MCLKO_2       CCP0_2/T0CLKO/P3.5 20
// 10 P1.7/ADC7/TxD_3/XTAL1                   ECI_2/T0/P3.4 19
// 11 P5.4/RST/MCLKO/CMP-                         INT1/P3.3 18
// 12 Vcc                                         INT0/P3.2 17
// 13 P5.5/CMP+                                 T2/TxD/P3.1 16
// 14 Gnd                               T2CLKO/INT4RxD/P3.0 15
//   ---------------------------------------------------------

#include "stc15.h"
#include <compiler.h>



/*********************************************/
int main()
{
    while (1)
    {

//PCON register (Power Control Register)
// SFR name Address bit B7   B6    B5   B4  B3  B2  B1 B0
// PCON     87H    name SMOD SMOD0 LVDF POF GF1 GF0 PD IDL
//
//descreption bit:
// SMOD : PCON.7 Double baud rate of UART interface, 1-Удвоенная скорость, приема/передачи, по умолчанью 0
//               0 Keep normal baud rate when the UART is used in mode 1,2 or 3.
//               1 Double baud rate bit when the UART is used in mode 1,2 or 3.
// SMOD0: PCON.6 SM0/FE bit select for SCON.7; setting this bit will set SCON.7 as Frame Error function. Clearing it to
//               set SCON.7 as one bit of UART mode selection bits.
// LVDF : PCON.5 Pin Low-Voltage Flag. Once low voltage condition is detected (VCC power is lower than LVD voltage), it is set by hardware (and should be cleared by software).
// POF  : PCON.4 Power-On flag. It is set by power-off-on action and can only cleared by software.
//               Practical application: if it is wanted to know which reset the MCU is used.
//                 POF=1? Yes, cold boot PowerOnReset, clear POF !
//                 POF=0 - external manual reset or WDT od soft reset or others. 
// GF1  : PCON.3 General-purposed flag 1
// GF0  : PCON.2 General-purposed flag 0  Флаг пользователя
// PD   : PCON.1 Stop Mode/Power-Down Select bit..
//               Setting this bit will place the STC15 series MCU in Stop/Power-Down mode. Stop/Power-Down mode
//               can be waked up by external interrupt. Because the MCU’ s internal oscillator stopped in Stop/Power-
//               Down mode, CPU, Timers, UARTs and so on stop to run, only external interrupt go on to work. The
//               following pins can wake up MCU from Stop/Power-Down mode: INT0/P3.2, INT1/P3.3, INT2/P3.6,
//               INT3/P3.7, INT4/P3.0; pins CCP0/CCP1/CCP2/CCP3/CCP4/CCP5; pins RxD/RxD2/RxD3/RxD4;
//               pins T0/T1/T2/T3/T4; Internal power-down wake-up Timer.     
// IDL  : PCON.0 Бит холостого кода, Idle mode select bit.
//               Setting this bit will place the STC15 series in Idle mode. only CPU goes into Idle mode. (Shuts off
//               clock to CPU, but clock to Timers, Interrupts, Serial Ports, and Analog Peripherals are still active.) External
//               Interrupts, Timer interrupts, low-voltage detection interrupt and ADC interrupt all can wake up
//               MCU from Idle mode.

        PCON |= 0x02;   // PD   : PCON.1 Stop Mode/Power-Down Select bit..
                        //  выключаем МК, сможет проснуться только от внешних прерываний или wake-up Timer смотри WKTCL, WKTCH

        NOP();          // 
        NOP();
        NOP();
    }
}
/* ------------------------------------------------------------------------- */
