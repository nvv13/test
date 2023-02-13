/*
  UTFT.cpp - Мульти-платформенная библиотека поддерживабщая цветные TFT LCD-панели
  Copyright (C)2015 Авторское Право: Rinky-Dink Electronics, Henning Karlsen. Все права защищены

  Эта библиотека является продолжением моих библиотек ITDB02_Graph, ITDB02_Graph16 и RGB_GLCD для Arduino и chipKit.
  Так как количество поддерживаемых модулей и контроллеров начало расти, я решил, что пришло время создать одну универсальную библиотеку.

  Основные функции этой библиотеки были созданы на основе демо-кода,
  предоставленного ITead Studio (для модулей ITDB02) и NKC Electronics (для RGB GLCD модулей/шилдов).

  Эта библиотека поддерживает 8 битные, 16 битные и последовательные интерфейсы графических дисплеев,
  и будет работать с платами Ардуино, chipKit и select TI LaunchPads.
  Полный список протестированных модулей и контроллеров укзан в файле UTFT_Supported_display_modules_&_controllers.pdf.

  При использовании 8 битных и 16 битных модулей, существуют некоторые требования, которые вы должны соблюдать.
  Эти требования можно найти в UTFT_Requirements.pdf.
  При использовании модулей с последовательным интерфейсом, нет никаких специальных требований.

  Вы можете найти свежую версию библиотеки перейдя по ссылке: http://www.RinkyDinkElectronics.com/

  Эта библиотека является свободным программным обеспечением;
  Вы можете распространять и/или изменять её в соответствии с пунктом 3.0 лицензии CC BY-NC-SA.

  Коммерческое использование этой библиотеки предусматривает приобретение лицензии.
  Включая использование библиотеки, модифицированной или нет, как инструмент для продажи продуктов.

  Лицензия распространяется на все библиотеки, в том числе примеры и инструменты, поставляемые с библиотекой.
*/

#include <string.h>
#include <math.h>

#include "UTFT.h"




#include "memorysaver.h"


#include "wm_io.h"
#include "wm_gpio.h"
#include "wm_gpio_afsel.h"


#include "hardware/sky/HW_W801_p.h"



/*
	The functions and variables below should not normally be used.
	They have been left publicly available for use in add-on libraries
	that might need access to the lower level functions of UTFT.

	Please note that these functions and variables are not documented
	and I do not provide support on how to use them.
*/
static		byte			fch, fcl, bch, bcl;
static		byte			orient;
static		long			disp_x_size, disp_y_size;
static		byte			display_model, display_transfer_mode, display_serial_mode;
static		regtype			*P_RS, *P_WR, *P_CS, *P_RST, *P_SDA, *P_SCL, *P_ALE;
static		regsize			B_RS, B_WR, B_CS, B_RST, B_SDA, B_SCL, B_ALE;
static		byte			__p1, __p2, __p3, __p4, __p5;
static		_current_font	cfont;
static		boolean			_transparent;
static		boolean			LCD_Write_1byte_Flag = 0;



static			void UTFT_LCD_Writ_Bus(char VH,char VL, byte mode);
static			void UTFT_LCD_Write_COM(char VL);
static			void UTFT_LCD_Write_DATA(char VH,char VL);
static			void UTFT_LCD_Write_DATA2(char VL);
static			void UTFT_LCD_Write_COM_DATA(char com1,int dat1);
static			void UTFT__hw_special_init();
static			void UTFT_setPixel(word color);
static			void UTFT_drawHLine(int x, int y, int l);
static			void UTFT_drawVLine(int x, int y, int l);
static			void UTFT_printChar(byte c, int x, int y);
static			void UTFT_setXY(word x1, word y1, word x2, word y2);
static			void UTFT_clrXY();
static			void UTFT_rotateChar(byte c, int x, int y, int pos, int deg);
static			void UTFT__set_direction_registers(byte mode);
static			void UTFT__fast_fill_16(int ch, int cl, long pix);
static			void UTFT__fast_fill_8(int ch, long pix);
static			void UTFT__convert_float(char *buf, double num, int width, byte prec);


// Подключение аппаратно-зависимых функций, для правильной работы микроконтроллеров
#include "hardware/sky/HW_SKY.h"
#include "hardware/sky/HW_W801.h"

extern void n_delay_ms (uint32_t ms);
static void delay(int ms){n_delay_ms(ms);};



