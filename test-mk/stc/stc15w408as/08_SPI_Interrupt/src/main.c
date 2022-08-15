/*--------------------------------------------------------------------------------------------------------------*/
/* --- STC MCU International Limited -----------------------------------------------------------------*/
/* --- STC 1T Series MCU SPI Demo (Each other as the master-slave) ---------------------------*/
/* If you want to use the program or the program referenced in the ------------------------------*/
/* article, please specify in which data and procedures from STC -------------------------------*/
/*---- In Keil C development environment, select the Intel 8052 to compiling -------------------*/
/*---- And only contain < reg51.h > as header file ---------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/
//suppose the frequency of test chip is 18.432MHz

	
#include "stc15.h"
#include <compiler.h>
//#include <stdint.h>
//#include <stdio.h>



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


// Macro, constant definition
//#define FALSE 0
//#define TRUE  1
#define LOBYTE(w) ((BYTE)(WORD)(w))
#define HIBYTE(w) ((BYTE)((WORD)(w) >> 8))

//#define MINBAUD 2400L
#define MAXBAUD 115200L

#define MASTER                      //define:master undefine:slave
#define BR16(n) (65536 - FOSC/4/(n))// 16 bit timer,Calculation formula of serial port baud rate of main control chip
#define BR8(n) (256 - FOSC/32/(n))  // 8 bit timer, Calculation formula of serial port baud rate of main control chip
//#define T1MS (65536 - FOSC/1000)    // The initial value of the 1ms timing of the main control chip


typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef unsigned long DWORD;

#define URMD 0 // 0: с помощью таймера 2 в качестве генератора скорости передачи 
               // 1: использовать режим 0 таймера 1 (16-разрядный режим автоматической перезагрузки) в качестве генератора скорости передачи 
               // 2: использовать режим 2 таймера 1 (8-разрядный режим автоматической перезагрузки) в качестве генератора скорости передачи 


/*
sfr T2H   = 0xd6;               // старшие 8 битов таймера 2 
sfr T2L   = 0xd7;               // младшие 8 битов таймера 2 

sfr P0M1 = 0x93;
sfr P0M0 = 0x94;
sfr P1M1 = 0x91;
sfr P1M0 = 0x92;
sfr P2M1 = 0x95;
sfr P2M0 = 0x96;
sfr P3M1 = 0xb1;
sfr P3M0 = 0xb2;
sfr P4M1 = 0xb3;
sfr P4M0 = 0xb4;
sfr P5M1 = 0xC9;
sfr P5M0 = 0xCA;
sfr P6M1 = 0xCB;
sfr P6M0 = 0xCC;
sfr P7M1 = 0xE1;
sfr P7M0 = 0xE2;

sfr  AUXR       =   0x8e;       // вспомогательный регистр                         
sfr SPSTAT      =   0xcd;       //SPI , регистр состояния                            
*/
#define SPIF        0x80        //SPSTAT.7                                
#define WCOL        0x40        //SPSTAT.6                                
/*
sfr SPCTL       =   0xce;       //SPI  регистр управления SPI                           
*/
#define SSIG        0x80        //SPCTL.7                                 
#define SPEN        0x40        //SPCTL.6                                 
#define DORD        0x20        //SPCTL.5                                 
#define MSTR        0x10        //SPCTL.4                                 
#define CPOL        0x08        //SPCTL.3                                 
#define CPHA        0x04        //SPCTL.2                                 
#define SPDHH       0x00        //CPU_CLK/4                               
#define SPDH        0x01        //CPU_CLK/16                              
#define SPDL        0x02        //CPU_CLK/64                              
#define SPDLL       0x03        //CPU_CLK/128                             
/*
sfr SPDAT       =   0xcf;       //SPI Регистр данных SPI                            
*/
                               // Когда SPI является ведущим мульти-ведомым В этом режиме используйте обычный порт ввода-вывода хоста для подключения к SS-порту 
