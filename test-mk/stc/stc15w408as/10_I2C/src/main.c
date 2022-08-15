
/*------------------------------------------------ ---------------------*/
/* --- STC MCU International Limited ----------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ---------------------------------------- -*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.GXWMCU.com -------------------------------------- ------*/
/* --- QQ: 800003751 -------------------------------------------- -------*/
/* If you want to use this code in the program , please indicate in the program that the data and program of Hongjing Technology are used */
/*------------------------------------------------ ---------------------*/




/**************** 	This program function description 	**************

write RTC, IC is PCF8563.

Use the IO mode of STC 's MCU to control the 74HC595 to drive the 8 -bit digital tube.

The user can modify the macro to select the clock frequency .
Use the 16 -bit automatic reload of Timer0 to generate a 1ms beat , 
  the program runs under this beat, and when the user modifies the MCU main clock frequency , 
  the automatic timing is 1ms.

Users can modify it to common cathode or common anode in the display function . 
It is recommended to use common cathode digital tubes as much as possible .

8 -digit digital tube display time ( hour - minute - second ).

The row and column scan key code is 17~32.

button only supports single-key pressing , 
 and does not support pressing multiple keys at the same time , 
 so there will be unpredictable results .

key is pressed for more than 1 second , 
 the heavy key output will be provided at a speed of 10 keys / second . 
 The user only needs to check whether the KeyCode is not 0 to determine whether the key is pressed .

Adjust time key :
Keycode 17: Hour +.
Keycode 18: Hour- .
Keycode 19: min +.
Keycode 20: Minutes- .


********************************************/



#include "stc15.h"
#include <compiler.h>

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
#define FOSC    35000000UL //35 Mhz  Define the main clock


/**************************************************** ************/

#define DIS_DOT 		0x20
#define DIS_BLACK 	0x10
#define DIS_ 		0x11


/******************************** User Defined Macro **************** ********************/


#define 	Timer0_Reload (65536UL - (FOSC / 1000)) 	//Timer 0 interrupt frequency , 1000 times / second

/**************************************************** ****************************/



typedef unsigned char u8;
typedef unsigned int  u16;
typedef unsigned long u32;



/**************** 	Local constant declaration 	**************/
u8 const t_display[]={ 						// Standard font
// 	0 1 2 3 4 5 6 7 8 9 ABCDEF
	0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,
//black 	- HJKL No PU t GQ r M y
	0x00,0x40,0x76,0x1E,0x70,0x38,0x37,0x5C,0x73,0x3E,0x78,0x3d,0x67,0x50,0x37,0x6e,
	0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,0x46};//0. 1. 2. 3. 4. 5. 6. 7. 8. 9. -1

u8 const T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}; 		// Bit code


/**************** 	IO port definition 	**************/
#define P_HC595_SER    P3_3 //pin SER 		data input
#define	P_HC595_RCLK   P3_5 //pin RCLkstore (latch) clock
#define	P_HC595_SRCLK  P3_6 //pin SRCLKShift data clock


/**************** 	Local variable declaration 	**************/

u8 	LED8[8]; 		// Display buffer
u8 	display_index;// Display bit index
u8 	B_1ms; 			//1ms flag

u8 IO_KeyState, IO_KeyState1, IO_KeyHoldCnt; 	// Row and column keyboard variables
u8 	KeyHoldCnt;// Key press timing
u8 	KeyCode;// Key code for user , 1~16 is valid
u8 	cnt50ms;

u8 	hour,minute,second;//RTC variable
u16     msecond 	;


/************* 	Local function declaration 	**************/
void 	CalculateAdcKey(u16 adc);
void 	IO_KeyScan(void);//50ms call
void 	WriteNbyte(u8 addr, u8 *p, u8 number);
void 	ReadNbyte( u8 addr, u8 *p, u8 number);
void 	DisplayRTC(void);
void 	ReadRTC(void);
void 	WriteRTC(void);


/**************** External function declaration and external variable declaration ****************/