void UTFT_UTFT(byte model, byte RS, byte WR, byte CS, byte RST, byte SER){
//	обозначение:				--		--		CTE32		--		DMTFT24104	--		--		--		CTE32W	    --			LPH9135		--		--		CTE50		--		--		ELEE32_REVA	--			--			ELEE32_REVB	CTE70		CTE32HR		CTE28		CTE22		--		DMTFT28105	MI0283QT9	CTE35IPS	CTE40		CTE50CPLD	DMTFT18101	--		--		--		--		--
//						--		--		--		--		DMTFT28103	--		--		--		--	    --			--		--		--		EHOUSE50	--		--		INFINIT32	--			--			--		EHOUSE70	--		--		DMTFT22102	--		--		--		--		--		CTE70CPLD	--		--		--		--		--		--
//						ITDB32		ITDB32WC	ITDB32S		ITDB24		ITDB28		--		ITDB22		ITDB22SP	ITDB32WD    ITDB18SP		--		ITDB25H		ITDB43		ITDB50		ITDB24E_8	--		--		--			--			--		--		--		--		--		--		--		--		--		--		EHOUSE50CPLD    --		--		--		--		--		--
//						--		--		--		--		ITDB24D		--		--		--		--	    --			--		--		--		--		--		--		--		--			--			--		--		--		--		--		--		TFT22SHLD	--		--		--		--		--		--		--		--		--		--
//						--		--		--		--		ITDB24DWOT	--		--		--		--	    --			--		--		--		--		--		--		--		--			--			--		--		--		--		--		--		TFT01_22SP	--		--		--		--		--		--		--		--		--		--
//						--		TFT01_32W	TFT01_32	--		TFT01_24_8  	TFT01_24_16	--		--		TFT01_32WD  --			--		--		TFT01_43	TFT01_50	TFT01_24R2	ITDB24E_16	--		--			--			--		TFT01_70	TFT32MEGA	TFT01_28	TFT01_22	TFT01_18SP	TFT01_24SP	--		--		--		--		--		TFT18SHLD	TFT28UNO	TFT28MEGA	TFT395UNO	TFT32MEGA_2             
//	контроллер:				HX8347-A	ILI9327		SSD1289		ILI9325C	ILI9325D	ILI9325D	HX8340-B	HX8340-B	HX8352-A    ST7735		PCF8833		S1D19122	SSD1963		SSD1963		S6D1121		S6D1121		SSD1289		--			--			SSD1289		SSD1963		ILI9481		ILI9325D	S6D0164		ST7735S		ILI9341		ILI9341		R61581		ILI9486		CPLD		HX8353C		ST7735		ILI9341		ILI9341		ILI9327		HX8357C                 ILI9225B
/*	размер x:	 */	word dsx[] = {	239,		239,		239,		239,		239,		239,		175,		175,		239,        127,		127,		239,		271,		479,		239,		239,		239,		0,			0,			239,		479,		319,		239,		175,		127,		239,		239,		319,		319,		799,		127,		127,		239,		239,		319,		319			,175};
/*	размер y:	 */	word dsy[] = {	319,		399,		319,		319,		319,		319,		219,		219,		399,        159,		127,		319,		479,		799,		319,		319,		319,		0,			0,			319,		799,		479,		319,		219,		159,		319,		319,		479,		479,		479,		159,		159,		319,		319,		479,		479			,219};
/*	размер шины: */		byte dtm[] = {	16,		16,		16,		8,		8,		16,		8,		SERIAL_4PIN,	16,	    SERIAL_5PIN,	SERIAL_5PIN,	16,		16,		16,		8,		16,		LATCHED_16,	0,			0,			8,		16,		16,		16,		8,		SERIAL_5PIN,	SERIAL_5PIN,	SERIAL_4PIN,	16,		16,		16,		SERIAL_5PIN, 	SERIAL_5PIN,	8,		8,		8,		16			, SERIAL_5PIN};

	disp_x_size =			dsx[model];
	disp_y_size =			dsy[model];
	display_transfer_mode =	dtm[model];
	display_model =			model;

	__p1 = RS;
	__p2 = WR;
	__p3 = CS;
	__p4 = RST;
	__p5 = SER;

	if (display_transfer_mode == SERIAL_4PIN)
	{
		display_transfer_mode=1;
		display_serial_mode=SERIAL_4PIN;
	}
	if (display_transfer_mode == SERIAL_5PIN)
	{
		display_transfer_mode=1;
		display_serial_mode=SERIAL_5PIN;
	}

	if (display_transfer_mode!=1)
	{
		UTFT__set_direction_registers(display_transfer_mode);
		P_RS	= portOutputRegister(digitalPinToPort(RS));
		B_RS	= digitalPinToBitMask(RS);
		P_WR	= portOutputRegister(digitalPinToPort(WR));
		B_WR	= digitalPinToBitMask(WR);
		P_CS	= portOutputRegister(digitalPinToPort(CS));
		B_CS	= digitalPinToBitMask(CS);
		P_RST	= portOutputRegister(digitalPinToPort(RST));
		B_RST	= digitalPinToBitMask(RST);
		if (display_transfer_mode==LATCHED_16)
		{
			P_ALE	= portOutputRegister(digitalPinToPort(SER));
			B_ALE	= digitalPinToBitMask(SER);
			cbi(P_ALE, B_ALE);
			pinMode(SER,OUTPUT);
			digitalWrite(SER, LOW);
		}
		if (display_model==ILI9341_UNO || display_model==ILI9341_MEGA || ILI9327_UNO){
			P_ALE	= portOutputRegister(digitalPinToPort(SER));
			B_ALE	= digitalPinToBitMask(SER);
			sbi(P_ALE, B_ALE);
		}
	}
	else
	{
		P_SDA	= portOutputRegister(digitalPinToPort(RS));
		B_SDA	= digitalPinToBitMask(RS);
		P_SCL	= portOutputRegister(digitalPinToPort(WR));
		B_SCL	= digitalPinToBitMask(WR);
		P_CS	= portOutputRegister(digitalPinToPort(CS));
		B_CS	= digitalPinToBitMask(CS);
		if (RST != NOTINUSE)
		{
			P_RST	= portOutputRegister(digitalPinToPort(RST));
			B_RST	= digitalPinToBitMask(RST);
		}
		if (display_serial_mode!=SERIAL_4PIN)
		{
			P_RS	= portOutputRegister(digitalPinToPort(SER));
			B_RS	= digitalPinToBitMask(SER);
		}
	}

}

void UTFT_LCD_Write_COM(char VL)  
{   
	if (display_transfer_mode!=1)
	{
		cbi(P_RS, B_RS);
		LCD_Write_1byte_Flag = 1;
		UTFT_LCD_Writ_Bus(0x00,VL,display_transfer_mode);
		LCD_Write_1byte_Flag = 0;
	}
	else
		UTFT_LCD_Writ_Bus(0x00,VL,display_transfer_mode);
}

void UTFT_LCD_Write_DATA(char VH,char VL)
{
	if (display_transfer_mode!=1)
	{
		sbi(P_RS, B_RS);
		UTFT_LCD_Writ_Bus(VH,VL,display_transfer_mode);
	}
	else
	{
		UTFT_LCD_Writ_Bus(0x01,VH,display_transfer_mode);
		UTFT_LCD_Writ_Bus(0x01,VL,display_transfer_mode);
	}
}

void UTFT_LCD_Write_DATA2(char VL)
{
	if (display_transfer_mode!=1)
	{
		sbi(P_RS, B_RS);
		LCD_Write_1byte_Flag = 1;
		UTFT_LCD_Writ_Bus(0x00,VL,display_transfer_mode);
		LCD_Write_1byte_Flag = 0;
	}
	else
		UTFT_LCD_Writ_Bus(0x01,VL,display_transfer_mode);
}

void UTFT_LCD_Write_COM_DATA(char com1,int dat1)
{
     UTFT_LCD_Write_COM(com1);
     UTFT_LCD_Write_DATA(dat1>>8,dat1);
}

