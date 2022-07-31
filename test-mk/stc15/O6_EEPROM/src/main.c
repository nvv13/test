/* ------------------------------------------------ --------------------- */ 
/* --- STC MCU Limited ------------------- ----------------------------- */ 
/* --- Пример внутренней EEPROM серии STC15F4K60S4 --------- ----------------------- */ 
/* --- Мобильный: (86) 13922805190 -------------- -------------------------- */ 
/* --- Факс: 86-0513-55012956,55012947,55012969 ------ ------------------ */ 
/* --- Тел: 86-0513-55012928,55012929,55012966 -------------- ----------- */ 
/* --- Веб: www.STCMCU.com ------------------------- ------------------- */ 
/* --- Веб: www.GXWMCU.com ----------------- --------------------------- */ 
/* Если вы хотите использовать этот код в своей программе, укажите в программе, что используется STC. Материалы и процедуры */ 
/* Если вы хотите применить этот код в статье, укажите в статье информацию и процедуры, используя STC */ 
/* ------------------------------------------- -------------------------- */
 
// Этот пример скомпилирован в среде разработки Keil, пожалуйста, выберите модель чипа Intel 8058. 
// Если нет особых пояснений, рабочая частота Обычно 11.0592 МГц 




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

typedef unsigned char BYTE;
typedef unsigned int WORD;

//-----------------------------------------------

/*
sfr P1M1 = 0x91;    //PxM1.n,PxM0.n     =00--->Standard,    01--->push-pull
sfr P1M0 = 0x92;    //                  =10--->pure input,  11--->open drain
sfr P0M1 = 0x93;
sfr P0M0 = 0x94;
sfr P2M1 = 0x95;
sfr P2M0 = 0x96;
sfr P3M1 = 0xB1;
sfr P3M0 = 0xB2;
sfr P4M1 = 0xB3;
sfr P4M0 = 0xB4;
sfr P5M1 = 0xC9;
sfr P5M0 = 0xCA;
sfr P6M1 = 0xCB;
sfr P6M0 = 0xCC;
sfr P7M1 = 0xE1;
sfr P7M0 = 0xE2;

sfr IAP_DATA    =   0xC2;           //регистр данных IAP
sfr IAP_ADDRH   =   0xC3;           //тарший байт регистра адреса IAP 
sfr IAP_ADDRL   =   0xC4;           //младший байт регистра адреса IAP
sfr IAP_CMD     =   0xC5;           //команда IAP Регистр
sfr IAP_TRIG    =   0xC6;           //Регистр триггера команды IAP 
sfr IAP_CONTR   =   0xC7;           //Регистр управления IAP 
*/

#define CMD_IDLE    0               //Режим ожидания 
#define CMD_READ    1               //Команда чтения байта IAP 
#define CMD_PROGRAM 2               //Байт IAP Команда программирования 
#define CMD_ERASE   3               //Команда удаления сектора IAP 

//#define ENABLE_IAP 0x80           //if SYSCLK<30MHz
//#define ENABLE_IAP 0x81           //if SYSCLK<24MHz
#define ENABLE_IAP  0x82            //if SYSCLK<20MHz
//#define ENABLE_IAP 0x83           //if SYSCLK<12MHz
//#define ENABLE_IAP 0x84           //if SYSCLK<6MHz
//#define ENABLE_IAP 0x85           //if SYSCLK<3MHz
//#define ENABLE_IAP 0x86           //if SYSCLK<2MHz
//#define ENABLE_IAP 0x87           //if SYSCLK<1MHz

//тестовый адрес 
#define IAP_ADDRESS 0x0400

void Delay(BYTE n);
void IapIdle();
BYTE IapReadByte(WORD addr);
void IapProgramByte(WORD addr, BYTE dat);
void IapEraseSector(WORD addr);

