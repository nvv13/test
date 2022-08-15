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



// TCON register: Timer/Counter Control Register (Bit-Addressable)
// SFR name Address bit B7  B6  B5  B4  B3  B2  B1  B0
// TCON     88H    name TF1 TR1 TF0 TR0 IE1 IT1 IE0 IT0
//
//descreption bit:
// TF1: Timer/Counter 1 Overflow Flag. Set by hardware on Timer/Counter 1 overflow. The flag can be cleared by
//      software but is automatically cleared by hardware when processor vectors to the Timer 1 interrupt routine.
//      If TF1 = 0, No Timer 1 overflow detected.
//      If TF1 = 1, Timer 1 has overflowed.
// TR1: Timer/Counter 1 Run Control bit. Set/cleared by software to turn Timer/Counter on/off.
//      If TR1 = 0, Timer 1 disabled.
//      If TR1 = 1, Timer 1 enabled.
// TF0: Timer/Counter 0 Overflow Flag. Set by hardware on Timer/Counter 0 overflow. The flag can be cleared by
//      software but is automatically cleared by hardware when processor vectors to the Timer 0 interrupt routine.
//      If TF0 = 0, No Timer 0 overflow detected.
//      If TF0 = 1, Timer 0 has overflowed.
// TR0: Timer/Counter 0 Run Control bit. Set/cleared by software to turn Timer/Counter on/off.
//      If TR0 = 0, Timer 0 disabled.
//      If TR0 = 1, Timer 0 enabled.
// IE1: External Interrupt 1 request flag. Set by hardware when external interrupt rising or falling edge defined by
//      IT1 is detected. The flag can be cleared by software but is automatically cleared when the external interrupt
//      1 service routine has been processed.
// IT1 : External Intenupt 1 Type Select bit. Set/cleared by software to specify rising / falling edges triggered external
//      interrupt 1.
//      If IT1 = 0, INT1 is both rising and falling edges triggered.
//      If IT1 = 1, INT1 is only falling edge triggered.
// IE0 : External Interrupt 0 request flag. Set by hardware when external interrupt rising or falling edge defined by
//      IT0 is detected. The flag can be cleared by software but is automatically cleared when the external interrupt
//      1 service routine has been processed.
// IT0 : External Intenupt 0 Type Select bit. Set/cleared by software to specify rising / falling edges triggered external
//      interrupt 0.
//      If IT0 = 0, INT0 is both rising and falling edges triggered.
//      If IT0 = 1, INT0 is only falling edge triggered

    IT0 = 0; //Both rising and falling edge of INT0 can wake up MCU



// IE: Interrupt Enable Rsgister (Bit-addressable)
// SFR name Address bit B7 B6   B5   B4 B3  B2  B1  B0
//    IE    A8H    name EA ELVD EADC ES ET1 EX1 ET0 EX0
//
//        Enable Bit = 1 enables the interrupt .
//        Enable Bit = 0 disables it .
//descreption bit:
// EA (IE.7): disables all interrupts.
//             If EA = 0, no interrupt would be acknowledged.
//             If EA = 1, each interrupt source would be individually enabled or disabled by setting or clearing
//                its enable bit.
// ELVD (IE.6): Low volatge detection interrupt enable bit.
//             If ELVD = 0, Low voltage detection interrupt would be diabled.
//             If ELVD = 1, Low voltage detection interrupt would be enabled.
// EADC (IE.5): ADC interrupt enable bit.
//             If EADC = 0, ADC interrupt would be diabled.
//             If EADC = 1, ADC interrupt would be enabled.
// ES (IE.4): Serial Port 1 (UART1) interrupt enable bit.
//             If ES = 0, UART1 interrupt would be diabled.
//             If ES = 1, UART1 interrupt would be enabled.
// ET1 (IE.3): Timer 1 interrupt enable bit.
//            If ET1 = 0, Timer 1 interrupt would be diabled.
//            If ET1 = 1, Timer 1 interrupt would be enabled.
// EX1 (IE.2): External interrupt 1 enable bit.
//            If EX1 = 0, external interrupt 1 would be diabled.
//            If EX1 = 1, external interrupt 1 would be enabled.
// ET0 (IE.1): Timer 0 interrupt enable bit.
//            If ET0 = 0, Timer 0 interrupt would be diabled.
//            If ET0 = 1, Timer 0 interrupt would be enabled.
// EX0 (IE.0): External interrupt 0 enable bit.
//            If EX0 = 0, external interrupt 0 would be diabled.
//            If EX0 = 1, external interrupt 0 would be enabled.


    EX0 = 1; //external interrupt 0 would be enabled.
    EA = 1;  //global - interrupt enabled
}


//-----------------------------------------------
unsigned char FLAG; //1:generate a interrupt on rising edge
           //0:generate a interrupt on falling edge
//---------------------------------------------
//Interrupt service routine
INTERRUPT(exint0, IE0_VECTOR)   //void exint0(void) __interrupt(IE0_VECTOR) 
{
P1_2 =!P1_2; //out reverse pin 2 port 1
P1_3 =!P1_3; //
P1_4 =!P1_4; //

FLAG = INT0; //save the sate of INT0, INT0=0(falling); INT0=1(rising)
}
//-----------------------------------------------

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

        NOP();          // Fisrt implement this statement and then enter interrupt service routine
                        // after be waked up from Stop/Power-Down mode 
        NOP();

    }
}
/* ------------------------------------------------------------------------- */