void UTFT_InitLCD(byte orientation)
{
	orient=orientation;
	UTFT__hw_special_init();

	pinMode(__p1,OUTPUT);
	pinMode(__p2,OUTPUT);
	pinMode(__p3,OUTPUT);
	if (__p4 != NOTINUSE)
		pinMode(__p4,OUTPUT);
	if ((display_transfer_mode==LATCHED_16) || (display_model==ILI9341_UNO) || (display_model==ILI9341_MEGA) || (display_model==ILI9327_UNO) || ((display_transfer_mode==1) && (display_serial_mode==SERIAL_5PIN)))
		pinMode(__p5,OUTPUT);
	if (display_transfer_mode!=1)
		UTFT__set_direction_registers(display_transfer_mode);

	sbi(P_RST, B_RST);
	delay(5); 
	cbi(P_RST, B_RST);
	delay(15);
	sbi(P_RST, B_RST);
	delay(15);

	cbi(P_CS, B_CS);

	switch(display_model)
	{
#ifndef DISABLE_HX8347A
	#include "tft_drivers/hx8347a/initlcd.h"
#endif
#ifndef DISABLE_ILI9327
	#include "tft_drivers/ili9327/initlcd.h"
#endif
#ifndef DISABLE_ILI9327_UNO
	#include "tft_drivers/ili9327/uno/initlcd.h"
#endif
#ifndef DISABLE_SSD1289
	#include "tft_drivers/ssd1289/initlcd.h"
#endif
#ifndef DISABLE_ILI9325C
	#include "tft_drivers/ili9325c/initlcd.h"
#endif
#ifndef DISABLE_ILI9325D
	#include "tft_drivers/ili9325d/default/initlcd.h"
#endif
#ifndef DISABLE_ILI9325D_ALT
	#include "tft_drivers/ili9325d/alt/initlcd.h"
#endif
#ifndef DISABLE_HX8340B_8
	#include "tft_drivers/hx8340b/8/initlcd.h"
#endif
#ifndef DISABLE_HX8340B_S
	#include "tft_drivers/hx8340b/s/initlcd.h"
#endif
#ifndef DISABLE_ST7735
	#include "tft_drivers/st7735/std/initlcd.h"
#endif
#ifndef DISABLE_ST7735_ALT
	#include "tft_drivers/st7735/alt/initlcd.h"
#endif
#ifndef DISABLE_PCF8833
	#include "tft_drivers/pcf8833/initlcd.h"
#endif
#ifndef DISABLE_S1D19122
	#include "tft_drivers/s1d19122/initlcd.h"
#endif
#ifndef DISABLE_HX8352A
	#include "tft_drivers/hx8352a/initlcd.h"
#endif
#ifndef DISABLE_SSD1963_480
	#include "tft_drivers/ssd1963/480/initlcd.h"
#endif
#ifndef DISABLE_SSD1963_800
	#include "tft_drivers/ssd1963/800/initlcd.h"
#endif
#ifndef DISABLE_SSD1963_800_ALT
	#include "tft_drivers/ssd1963/800alt/initlcd.h"
#endif
#ifndef DISABLE_S6D1121
	#include "tft_drivers/s6d1121/initlcd.h"
#endif
#ifndef DISABLE_ILI9481
	#include "tft_drivers/ili9481/initlcd.h"
#endif
#ifndef DISABLE_S6D0164
	#include "tft_drivers/s6d0164/initlcd.h"
#endif
#ifndef DISABLE_ST7735S
	#include "tft_drivers/st7735s/initlcd.h"
#endif
#ifndef DISABLE_ILI9341_S4P
	#include "tft_drivers/ili9341/s4p/initlcd.h"
#endif
#ifndef DISABLE_ILI9341_S5P
	#include "tft_drivers/ili9341/s5p/initlcd.h"
#endif
#ifndef DISABLE_ILI9341_UNO
	#include "tft_drivers/ili9341/uno/initlcd.h"
#endif
#ifndef DISABLE_ILI9341_MEGA
	#include "tft_drivers/ili9341/mega/initlcd.h"
#endif
#ifndef DISABLE_R61581
	#include "tft_drivers/r61581/initlcd.h"
#endif
#ifndef DISABLE_ILI9486
	#include "tft_drivers/ili9486/initlcd.h"
#endif
#ifndef DISABLE_CPLD
	#include "tft_drivers/cpld/initlcd.h"
#endif
#ifndef DISABLE_HX8353C
	#include "tft_drivers/hx8353c/initlcd.h"
#endif
#ifndef DISABLE_HX8357C
	#include "tft_drivers/hx8357c/initlcd.h"
#endif
#ifndef DISABLE_ILI9225B
	#include "tft_drivers/ili9225b/initlcd.h"
#endif
	}
	sbi (P_CS, B_CS); 

	UTFT_setColor(255, 255, 255);
	UTFT_setBackColor(0, 0, 0);
	cfont.font=0;
	_transparent = false;
}

void UTFT_setXY(word x1, word y1, word x2, word y2)
{
	if (orient==LANDSCAPE)
	{
		swap_word( x1, y1);
		swap_word( x2, y2)
		y1=disp_y_size-y1;
		y2=disp_y_size-y2;
		swap_word( y1, y2)
	}

	switch(display_model)
	{
#ifndef DISABLE_HX8347A
	#include "tft_drivers/hx8347a/setxy.h"
#endif
#ifndef DISABLE_HX8352A
	#include "tft_drivers/hx8352a/setxy.h"
#endif
#ifndef DISABLE_ILI9327
	#include "tft_drivers/ili9327/setxy.h"
#endif
#ifndef DISABLE_ILI9327_UNO
	#include "tft_drivers/ili9327/uno/setxy.h"
#endif
#ifndef DISABLE_SSD1289
	#include "tft_drivers/ssd1289/setxy.h"
#endif
#ifndef DISABLE_ILI9325C
	#include "tft_drivers/ili9325c/setxy.h"
#endif
#ifndef DISABLE_ILI9325D
	#include "tft_drivers/ili9325d/default/setxy.h"
#endif
#ifndef DISABLE_ILI9325D_ALT
	#include "tft_drivers/ili9325d/alt/setxy.h"
#endif
#ifndef DISABLE_HX8340B_8
	#include "tft_drivers/hx8340b/8/setxy.h"
#endif
#ifndef DISABLE_HX8340B_S
	#include "tft_drivers/hx8340b/s/setxy.h"
#endif
#ifndef DISABLE_ST7735
	#include "tft_drivers/st7735/std/setxy.h"
#endif
#ifndef DISABLE_ST7735_ALT
	#include "tft_drivers/st7735/alt/setxy.h"
#endif
#ifndef DISABLE_S1D19122
	#include "tft_drivers/s1d19122/setxy.h"
#endif
#ifndef DISABLE_PCF8833
	#include "tft_drivers/pcf8833/setxy.h"
#endif
#ifndef DISABLE_SSD1963_480
	#include "tft_drivers/ssd1963/480/setxy.h"
#endif
#ifndef DISABLE_SSD1963_800
	#include "tft_drivers/ssd1963/800/setxy.h"
#endif
#ifndef DISABLE_SSD1963_800_ALT
	#include "tft_drivers/ssd1963/800alt/setxy.h"
#endif
#ifndef DISABLE_S6D1121
	#include "tft_drivers/s6d1121/setxy.h"
#endif
#ifndef DISABLE_ILI9481
	#include "tft_drivers/ili9481/setxy.h"
#endif
#ifndef DISABLE_S6D0164
	#include "tft_drivers/s6d0164/setxy.h"
#endif
#ifndef DISABLE_ST7735S
	#include "tft_drivers/st7735s/setxy.h"
#endif
#ifndef DISABLE_ILI9341_S4P
	#include "tft_drivers/ili9341/s4p/setxy.h"
#endif
#ifndef DISABLE_ILI9341_S5P
	#include "tft_drivers/ili9341/s5p/setxy.h"
#endif
#ifndef DISABLE_ILI9341_UNO
	#include "tft_drivers/ili9341/uno/setxy.h"
#endif
#ifndef DISABLE_ILI9341_MEGA
	#include "tft_drivers/ili9341/mega/setxy.h"
#endif
#ifndef DISABLE_R61581
	#include "tft_drivers/r61581/setxy.h"
#endif
#ifndef DISABLE_ILI9486
	#include "tft_drivers/ili9486/setxy.h"
#endif
#ifndef DISABLE_CPLD
	#include "tft_drivers/cpld/setxy.h"
#endif
#ifndef DISABLE_HX8353C
	#include "tft_drivers/hx8353c/setxy.h"
#endif
#ifndef DISABLE_HX8357C
	#include "tft_drivers/hx8357c/setxy.h"
#endif
#ifndef DISABLE_ILI9225B
	#include "tft_drivers/ili9225b/setxy.h"
#endif
	}
}