/************************************************/
void main(void)
{
	u8 i;

	P0M1 = 0;P0M0 = 0;// Set the quasi-bidirectional port
	P1M1 = 0; P1M0 = 0;// Set the quasi-bidirectional port
	P2M1 = 0; P2M0 = 0;// Set the quasi-bidirectional port
	P3M1 = 0;P3M0 = 0;// Set the quasi-bidirectional port
	P4M1 = 0; P4M0 = 0;// Set the quasi-bidirectional port
	P5M1 = 0;P5M0 = 0;// Set the quasi-bidirectional port
	P6M1 = 0; P6M0 = 0;// Set the quasi-bidirectional port
	P7M1 = 0;P7M0 = 0;// Set the quasi-bidirectional port
	
	display_index = 0;
	
	AUXR = 0x80;//Timer0 set as 1T, 16 bits timer auto-reload,
	TH0 = (u8)(Timer0_Reload / 256);
	TL0 = (u8)(Timer0_Reload % 256);
	ET0 = 1;//Timer0 interrupt enable
	TR0 = 1; //Tiner0 run
	EA = 1; 		// turn on total interrupt
	
	for(i=0; i<8; i++)LED8[i] = 0x10;// Power-on blanking

	
	ReadRTC();
	F0 = 0;
	if(second >= 60)F0 = 1;// Error
	if(minute >= 60)F0 = 1;// Error
	if(hour >= 24)F0 = 1;// Error
	if(F0)// There is an error , the default is 12:00:00
	{
		second = 0;
		minute = 0;
		hour = 12;
		WriteRTC();
	}

	DisplayRTC();
	LED8[2] = DIS_;
	LED8[5] = DIS_;

	KeyHoldCnt = 0;// Key press timing
	KeyCode = 0;// Key code for users , 1~16 are valid

	IO_KeyState = 0;
	IO_KeyState1 = 0;
	IO_KeyHoldCnt = 0;
	cnt50ms = 0;
	
	while(1)
	{
		if(B_1ms)//1ms to
		{
			B_1ms = 0;
			if(++msecond >= 1000)//1 second to
			{
				msecond = 0;
				ReadRTC();
				DisplayRTC();
			}

			if(++cnt50ms >= 50) 		//50ms scan row and column keyboard
			{
				cnt50ms = 0;
				IO_KeyScan();
			}
			
			if(KeyCode != 0) 		// A key is pressed
			{
				if(KeyCode == 17)//hour +1
				{
					if(++hour >= 24)hour = 0;
					WriteRTC();
					DisplayRTC();
				}
				if(KeyCode == 18)//hour -1
				{
					if(--hour >= 24)hour = 23;
					WriteRTC();
					DisplayRTC();
				}
				if(KeyCode == 19)//minute +1
				{
					second = 0;
					if(++minute >= 60)minute = 0;
					WriteRTC();
					DisplayRTC();
				}
				if(KeyCode == 20)//minute -1
				{
					second = 0;
					if(--minute >= 60)minute = 59;
					WriteRTC();
					DisplayRTC();
				}

				KeyCode = 0;
			}

		}
	}
}
/************************************************/

/************************ Display clock function ************************/
void 	DisplayRTC(void)
{
	if(hour >= 10)LED8[0] = hour / 10;
	else 			LED8[0] = DIS_BLACK;
	LED8[1] = hour % 10;
	LED8[2] = DIS_;
	LED8[3] = minute / 10;
	LED8[4] = minute % 10;
	LED8[6] = second / 10;
	LED8[7] = second % 10;
}

/************************ Read RTC function ************************/
void 	ReadRTC(void)
{
	u8 tmp[3];

	ReadNbyte(2, tmp, 3);
	second = ((tmp[0] >> 4) & 0x07) * 10 + (tmp[0] & 0x0f);
	minute = ((tmp[1] >> 4) & 0x07) * 10 + (tmp[1] & 0x0f);
	hour = ((tmp[2] >> 4) & 0x03) * 10 + (tmp[2] & 0x0f);
}

/************************ Write RTC function ************************/
void 	WriteRTC(void)
{
	u8 tmp[3];

	tmp[0] = ((second / 10) << 4) + (second % 10);
	tmp[1] = ((minute / 10) << 4) + (minute % 10);
	tmp[2] = ((hour / 10) << 4) + (hour % 10);
	WriteNbyte(2, tmp, 3);
}


/**************************************************** ****
	Row and Column Key Scanner
	using XY to find 4x4 keys , only single key , fast

Y P04 P05 P06 P07
| | | |
X | | | |
P00 ---- K00 ---- K01 ---- K02 ---- K03 ----
| | | |
P01 ---- K04 ---- K05 ---- K06 ---- K07 ----
| | | |
P02 ---- K08 ---- K09 ---- K10 ---- K11 ----
| | | |
P03----K12----K13----K14----K15----
| | | |
**************************************************** ****/


u8 const T_KeyTable[16] = {0,1,2,0,3,0,0,0,4,0,0,0,0,0,0,0};

void IO_KeyDelay(void)
{
	u8 i;
	i = 60;
	while(--i);
}

void 	IO_KeyScan(void)//50ms call
{
	u8 j;

	j = IO_KeyState1;// Save the last state

	P0 = 0xf0; //X low , read Y
	IO_KeyDelay();
	IO_KeyState1 = P0 & 0xf0;

	P0 = 0x0f; //Y low , read X
	IO_KeyDelay();
	IO_KeyState1 |= (P0 & 0x0f);
	IO_KeyState1 ^= 0xff;// Invert
	
	if(j == IO_KeyState1)// Two consecutive reads are equal
	{
		j = IO_KeyState;
		IO_KeyState = IO_KeyState1;
		if(IO_KeyState != 0)// There is a key pressed
		{
			F0 = 0;
			if(j == 0)F0 = 1;// First press
			else if(j == IO_KeyState)
			{
				if(++IO_KeyHoldCnt >= 20)// Re-key after 1 second
				{
					IO_KeyHoldCnt = 18;
					F0 = 1;
				}
			}
			if(F0)
			{
				j = T_KeyTable[IO_KeyState >> 4];
				if((j != 0) && (T_KeyTable[IO_KeyState& 0x0f] != 0))
					KeyCode = (j - 1) * 4 + T_KeyTable[IO_KeyState & 0x0f] + 16;// Calculate the key code , 17~32
			}
		}
		else IO_KeyHoldCnt = 0;
	}
	P0 = 0xff;
}




