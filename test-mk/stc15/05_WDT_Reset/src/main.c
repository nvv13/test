/*------------------------------------------------ ---------------------*/
/* --- STC MCU Limited ------------------------------------------ -- ------*/
/* --- Пример теста сброса переполнения сторожевого устройства серии STC15F4K60S4--------------------------*/
/* --- Мобильный: (86)13922805190 --------------------------------------- - -*/
/* --- Факс: 86-0513-55012956,55012947,55012969 ----------*/
/* --- Тел: 86-0513-55012928,55012929,55012966-----------*/
/* --- Интернет: www.STCMCU.com -------------------------------------- ------*/
/* --- Интернет: www.GXWMCU.com -------------------------------------- ------*/
/* Если вы хотите использовать этот код в программе, укажите в программе данные и программу с использованием STC */
/* Если вы хотите применить этот код в статье, пожалуйста, укажите в статье использование данных и процедур STC */
/*------------------------------------------------ ---------------------*/

//Пожалуйста, выберите модель чипа Intel 8058, чтобы скомпилировать этот пример в среде разработки Keil
//Если не указано иное, рабочая частота обычно составляет 11,0592 МГц.




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
// к 2 бит 3 порта, соответственно
//   13 ножка(20 выводов) или 17 ножка(28 выводов)




//Частота задается в makefile для проги прошивающий флэш
// вот эта строка :
//  SYSCLK ?= 35000
// в результате, внутренний генератор МК будет настроен на частоту 35 Mhz

//далее, Укажем, на какой частоте работает МК  :  
//#define FOSC    11059200UL //11.0592 MHz
//#define FOSC    18432000UL //18.432  MHz
//#define FOSC    22118400UL //22.1184 MHz
//#define FOSC    24000000UL //24 Mhz
//#define FOSC    33177000UL //33.177 Mhz 
#define FOSC    35000000UL //35 Mhz


#include "stc15.h"



/*

WDT_CONTR: Watch-Dog-Timer Control Register
*-----------------------------------------------------------------------*
|SFR name |Address| bit |   B7   |B6|  B5  |   B4  |  B3    |B2 |B1 |B0 |
|WDT_CONTR|0C1H   |name |WDT_FLAG|- |EN_WDT|CLR_WDT|IDLE_WDT|PS2|PS1|PS0|
*-----------------------------------------------------------------------*

WDT_FLAG : WDT reset flag.
       0 : This bit should be cleared by software.
       1 : When WDT overflows, this bit is set by hardware to indicate a WDT reset happened.

EN_WDT   : Enable WDT bit. When set, WDT is started.

CLR_WDT  : WDT clear bit. When set, WDT will recount. Hardware will automatically clear this bit.

IDLE_WDT : WDT IDLE mode bit. When set, WDT is enabled in IDLE mode. When clear, WDT is disabled in IDLE.

PS2, PS1, PS0: WDT Pre-scale value set bit.

*/


void  delay_ms(unsigned int ms)
{
     unsigned int i;
     do{
          i = FOSC / 17000;
          while(--i)    ;
     }while(--ms);
}
/* ------------------------------------------------------------------------- */

void main()
{
    P0M0 = 0x00;
    P0M1 = 0x00;
    P1M0 = 0x00;
    P1M1 = 0x00;
    P2M0 = 0x00;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P4M0 = 0x00;
    P4M1 = 0x00;
    P5M0 = 0x00;
    P5M1 = 0x00;
    P6M0 = 0x00;
    P6M1 = 0x00;
    P7M0 = 0x00;
    P7M1 = 0x00;

    P3_2 = 0;
    delay_ms(10000); // ;      //задержка мигания
    P3_2 = 1;
    WDT_CONTR = 0x04;  //Формула расчета времени переполнения сторожевого таймера: (12 * 32768 * PS) / FOSC (секунды)
                       //Установите деление частоты сторожевого таймера на 32, а время переполнения будет следующим:
                       //11.0592M : 1,14 с
                       //18.432M : 0,68 с
                       //20M : 0,63 с
    
   /* в формуле WDT owerflow time, (12 * 32768 * PS) / FOSC = (секунды)
      где PS - это Pre-Scale (делитель)
      ->  Pre-Scale
     0x00 2
     0x01 4
     0x02 8
     0x03 16
     0x04 32       -> WDT_CONTR = 0x04;
     0x05 64
     0x06 128
     0x07 256
    */

    WDT_CONTR |= 0x20; //Запускаем сторожевой таймер B5=EN_WDT


    while (1)
     {
     // WDT_CONTR &= ~0x80; //это, если надо сбросить проверку B7=WDT reset flag. reset -> 0 
                            // , чтоб не срабатывал, время от времени, делать в программе
     };
}