void UTFT_clrXY()
{
	if (orient==PORTRAIT)
		UTFT_setXY(0,0,disp_x_size,disp_y_size);
	else
		UTFT_setXY(0,0,disp_y_size,disp_x_size);
}

void UTFT_drawRect(int x1, int y1, int x2, int y2)
{
	if (x1>x2)
	{
		swap_int( x1, x2);
	}
	if (y1>y2)
	{
		swap_int( y1, y2);
	}

	UTFT_drawHLine(x1, y1, x2-x1);
	UTFT_drawHLine(x1, y2, x2-x1);
	UTFT_drawVLine(x1, y1, y2-y1);
	UTFT_drawVLine(x2, y1, y2-y1);
}

void UTFT_drawRoundRect(int x1, int y1, int x2, int y2)
{
	if (x1>x2)
	{
		swap_int( x1, x2);
	}
	if (y1>y2)
	{
		swap_int( y1, y2);
	}
	if ((x2-x1)>4 && (y2-y1)>4)
	{
		UTFT_drawPixel(x1+1,y1+1);
		UTFT_drawPixel(x2-1,y1+1);
		UTFT_drawPixel(x1+1,y2-1);
		UTFT_drawPixel(x2-1,y2-1);
		UTFT_drawHLine(x1+2, y1, x2-x1-4);
		UTFT_drawHLine(x1+2, y2, x2-x1-4);
		UTFT_drawVLine(x1, y1+2, y2-y1-4);
		UTFT_drawVLine(x2, y1+2, y2-y1-4);
	}
}

void UTFT_fillRect(int x1, int y1, int x2, int y2)
{
	if (x1>x2)
	{
		swap_int( x1, x2);
	}
	if (y1>y2)
	{
		swap_int( y1, y2);
	}
	if (display_transfer_mode==16)
	{
		cbi(P_CS, B_CS);
		UTFT_setXY(x1, y1, x2, y2);
		sbi(P_RS, B_RS);
		UTFT__fast_fill_16(fch,fcl,(((long)(x2-x1)+1)*((long)(y2-y1)+1)));
		sbi(P_CS, B_CS);
	}
	else if ((display_transfer_mode==8) && (fch==fcl))
	{
		cbi(P_CS, B_CS);
		UTFT_setXY(x1, y1, x2, y2);
		sbi(P_RS, B_RS);
		UTFT__fast_fill_8(fch,(((long)(x2-x1)+1)*((long)(y2-y1)+1)));
		sbi(P_CS, B_CS);
	}
	else
	{
		if (orient==PORTRAIT)
		{
			for (int i=0; i<((y2-y1)/2)+1; i++)
			{
				UTFT_drawHLine(x1, y1+i, x2-x1);
				UTFT_drawHLine(x1, y2-i, x2-x1);
			}
		}
		else
		{
			for (int i=0; i<((x2-x1)/2)+1; i++)
			{
				UTFT_drawVLine(x1+i, y1, y2-y1);
				UTFT_drawVLine(x2-i, y1, y2-y1);
			}
		}
	}
}

void UTFT_fillRoundRect(int x1, int y1, int x2, int y2)
{
	if (x1>x2)
	{
		swap_int( x1, x2);
	}
	if (y1>y2)
	{
		swap_int( y1, y2);
	}

	if ((x2-x1)>4 && (y2-y1)>4)
	{
		for (int i=0; i<((y2-y1)/2)+1; i++)
		{
			switch(i)
			{
			case 0:
				UTFT_drawHLine(x1+2, y1+i, x2-x1-4);
				UTFT_drawHLine(x1+2, y2-i, x2-x1-4);
				break;
			case 1:
				UTFT_drawHLine(x1+1, y1+i, x2-x1-2);
				UTFT_drawHLine(x1+1, y2-i, x2-x1-2);
				break;
			default:
				UTFT_drawHLine(x1, y1+i, x2-x1);
				UTFT_drawHLine(x1, y2-i, x2-x1);
			}
		}
	}
}

void UTFT_drawCircle(int x, int y, int radius)
{
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x1 = 0;
	int y1 = radius;
 
	cbi(P_CS, B_CS);
	UTFT_setXY(x, y + radius, x, y + radius);
	UTFT_LCD_Write_DATA(fch,fcl);
	UTFT_setXY(x, y - radius, x, y - radius);
	UTFT_LCD_Write_DATA(fch,fcl);
	UTFT_setXY(x + radius, y, x + radius, y);
	UTFT_LCD_Write_DATA(fch,fcl);
	UTFT_setXY(x - radius, y, x - radius, y);
	UTFT_LCD_Write_DATA(fch,fcl);
 
	while(x1 < y1)
	{
		if(f >= 0) 
		{
			y1--;
			ddF_y += 2;
			f += ddF_y;
		}
		x1++;
		ddF_x += 2;
		f += ddF_x;    
		UTFT_setXY(x + x1, y + y1, x + x1, y + y1);
		UTFT_LCD_Write_DATA(fch,fcl);
		UTFT_setXY(x - x1, y + y1, x - x1, y + y1);
		UTFT_LCD_Write_DATA(fch,fcl);
		UTFT_setXY(x + x1, y - y1, x + x1, y - y1);
		UTFT_LCD_Write_DATA(fch,fcl);
		UTFT_setXY(x - x1, y - y1, x - x1, y - y1);
		UTFT_LCD_Write_DATA(fch,fcl);
		UTFT_setXY(x + y1, y + x1, x + y1, y + x1);
		UTFT_LCD_Write_DATA(fch,fcl);
		UTFT_setXY(x - y1, y + x1, x - y1, y + x1);
		UTFT_LCD_Write_DATA(fch,fcl);
		UTFT_setXY(x + y1, y - x1, x + y1, y - x1);
		UTFT_LCD_Write_DATA(fch,fcl);
		UTFT_setXY(x - y1, y - x1, x - y1, y - x1);
		UTFT_LCD_Write_DATA(fch,fcl);
	}
	sbi(P_CS, B_CS);
	UTFT_clrXY();
}

