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

// светодиоды присоеденяем (через резистор 470 ом)
// к 2,3,4 битам 1 порта, соответственно
//   1,2,3 ножкам

#include "stc15.h"
#include <compiler.h>
                     


void setup(void)
{
/*
Таблица с настройкой режима работы порта, каждого пина - порта
для Рорта 1 , с 7 по 0 - биты
настраевается через два регистра специальных функций
P1M1 и P1M0

 I/O ports Mode
P1M1[7 : 0] P1M0 [7 : 0]

0 0 quasi_bidirectional(traditional 8051 I/O port output) ,
    Sink Current up to 20mA , pull-up Current is 270μA ,
    Because of manufactured error, the actual pull-up current is 270uA ~ 150uA
    это и вход (можно читать что на входе), 
    и выход, открытый коллетор с резистором на питание (ток примерно 270мКа), 
    если вниз на 0 -то транзистор тянет до 20ma 

0 1 push-pull output(strong pull-up output,current can be up to 20mA, resistors need to be added to restrict current
    выход, тяни-толкай режим (до 20ma ток)

1 0 input-only (high-impedance )        
    как вход, 

1 1 Open Drain,internal pull-up resistors should be disabled and external pull-up resistors need to join.
    открытый коллектор, транзистор тянет до 20ma на 0, если что, нуждаеться во внешнем резисторе на питание

*/
    // set P1 pins 2 to push-pull output
    // настраеваем
    P1M1 &= ~(1<<2) ;//сбрасываем в 0, 2 бит
    P1M0 |= (1<<2)  ;//устанавливаем в 1, 2 бит

    // set P1 pins 3 to push-pull output
    P1M1 &= ~(1<<3) ;
    P1M0 |= (1<<3) ;

    // set P1 pins 4 to push-pull output
    P1M1 &= ~(1<<4) ;
    P1M0 |= (1<<4) ;

    //вывод в порт
    P1_2 =0; 
         //при записи в память, по адресу P1_2, происходит запись во 2 бит порта 1, это так называемое,
         // проецирование(отображение) памяти на бит порта, применяеться в некоторых МК
    P1_3 =1; //
    P1_4 =0; //


// Power-down wake-up special Timer is added to parts of STC15 series MCU. Besides external interrupts, powerdown
// wake-up timer also can wake up MCU from Stop/PD mode after MCU go into Stop/Power-Down (PD)
// mode.
// The power consumption of "power-down wake-up special Timer : 3uA (for 3V chip) and 5uA (for 5V chip)".
// Power-down wake-up special Timer is controlled and managed by registers WKTCH and WKTCL
//   WKTCL : Power-Down Wake-up Timer Control register low (Non bit-addressable)
//   WKTCH : Power-Down Wake-up Timer Control register high (Non bit-addressable)

// Internal power-down wake-up special Timer consists of a 15-bit timer {WKTCH[6:0],WKTCL[7:0]}. The
// maximum count value of the 15-bit timer {WKTCH[6:0],WKTCL[7:0]} is 32768, while the minimum is 0.

// WKTCL : 7-0 - биты счетчика (обратный отсчёт)
// WKTCH : 6-0 - биты 14-8 счетчика (обратный отсчёт)
//         7 бит в данном регистре, WKTEN - включить 
//         WKTEN：The enable bit of internal power-down wake-up special Timer
//               WKTEN=1，enable internal power-down wake-up special Timer；
//               WKTEN=0，disable internal power-down wake-up special Timer.

    WKTCL = 49;   // wake-up cycle: 488us*(49+1) = 24.4ms, suppose the frequency of chip is 18.432MHz
    WKTCH = 0x80; // WKTEN=1

}


/*********************************************/
int main()
{
    setup(); 

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

        
                        //  выключаем МК, сможет проснуться только от внешних прерываний или wake-up Timer смотри WKTCL, WKTCH
        PCON = 0x02;    //Enter Stop/Power-Down Mode
        NOP();          // 
        NOP();
        P1_2 =!P1_2; //out reverse pin 2 port 1
        P1_3 =!P1_3; //
        P1_4 =!P1_4; //

    }
}
/* ------------------------------------------------------------------------- */