void main()
{
    WORD i;

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

    P1 = 0xfe;                      //1111,1110 Системная 
    Delay(10);                      //Задержка времени 
    IapEraseSector(IAP_ADDRESS);    //Стирание сектора 
    for (i=0; i<512; i++)           //Определить, если стирание прошло успешно (полное определение FF) 
    {
        if (IapReadByte(IAP_ADDRESS+i) != 0xff)
            goto Error;             //Если есть ошибка, выход 
    }
    P1 = 0xfc;                      //1111,1100 Стереть успешно 
    Delay(10);                      //Задержка 
    for (i=0; i<512; i++)           //Программа 512 байт
    {
        IapProgramByte(IAP_ADDRESS+i, (BYTE)i);
    }
    P1 = 0xf8;                      //1111,1000 Программирование завершается 
    Delay(10);                      //задержка 
    for (i=0; i<512; i++)           //проверяет 512 байт 
    {
        if (IapReadByte(IAP_ADDRESS+i) != (BYTE)i)
            goto Error;             //Если контрольная сумма неверна, выход 
    }
    P1 = 0xf0;                      //1111,0000 Проверка завершена, 
    while (1);
Error:
    P1 &= 0x7f;                     //0xxx, xxxx IAP операция не удалась 
    while (1);
}

/*----------------------------
Задержка программного обеспечения 
----------------------------*/
void Delay(BYTE n)
{
    WORD x;

    while (n--)
    {
        x = 0;
        while (++x);
    }
}

/*----------------------------
Закрыть IAP 
----------------------------*/
void IapIdle()
{
    IAP_CONTR = 0;                  //Закрыть функцию IAP 
    IAP_CMD = 0;                    //Очистить регистр команд 
    IAP_TRIG = 0;                   //Очистить регистр триггера
    IAP_ADDRH = 0x80;               //Установить адрес для области без IAP 
    IAP_ADDRL = 0;
}

/*----------------------------
от ISP / IAP / EEPROM Область читает байт 
----------------------------*/
BYTE IapReadByte(WORD addr)
{
    BYTE dat;                       //Буфер данных Область 

    IAP_CONTR = ENABLE_IAP;         //Включить IAP
    IAP_CMD = CMD_READ;             //Установить команду IAP 
    IAP_ADDRL = addr;               //Установка низкого адреса 
    IAP_ADDRH = addr >> 8;          //Установка высокого адреса 
    IAP_TRIG = 0x5a;                //Команда записи триггера (0x5a) 
    IAP_TRIG = 0xa5;                //Запись команды триггера (0xa5) 
    NOP();                        //Дождитесь завершения операции ISP / IAP / EEPROM 
    dat = IAP_DATA;                 //Считайте данные ISP / IAP / EEPROM 
    IapIdle();                      //Закройте функцию IAP 

    return dat;                     //dat; // Return 
}

/*----------------------------
Запишите один байт данных в область ISP / IAP / EEPROM 
----------------------------*/
void IapProgramByte(WORD addr, BYTE dat)
{
    IAP_CONTR = ENABLE_IAP;         //Включить IAP
    IAP_CMD = CMD_PROGRAM;          //Установить команду IAP
    IAP_ADDRL = addr;               //Установка низкого адреса
    IAP_ADDRH = addr >> 8;          //Установка высокого адреса 
    IAP_DATA = dat;                 //Запись данных ISP / IAP / EEPROM 
    IAP_TRIG = 0x5a;                //Запись команды триггера (0x5a) 
    IAP_TRIG = 0xa5;                //Запись команды триггера (0xa5) 
    NOP();                        //Ожидание завершения 
    IapIdle();
}

/*----------------------------
Удаление сектора 
----------------------------*/
void IapEraseSector(WORD addr)
{
    IAP_CONTR = ENABLE_IAP;         //Включить IAP 
    IAP_CMD = CMD_ERASE;            //Установить команду IAP 
    IAP_ADDRL = addr;               //Установить низкий адрес 
    IAP_ADDRH = addr >> 8;          //Установить высокий адрес 
    IAP_TRIG = 0x5a;                //Команда триггера записи (0x5a)
    IAP_TRIG = 0xa5;                //Запись триггерной команды (0xa5) 
    NOP();                        //Ожидание завершения операции ISP / IAP / EEPROM
    IapIdle();
}