void UTFT_fillCircle(int x, int y, int radius)
{
	for(int y1=-radius; y1<=0; y1++) 
		for(int x1=-radius; x1<=0; x1++)
			if(x1*x1+y1*y1 <= radius*radius) 
			{
				UTFT_drawHLine(x+x1, y+y1, 2*(-x1));
				UTFT_drawHLine(x+x1, y-y1, 2*(-x1));
				break;
			}
}

void UTFT_clrScr()
{
	long i;
	
	cbi(P_CS, B_CS);
	UTFT_clrXY();
	if (display_transfer_mode!=1)
		sbi(P_RS, B_RS);
	if (display_transfer_mode==16)
		UTFT__fast_fill_16(0,0,((disp_x_size+1)*(disp_y_size+1)));
	else if (display_transfer_mode==8)
		UTFT__fast_fill_8(0,((disp_x_size+1)*(disp_y_size+1)));
	else
	{
		for (i=0; i<((disp_x_size+1)*(disp_y_size+1)); i++)
		{
			if (display_transfer_mode!=1)
				UTFT_LCD_Writ_Bus(0,0,display_transfer_mode);
			else
			{
				UTFT_LCD_Writ_Bus(1,0,display_transfer_mode);
				UTFT_LCD_Writ_Bus(1,0,display_transfer_mode);
			}
		}
	}
	sbi(P_CS, B_CS);
}

void UTFT_fillScr(byte r, byte g, byte b)
{
	word color = ((r&248)<<8 | (g&252)<<3 | (b&248)>>3);
	UTFT_fillScr2(color);
}

void UTFT_fillScr2(word color)
{
	long i;
	char ch, cl;
	
	ch=(byte)(color>>8);
	cl=(byte)(color & 0xFF);

	cbi(P_CS, B_CS);
	UTFT_clrXY();
	if (display_transfer_mode!=1)
		sbi(P_RS, B_RS);
	if (display_transfer_mode==16)
		UTFT__fast_fill_16(ch,cl,((disp_x_size+1)*(disp_y_size+1)));
	else if ((display_transfer_mode==8) && (ch==cl))
		UTFT__fast_fill_8(ch,((disp_x_size+1)*(disp_y_size+1)));
	else
	{
		for (i=0; i<((disp_x_size+1)*(disp_y_size+1)); i++)
		{
			if (display_transfer_mode!=1)
				UTFT_LCD_Writ_Bus(ch,cl,display_transfer_mode);
			else
			{
				UTFT_LCD_Writ_Bus(1,ch,display_transfer_mode);
				UTFT_LCD_Writ_Bus(1,cl,display_transfer_mode);
			}
		}
	}
	sbi(P_CS, B_CS);
}

void UTFT_setColor(byte r, byte g, byte b)
{
	fch=((r&248)|g>>5);
	fcl=((g&28)<<3|b>>3);
}

void UTFT_setColor2(word color)
{
	fch=(byte)(color>>8);
	fcl=(byte)(color & 0xFF);
}

word UTFT_getColor()
{
	return (fch<<8) | fcl;
}

void UTFT_setBackColor(byte r, byte g, byte b)
{
	bch=((r&248)|g>>5);
	bcl=((g&28)<<3|b>>3);
	_transparent=false;
}

void UTFT_setBackColor2(uint32_t color)
{
	if (color==VGA_TRANSPARENT)
		_transparent=true;
	else
	{
		bch=(byte)(color>>8);
		bcl=(byte)(color & 0xFF);
		_transparent=false;
	}
}

word UTFT_getBackColor()
{
	return (bch<<8) | bcl;
}

void UTFT_setPixel(word color)
{
	UTFT_LCD_Write_DATA((color>>8),(color&0xFF));	// rrrrrggggggbbbbb
}

void UTFT_drawPixel(int x, int y)
{
	cbi(P_CS, B_CS);
	UTFT_setXY(x, y, x, y);
	UTFT_setPixel((fch<<8)|fcl);
	sbi(P_CS, B_CS);
	UTFT_clrXY();
}

void UTFT_drawLine(int x1, int y1, int x2, int y2)
{
	if (y1==y2)
		UTFT_drawHLine(x1, y1, x2-x1);
	else if (x1==x2)
		UTFT_drawVLine(x1, y1, y2-y1);
	else
	{
		unsigned int	dx = (x2 > x1 ? x2 - x1 : x1 - x2);
		short			xstep =  x2 > x1 ? 1 : -1;
		unsigned int	dy = (y2 > y1 ? y2 - y1 : y1 - y2);
		short			ystep =  y2 > y1 ? 1 : -1;
		int				col = x1, row = y1;

		cbi(P_CS, B_CS);
		if (dx < dy)
		{
			int t = - (dy >> 1);
			while (true)
			{
				UTFT_setXY (col, row, col, row);
				UTFT_LCD_Write_DATA(fch, fcl);
				if (row == y2)
					return;
				row += ystep;
				t += dx;
				if (t >= 0)
				{
					col += xstep;
					t   -= dy;
				}
			} 
		}
		else
		{
			int t = - (dx >> 1);
			while (true)
			{
				UTFT_setXY (col, row, col, row);
				UTFT_LCD_Write_DATA(fch, fcl);
				if (col == x2)
					return;
				col += xstep;
				t += dy;
				if (t >= 0)
				{
					row += ystep;
					t   -= dx;
				}
			} 
		}
		sbi(P_CS, B_CS);
	}
	UTFT_clrXY();
}