#define SPISS       P1_1       //SPI  Порт выбора ведомого SPI, подключиться к порту SS другого MCU 
                               
/*
sfr IE2         =   0xAF;       //interrupt enable rgister 2
*/
#define ESPI        0x02        //IE2.1

void InitUart();
void InitSPI();
void SendUart(BYTE dat);        //send data to PC
BYTE RecvUart();                //receive data from PC

BYTE MSSEL;                      //1: master 0:slave

///////////////////////////////////////////////////////////

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

    InitUart(); //initial UART
    InitSPI();  //initial SPI

    IE2 |= ESPI;
    EA = 1;

    while (1)
    {
        if (RI)
        {
            SPCTL = SPEN | MSTR; //set as master
            MSSEL = 1;
            ACC = RecvUart();
            SPISS = 0;           //pull low slave SS
            SPDAT = ACC;         //trigger SPI send
        }
    }
}

///////////////////////////////////////////////////////////

/* Interrupt numbers: address = (number * 8) + 3 */
//        9*8+3=75 = 4b
//#define SPI_VECTOR      9       /* 0x4b SPI */
//INTERRUPT(spi_isr, SPI_VECTOR)   
//void spi_isr() interrupt 9 using 1     //SPI interrupt routine 9 (004BH)
void spi_isr(void) __interrupt (9) __using (1)
{
    SPSTAT = SPIF | WCOL;       //clear SPI status
    if (MSSEL)
    {
        SPCTL = SPEN;           //reset as slave
        MSSEL = 0;
        SPISS = 1;              //push high slave SS
        SendUart(SPDAT);        //return received SPI data
    }
    else
    {                           //for salve (receive SPI data from master and
        SPDAT = SPDAT;          // send previous SPI data to master)
    }
}

///////////////////////////////////////////////////////////

void InitUart()
{
    SCON = 0x5a;                //Установить последовательный порт на 8-битную переменную скорость передачи данных 
#if URMD == 0
    T2H = HIBYTE(BR16(MAXBAUD));  //Установить значение перезагрузки скорости передачи данных 
    T2L = LOBYTE(BR16(MAXBAUD));  //115200 bps(65536-11059200/4/115200)
    AUXR = 0x14;                //T2 - режим 1T, и таймер запуска 2 
    AUXR |= 0x01;               //Выбор таймера 2 в качестве генератора скорости передачи последовательного порта 1 
#elif URMD == 1
    AUXR = 0x40;                //Таймер 1 - это режим 1T 
    TMOD = 0x00;                //Таймер 1 - это режим 0 (16-разрядная автоматическая перезагрузка) 
    TL1 = LOBYTE(BR16(MAXBAUD));//Установка значения перезагрузки скорости передачи 
    TH1 = HIBYTE(BR16(MAXBAUD));//115200 bps(65536-11059200/4/115200)
    TR1 = 1;                    //таймер 1 начал 
#else
    TMOD = 0x20;                //установка таймера 1 до 8 режим автоперезагрузкой
    AUXR = 0x40;                //таймер 1 режим 1T
    TH1 = TL1 = BR8(MAXBAUD);   //115200 bps(256 - 11059200/32/115200)
    TR1 = 1;
#endif
}


///////////////////////////////////////////////////////////

void InitSPI()
{
    SPDAT = 0;                  //initial SPI data
    SPSTAT = SPIF | WCOL;       //clear SPI status
    SPCTL = SPEN;               //slave mode
}

///////////////////////////////////////////////////////////

void SendUart(BYTE dat)
{
    while (!TI);                //wait pre-data sent
    TI = 0;                     //clear TI flag
    SBUF = dat;                 //send current data
}

///////////////////////////////////////////////////////////

BYTE RecvUart()
{
    while (!RI);                //wait receive complete
    RI = 0;                     //clear RI flag
    return SBUF;                //return receive data
}

