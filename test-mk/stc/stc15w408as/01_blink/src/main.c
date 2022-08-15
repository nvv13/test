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
#include <stdint.h>
#include <stdio.h>



//========================================================================
void  delay_ms(unsigned int ms)
{
     unsigned int i;
     do{
          i = FOSC / 17000;
          while(--i)    ;
     }while(--ms);
}
/* ------------------------------------------------------------------------- */

#define START_DELAY 50 

int i_my_deley;


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

*все порты, по умолчанию, в тежиме 0 0 quasi_bidirectional, однако, P1.7/XLAT1 и P1.6/XLAT2 могут использовать внешний кристал
 МК при старте проверяет, также при старте, проверяется режим "прошивки" кристала...

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

    i_my_deley=START_DELAY;
}


/*********************************************/
int main()
{
    setup();


    // LOOP  цикл
    while(1)
    {

      P1_2 = 0; //out 0 pin 2 port 1
         //при записи в память, по адресу P1_2, происходит запись во 2 бит порта 1, это так называемое,
         // проецирование(отображение) памяти на бит порта, применяеться в некоторых МК
      delay_ms(i_my_deley); // 
      P1_2 = 1; //out 1 pin 2 port 1

      // P1_2 - если поискать в заголовочных файлах, это:  
      //	__sbit __at (0x92) P1_2 ;
      //  т.е. записывает в ячейку 0x92 специального адр. пространства __sbit
      // можно еще встретьть такую запись:
      //  P1^2 = 1;
      // что эквивалентно 
      //  P1_2 = 1;
      //   если поискать в заголовочных файлах, то
      // 	sfr P1          =   0x90;         // (sfr - спец. функция регистр)
      // 	sbit P12        =   P1^2;
      //  т.е. записывает в ячейку  0x90^2=0x92 специального адр. пространства __sbit



      P1_3 = 0; //out 0 pin 3 port 1
      delay_ms(i_my_deley);
      P1_3 = 1; //out 1 pin 3 port 1

      P1_4 = 0; //out 0 pin 4 port 1
      delay_ms(i_my_deley);
      P1_4 = 1; //out 1 pin 4 port 1

      i_my_deley-=1;
      if(i_my_deley<=0)i_my_deley=START_DELAY;

    }
}
/* ------------------------------------------------------------------------- */