void UTFT_drawHLine(int x, int y, int l)
{
	if (l<0)
	{
		l = -l;
		x -= l;
	}
	cbi(P_CS, B_CS);
	UTFT_setXY(x, y, x+l, y);
	if (display_transfer_mode == 16)
	{
		sbi(P_RS, B_RS);
		UTFT__fast_fill_16(fch,fcl,l);
	}
	else if ((display_transfer_mode==8) && (fch==fcl))
	{
		sbi(P_RS, B_RS);
		UTFT__fast_fill_8(fch,l);
	}
	else
	{
		for (int i=0; i<l+1; i++)
		{
			UTFT_LCD_Write_DATA(fch, fcl);
		}
	}
	sbi(P_CS, B_CS);
	UTFT_clrXY();
}

void UTFT_drawVLine(int x, int y, int l)
{
	if (l<0)
	{
		l = -l;
		y -= l;
	}
	cbi(P_CS, B_CS);
	UTFT_setXY(x, y, x, y+l);
	if (display_transfer_mode == 16)
	{
		sbi(P_RS, B_RS);
		UTFT__fast_fill_16(fch,fcl,l);
	}
	else if ((display_transfer_mode==8) && (fch==fcl))
	{
		sbi(P_RS, B_RS);
		UTFT__fast_fill_8(fch,l);
	}
	else
	{
		for (int i=0; i<l+1; i++)
		{
			UTFT_LCD_Write_DATA(fch, fcl);
		}
	}
	sbi(P_CS, B_CS);
	UTFT_clrXY();
}

void UTFT_printChar(byte c, int x, int y)
{
	byte i,ch;
	word j;
	word temp; 

	cbi(P_CS, B_CS);
  
	if (!_transparent)
	{
		if (orient==PORTRAIT)
		{
			UTFT_setXY(x,y,x+cfont.x_size-1,y+cfont.y_size-1);
	  
			temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
			for(j=0;j<((cfont.x_size/8)*cfont.y_size);j++)
			{
				ch=pgm_read_byte(&cfont.font[temp]);
				for(i=0;i<8;i++)
				{   
					if((ch&(1<<(7-i)))!=0)   
					{
						UTFT_setPixel((fch<<8)|fcl);
					} 
					else
					{
						UTFT_setPixel((bch<<8)|bcl);
					}   
				}
				temp++;
			}
		}
		else
		{
			temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;

			for(j=0;j<((cfont.x_size/8)*cfont.y_size);j+=(cfont.x_size/8))
			{
				UTFT_setXY(x,y+(j/(cfont.x_size/8)),x+cfont.x_size-1,y+(j/(cfont.x_size/8)));
				for (int zz=(cfont.x_size/8)-1; zz>=0; zz--)
				{
					ch=pgm_read_byte(&cfont.font[temp+zz]);
					for(i=0;i<8;i++)
					{   
						if((ch&(1<<i))!=0)   
						{
							UTFT_setPixel((fch<<8)|fcl);
						} 
						else
						{
							UTFT_setPixel((bch<<8)|bcl);
						}   
					}
				}
				temp+=(cfont.x_size/8);
			}
		}
	}
	else
	{
		temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
		for(j=0;j<cfont.y_size;j++) 
		{
			for (int zz=0; zz<(cfont.x_size/8); zz++)
			{
				ch=pgm_read_byte(&cfont.font[temp+zz]); 
				for(i=0;i<8;i++)
				{   
				
					if((ch&(1<<(7-i)))!=0)   
					{
						UTFT_setXY(x+i+(zz*8),y+j,x+i+(zz*8)+1,y+j+1);
						UTFT_setPixel((fch<<8)|fcl);
					} 
				}
			}
			temp+=(cfont.x_size/8);
		}
	}

	sbi(P_CS, B_CS);
	UTFT_clrXY();
}

void UTFT_rotateChar(byte c, int x, int y, int pos, int deg)
{
	byte i,j,ch;
	word temp; 
	int newx,newy;
	double radian;
	radian=deg*0.0175;  

	cbi(P_CS, B_CS);

	temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
	for(j=0;j<cfont.y_size;j++) 
	{
		for (int zz=0; zz<(cfont.x_size/8); zz++)
		{
			ch=pgm_read_byte(&cfont.font[temp+zz]); 
			for(i=0;i<8;i++)
			{   
				newx=x+(((i+(zz*8)+(pos*cfont.x_size))*cos(radian))-((j)*sin(radian)));
				newy=y+(((j)*cos(radian))+((i+(zz*8)+(pos*cfont.x_size))*sin(radian)));

				UTFT_setXY(newx,newy,newx+1,newy+1);
				
				if((ch&(1<<(7-i)))!=0)   
				{
					UTFT_setPixel((fch<<8)|fcl);
				} 
				else  
				{
					if (!_transparent)
						UTFT_setPixel((bch<<8)|bcl);
				}   
			}
		}
		temp+=(cfont.x_size/8);
	}
	sbi(P_CS, B_CS);
	UTFT_clrXY();
}

void UTFT_print(char *st, int x, int y, int deg)
{
	int stl, i;

	stl = strlen(st);

	if (orient==PORTRAIT)
	{
	if (x==RIGHT)
		x=(disp_x_size+1)-(stl*cfont.x_size);
	if (x==CENTER)
		x=((disp_x_size+1)-(stl*cfont.x_size))/2;
	}
	else
	{
	if (x==RIGHT)
		x=(disp_y_size+1)-(stl*cfont.x_size);
	if (x==CENTER)
		x=((disp_y_size+1)-(stl*cfont.x_size))/2;
	}

	for (i=0; i<stl; i++)
		if (deg==0)
			UTFT_printChar(*st++, x + (i*(cfont.x_size)), y);
		else
			UTFT_rotateChar(*st++, x, y, i, deg);
}

/*
void UTFT_print(String st, int x, int y, int deg)
{
	char buf[st.length()+1];

	st.toCharArray(buf, st.length()+1);
	UTFT_print(buf, x, y, deg);
}
*/

void UTFT_printNumI(long num, int x, int y, int length, char filler)
{
	char buf[25];
	char st[27];
	boolean neg=false;
	int c=0, f=0;
  
	if (num==0)
	{
		if (length!=0)
		{
			for (c=0; c<(length-1); c++)
				st[c]=filler;
			st[c]=48;
			st[c+1]=0;
		}
		else
		{
			st[0]=48;
			st[1]=0;
		}
	}
	else
	{
		if (num<0)
		{
			neg=true;
			num=-num;
		}
	  
		while (num>0)
		{
			buf[c]=48+(num % 10);
			c++;
			num=(num-(num % 10))/10;
		}
		buf[c]=0;
	  
		if (neg)
		{
			st[0]=45;
		}
	  
		if (length>(c+neg))
		{
			for (int i=0; i<(length-c-neg); i++)
			{
				st[i+neg]=filler;
				f++;
			}
		}

		for (int i=0; i<c; i++)
		{
			st[i+neg+f]=buf[c-i-1];
		}
		st[c+neg+f]=0;

	}

	UTFT_print(st,x,y,0);
}