/**************** Send a byte function to HC595 ******************/
void Send_595(u8 dat)
{		
	u8 i;
	for(i=0; i<8; i++)
	{
		dat <<= 1;
		P_HC595_SER = CY;
		P_HC595_SRCLK = 1;
		P_HC595_SRCLK = 0;
	}
}

/************************ Show scan function ************************/
void DisplayScan(void)
{	
	Send_595(~T_COM[display_index]); 				// Output bit code
	Send_595(t_display[LED8[display_index]]);// Output segment code

	P_HC595_RCLK = 1;
	P_HC595_RCLK = 0; 							// Latch output data
	if(++display_index >= 8)display_index = 0;//The end of 8 bits returns to 0
}


/************************ Timer0 1ms interrupt function ************************ */
//void timer0 (void) interrupt TIMER0_VECTOR
INTERRUPT(int0, TF0_VECTOR)   //void t0int() interrupt 1            //запись прерывания
{
	DisplayScan();//1ms scan to display one bit
	B_1ms = 1; 		//1ms flag
}



#define SLAW 	0xA2
#define SLAR 	0xA3

#define  	SDA P1_1 // pin Define SDA 
#define 	SCL P1_0 // pin Define SCL 

/****************************/
void 	I2C_Delay(void)//for normal MCS51, delay (2 * dly + 4) T, for STC12Cxxxx delay (4 * dly + 10) T
{
	u8 dly;
	dly = FOSC / 2000000UL; 		// calculated by 2us
	while(--dly);
}

/****************************/
void I2C_Start(void) //start the I2C, SDA High-to-low when SCL is high
{
	SDA = 1;
	I2C_Delay();
	SCL = 1;
	I2C_Delay();
	SDA = 0;
	I2C_Delay();
	SCL = 0;
	I2C_Delay();
}		


void I2C_Stop(void) 					//STOP the I2C, SDA Low-to-high when SCL is high
{
	SDA = 0;
	I2C_Delay();
	SCL = 1;
	I2C_Delay();
	SDA = 1;
	I2C_Delay();
}

void S_ACK(void) //Send ACK (LOW)
{
	SDA = 0;
	I2C_Delay();
	SCL = 1;
	I2C_Delay();
	SCL = 0;
	I2C_Delay();
}

void S_NoACK(void) //Send No ACK (High)
{
	SDA = 1;
	I2C_Delay();
	SCL = 1;
	I2C_Delay();
	SCL = 0;
	I2C_Delay();
}
		
void I2C_Check_ACK(void) //Check ACK, If F0=0, then right, if F0=1, then error
{
	SDA = 1;
	I2C_Delay();
	SCL = 1;
	I2C_Delay();
	F0 = SDA;
	SCL = 0;
	I2C_Delay();
}

/****************************/
void I2C_WriteAbyte(u8 dat) 		//write a byte to I2C
{
	u8 i;
	i = 8;
	do
	{
		if(dat & 0x80) 
                   SDA = 1;
		else
                   SDA= 0;
		dat <<= 1;
		I2C_Delay();
		SCL = 1;
		I2C_Delay();
		SCL = 0;
		I2C_Delay();
	}
	while(--i);
}

/****************************/
u8 I2C_ReadAbyte(void) 			//read A byte from I2C
{
	u8 i,dat=0;
	i = 8;
	SDA = 1;
	do
	{
		SCL = 1;
		I2C_Delay();
		dat <<= 1;
		if(SDA)dat++;
		SCL = 0;
		I2C_Delay();
	}
	while(--i);
	return(dat);
}

/****************************/
void WriteNbyte(u8 addr, u8 *p, u8 number) 			/*WordAddress,First Data Address,Byte lenth*/
                         									//F0=0,right, F0=1,error
{
	I2C_Start();
	I2C_WriteAbyte(SLAW);
	I2C_Check_ACK();
	if(!F0)
	{
		I2C_WriteAbyte(addr);
		I2C_Check_ACK();
		if(!F0)
		{
			do
			{
				I2C_WriteAbyte(*p); 		p++;
				I2C_Check_ACK();
				if(F0)break;
			}
			while(--number);
		}
	}
	I2C_Stop();
}


/****************************/
void ReadNbyte(u8 addr, u8 *p, u8 number) 		/*WordAddress,First Data Address,Byte lenth*/
{
	I2C_Start();
	I2C_WriteAbyte(SLAW);
	I2C_Check_ACK();
	if(!F0)
	{
		I2C_WriteAbyte(addr);
		I2C_Check_ACK();
		if(!F0)
		{
			I2C_Start();
			I2C_WriteAbyte(SLAR);
			I2C_Check_ACK();
			if(!F0)
			{
				do
				{
					*p = I2C_ReadAbyte();p++;
					if(number != 1) 		S_ACK();//send ACK
				}
				while(--number);
				S_NoACK(); 			//send no ACK
			}
		}
	}
	I2C_Stop();
}


/****************************/


