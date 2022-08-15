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

// Pin Configurations of STC15W204S    DIP8/SOP8:
//--------------------------------------------------------
//CMP-/MCLKO/RST/P5.4 1    8 P3.3/INT1
//		  Vcc 2    7 P3.2/INT0
//	    CMP+/P5.5 3    6 P3.1/TxD/T2
//	    	  Gnd 4    5 P3.0/RxD/INT4/T2CLKO
//--------------------------------------------------------


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
    // set P3 pins 0 to push-pull output
    // настраеваем
    P3M1 &= ~(1<<0) ;//сбрасываем в 0, 2 бит
    P3M0 |= (1<<0)  ;//устанавливаем в 1, 2 бит

    // set P3 pins 1 to push-pull output
    P3M1 &= ~(1<<1) ;
    P3M0 |= (1<<1) ;

    // set P3 pins 2 to push-pull output
    P3M1 &= ~(1<<2) ;
    P3M0 |= (1<<2) ;

    i_my_deley=START_DELAY;
}


/*********************************************/
int main()
{
    setup();


    // LOOP  цикл
    while(1)
    {

      P3_0 = 0; //out 0 pin 0 port 3
         //при записи в память, по адресу P3_0, происходит запись в 0 бит порта 3, это так называемое,
         // проецирование(отображение) памяти на бит порта, применяеться в некоторых МК
      delay_ms(i_my_deley); // 
      P3_0 = 1; //out 1 pin 0 port 3

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



      P3_1 = 0; //out 0 pin 1 port 3
      delay_ms(i_my_deley);
      P3_1 = 1; //out 1 pin 1 port 3

      P3_2 = 0; //out 0 pin 2 port 3
      delay_ms(i_my_deley);
      P3_2 = 1; //out 1 pin 2 port 3

      i_my_deley-=1;
      if(i_my_deley<=0)i_my_deley=START_DELAY;

    }
}
/* ------------------------------------------------------------------------- */