void UTFT_printNumF(double num, byte dec, int x, int y, char divider, int length, char filler)
{
	char st[27];
	boolean neg=false;

	if (dec<1)
		dec=1;
	else if (dec>5)
		dec=5;

	if (num<0)
		neg = true;

	UTFT__convert_float(st, num, length, dec);

	if (divider != '.')
	{
		for (int i=0; i<sizeof(st); i++)
			if (st[i]=='.')
				st[i]=divider;
	}

	if (filler != ' ')
	{
		if (neg)
		{
			st[0]='-';
			for (int i=1; i<sizeof(st); i++)
				if ((st[i]==' ') || (st[i]=='-'))
					st[i]=filler;
		}
		else
		{
			for (int i=0; i<sizeof(st); i++)
				if (st[i]==' ')
					st[i]=filler;
		}
	}

	UTFT_print(st,x,y,0);
}

void UTFT_setFont(uint8_t* font)
{
	cfont.font=font;
	cfont.x_size=fontbyte(0);
	cfont.y_size=fontbyte(1);
	cfont.offset=fontbyte(2);
	cfont.numchars=fontbyte(3);
}

uint8_t* UTFT_getFont()
{
	return cfont.font;
}

uint8_t UTFT_getFontXsize()
{
	return cfont.x_size;
}

uint8_t UTFT_getFontYsize()
{
	return cfont.y_size;
}

void UTFT_drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int scale)
{
	unsigned int col;
	int tx, ty, tc, tsx, tsy;

	if (scale==1)
	{
		if (orient==PORTRAIT)
		{
			cbi(P_CS, B_CS);
			UTFT_setXY(x, y, x+sx-1, y+sy-1);
			for (tc=0; tc<(sx*sy); tc++)
			{
				col=pgm_read_word(&data[tc]);
				UTFT_LCD_Write_DATA(col>>8,col & 0xff);
			}
			sbi(P_CS, B_CS);
		}
		else
		{
			cbi(P_CS, B_CS);
			for (ty=0; ty<sy; ty++)
			{
				UTFT_setXY(x, y+ty, x+sx-1, y+ty);
				for (tx=sx-1; tx>=0; tx--)
				{
					col=pgm_read_word(&data[(ty*sx)+tx]);
					UTFT_LCD_Write_DATA(col>>8,col & 0xff);
				}
			}
			sbi(P_CS, B_CS);
		}
	}
	else
	{
		if (orient==PORTRAIT)
		{
			cbi(P_CS, B_CS);
			for (ty=0; ty<sy; ty++)
			{
				UTFT_setXY(x, y+(ty*scale), x+((sx*scale)-1), y+(ty*scale)+scale);
				for (tsy=0; tsy<scale; tsy++)
					for (tx=0; tx<sx; tx++)
					{
						col=pgm_read_word(&data[(ty*sx)+tx]);
						for (tsx=0; tsx<scale; tsx++)
							UTFT_LCD_Write_DATA(col>>8,col & 0xff);
					}
			}
			sbi(P_CS, B_CS);
		}
		else
		{
			cbi(P_CS, B_CS);
			for (ty=0; ty<sy; ty++)
			{
				for (tsy=0; tsy<scale; tsy++)
				{
					UTFT_setXY(x, y+(ty*scale)+tsy, x+((sx*scale)-1), y+(ty*scale)+tsy);
					for (tx=sx-1; tx>=0; tx--)
					{
						col=pgm_read_word(&data[(ty*sx)+tx]);
						for (tsx=0; tsx<scale; tsx++)
							UTFT_LCD_Write_DATA(col>>8,col & 0xff);
					}
				}
			}
			sbi(P_CS, B_CS);
		}
	}
	UTFT_clrXY();
}

void UTFT_drawBitmap2(int x, int y, int sx, int sy, bitmapdatatype data, int deg, int rox, int roy)
{
	unsigned int col;
	int tx, ty, newx, newy;
	double radian;
	radian=deg*0.0175;  

	if (deg==0)
		UTFT_drawBitmap(x, y, sx, sy, data, 0);
	else
	{
		cbi(P_CS, B_CS);
		for (ty=0; ty<sy; ty++)
			for (tx=0; tx<sx; tx++)
			{
				col=pgm_read_word(&data[(ty*sx)+tx]);

				newx=x+rox+(((tx-rox)*cos(radian))-((ty-roy)*sin(radian)));
				newy=y+roy+(((ty-roy)*cos(radian))+((tx-rox)*sin(radian)));

				UTFT_setXY(newx, newy, newx, newy);
				UTFT_LCD_Write_DATA(col>>8,col & 0xff);
			}
		sbi(P_CS, B_CS);
	}
	UTFT_clrXY();
}

void UTFT_lcdOff()
{
	cbi(P_CS, B_CS);
	switch (display_model)
	{
	case PCF8833:
		UTFT_LCD_Write_COM(0x28);
		break;
	case CPLD:
		UTFT_LCD_Write_COM_DATA(0x01,0x0000);
		UTFT_LCD_Write_COM(0x0F);   
		break;
	}
	sbi(P_CS, B_CS);
}

void UTFT_lcdOn()
{
	cbi(P_CS, B_CS);
	switch (display_model)
	{
	case PCF8833:
		UTFT_LCD_Write_COM(0x29);
		break;
	case CPLD:
		UTFT_LCD_Write_COM_DATA(0x01,0x0010);
		UTFT_LCD_Write_COM(0x0F);   
		break;
	}
	sbi(P_CS, B_CS);
}

void UTFT_setContrast(char c)
{
	cbi(P_CS, B_CS);
	switch (display_model)
	{
	case PCF8833:
		if (c>64) c=64;
		UTFT_LCD_Write_COM(0x25);
		UTFT_LCD_Write_DATA2(c);
		break;
	}
	sbi(P_CS, B_CS);
}

int UTFT_getDisplayXSize()
{
	if (orient==PORTRAIT)
		return disp_x_size+1;
	else
		return disp_y_size+1;
}

int UTFT_getDisplayYSize()
{
	if (orient==PORTRAIT)
		return disp_y_size+1;
	else
		return disp_x_size+1;
}

void UTFT_setBrightness(byte br)
{
	cbi(P_CS, B_CS);
	switch (display_model)
	{
	case CPLD:
		if (br>16) br=16;
		UTFT_LCD_Write_COM_DATA(0x01,br);
		UTFT_LCD_Write_COM(0x0F);   
		break;
	}
	sbi(P_CS, B_CS);
}

void UTFT_setDisplayPage(byte page)
{
	cbi(P_CS, B_CS);
	switch (display_model)
	{
	case CPLD:
		if (page>7) page=7;
		UTFT_LCD_Write_COM_DATA(0x04,page);
		UTFT_LCD_Write_COM(0x0F);   
		break;
	}
	sbi(P_CS, B_CS);
}

void UTFT_setWritePage(byte page)
{
	cbi(P_CS, B_CS);
	switch (display_model)
	{
	case CPLD:
		if (page>7) page=7;
		UTFT_LCD_Write_COM_DATA(0x05,page);
		UTFT_LCD_Write_COM(0x0F);   
		break;
	}
	sbi(P_CS, B_CS);
}





#include "wm_gpio_afsel.h"
#include "ff.h"

#define SERIAL_DEBUG
//#define SERIAL_DEBUG_ALL

u8 file_buffer[512] = {0};

word UTFT_loadBitmap(int x, int y, int sx, int sy, char *filename)
{

	FATFS fs; //FatFs file system object
	FIL fnew; // file object
	FRESULT res_sd;// file operation results
	UINT fnum; // The number of files successfully read and written

	int cx, cy, cp;

 	wm_sdio_host_config(0);

	//mount SD card
	res_sd = f_mount(&fs, "0:", 1);
	
	//***********************formatting test****************************
	if(res_sd == FR_NO_FILESYSTEM)
	{
		return FR_NO_FILESYSTEM;
	}
	else if(res_sd != FR_OK)
	{
#ifdef SERIAL_DEBUG
               printf ("Failed to mount file system! Probably because the file initialization failed! error code:%d\r\n", res_sd);
#endif
		return FR_NO_FILESYSTEM;
	}
	else
	{
#ifdef SERIAL_DEBUG_ALL
               printf ("The file system is successfully mounted, and the read and write test can be performed!\r\n");
#endif
	}

	//Open the file
	res_sd = f_open(&fnew, /*"0:FatFs_test_files.txt"*/ filename, FA_OPEN_EXISTING | FA_READ);
	//file opened successfully?
	if(res_sd == FR_OK)
	{
#ifdef SERIAL_DEBUG_ALL
                printf ("Open file successfully! Start reading data!\r\n");
#endif
		cbi(P_CS, B_CS);
		cx=0;
		cy=0;
		fnum=sizeof(file_buffer);
		if (orient==PORTRAIT)
		{
			UTFT_setXY(x, y, x+sx-1, y+sy-1);
		}
		while(fnum==sizeof(file_buffer))
		{
			res_sd= f_read(&fnew, file_buffer, sizeof(file_buffer), &fnum);
			if(res_sd == FR_OK)
			{
#ifdef SERIAL_DEBUG_ALL
                        printf ("Data read successfully!\r\n");
                        printf ("data: %s\r\n", file_buffer);
#endif
					if (orient==PORTRAIT)
					{
						for (int i=0; i<fnum; i+=2)
							UTFT_LCD_Write_DATA(file_buffer[i],file_buffer[i+1]);
					}
					else
					{
						cp=0;
						while (cp<fnum)
						{
							if (((fnum-cp)/2)<(sx-cx))
							{
								UTFT_setXY(x+cx, y+cy, x+cx+((fnum-cp)/2)-1, y+cy);
								for (int i=(fnum-cp)-2; i>=0; i-=2)
									UTFT_LCD_Write_DATA(file_buffer[cp+i],file_buffer[cp+i+1]);
								cx+=((fnum-cp)/2);
								cp=fnum;
							}
							else
							{
								UTFT_setXY(x+cx, y+cy, x+sx-1, y+cy);
								for (int i=sx-cx-1; i>=0; i--)
									UTFT_LCD_Write_DATA(file_buffer[cp+(i*2)],file_buffer[cp+(i*2)+1]);
								cp+=(sx-cx)*2;
								cx=0;
								cy++;
							}
						}
					}

			}
			else
			{
#ifdef SERIAL_DEBUG
                        printf ("Data reading failed!\r\n");
#endif
			break;
			}
		}
		//close file
		f_close(&fnew);
		UTFT_setXY(0,0,UTFT_getDisplayXSize()-1,UTFT_getDisplayYSize()-1);
		sbi(P_CS, B_CS);
	}
	
	//unmount file system
	f_mount(NULL, "0:", 1);
	return res_sd;

/*
	int res;
	int cx, cy, cp;
	word result;

	res=file_openFile(filename, FILEMODE_BINARY);
	if (res==NO_ERROR)
	{
		cbi(P_CS, B_CS);
		cx=0;
		cy=0;
		result=512;
		if (orient==PORTRAIT)
		{
			UTFT_setXY(x, y, x+sx-1, y+sy-1);
		}
		while (result==512)
		{
			result=file_readBinary();
			switch(result)
			{
				case ERROR_WRONG_FILEMODE:
					return ERROR_WRONG_FILEMODE;
					break;
				case ERROR_NO_FILE_OPEN:
					return ERROR_NO_FILE_OPEN;
					break;
				default:
					if (orient==PORTRAIT)
					{
						for (int i=0; i<result; i+=2)
							UTFT_LCD_Write_DATA(file_buffer[i],file_buffer[i+1]);
					}
					else
					{
						cp=0;
						while (cp<result)
						{
							if (((result-cp)/2)<(sx-cx))
							{
								UTFT_setXY(x+cx, y+cy, x+cx+((result-cp)/2)-1, y+cy);
								for (int i=(result-cp)-2; i>=0; i-=2)
									UTFT_LCD_Write_DATA(file_buffer[cp+i],file_buffer[cp+i+1]);
								cx+=((result-cp)/2);
								cp=result;
							}
							else
							{
								UTFT_setXY(x+cx, y+cy, x+sx-1, y+cy);
								for (int i=sx-cx-1; i>=0; i--)
									UTFT_LCD_Write_DATA(file_buffer[cp+(i*2)],file_buffer[cp+(i*2)+1]);
								cp+=(sx-cx)*2;
								cx=0;
								cy++;
							}
						}
					}
					break;
			}              
		}
		file_closeFile();
		UTFT_setXY(0,0,UTFT_getDisplayXSize()-1,UTFT_getDisplayYSize()-1);
		sbi(P_CS, B_CS);
		return 0;
	}
	else
	{
		return res;
	}
*/
}


