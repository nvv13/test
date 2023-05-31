/*
  URTouch.cpp - Arduino/chipKit library support for Color TFT LCD Touch screens 
  Copyright (C)2018 Rinky-Dink Electronics, Henning Karlsen. All right reserved
  
  Basic functionality of this library are based on the demo-code provided by
  ITead studio.

  You can find the latest version of the library at 
  http://www.RinkyDinkElectronics.com/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.

  Commercial use of this library requires you to buy a license that
  will allow commercial use. This includes using the library,
  modified or not, as a tool to sell products.

  The license applies to all part of the library including the 
  examples and tools supplied with the library.
*/

#include "URTouch.h"
#include "URTouchCD.h"





#include "hardware/sky/HW_SKY_defines.h"

static uint32_t CAL_X = 0x00378F66UL;
static uint32_t CAL_Y = 0x03C34155UL;
static uint32_t CAL_S = 0x000EF13FUL;


static regtype *P_CLK, *P_CS, *P_DIN, *P_DOUT, *P_IRQ;
static regsize B_CLK, B_CS, B_DIN, B_DOUT, B_IRQ;
static byte	T_CLK, T_CS, T_DIN, T_DOUT, T_IRQ;
static long	_default_orientation;
static byte	orient;
static byte	prec;
//static byte	display_model;
static long	disp_x_size, disp_y_size;//, default_orientation;
static long	touch_x_left, touch_x_right, touch_y_top, touch_y_bottom;



static void	URTouch_touch_WriteData(byte data);
static word	URTouch_touch_ReadData();

#include "hardware/sky/HW_W801_p.h"
#include "hardware/sky/HW_SKY.h"


void URTouch_URTouch(byte tclk, byte tcs, byte din, byte dout, byte irq)
{
	T_CLK	= tclk;
	T_CS	= tcs;
	T_DIN	= din;
	T_DOUT	= dout;
	T_IRQ	= irq;
}

void URTouch_set_calibrate(uint32_t calx,uint32_t  caly,uint32_t  cals)
{
CAL_X = calx;
CAL_Y = caly;
CAL_S = cals;
	_default_orientation	= CAL_S>>31;
	touch_x_left			= (CAL_X>>14) & 0x3FFF;
	touch_x_right			= CAL_X & 0x3FFF;
	touch_y_top				= (CAL_Y>>14) & 0x3FFF;
	touch_y_bottom			= CAL_Y & 0x3FFF;
	disp_x_size				= (CAL_S>>12) & 0x0FFF;
	disp_y_size				= CAL_S & 0x0FFF;
}

void URTouch_InitTouch(byte orientation)
{
	orient					= orientation;
	_default_orientation	= CAL_S>>31;
	touch_x_left			= (CAL_X>>14) & 0x3FFF;
	touch_x_right			= CAL_X & 0x3FFF;
	touch_y_top				= (CAL_Y>>14) & 0x3FFF;
	touch_y_bottom			= CAL_Y & 0x3FFF;
	disp_x_size				= (CAL_S>>12) & 0x0FFF;
	disp_y_size				= CAL_S & 0x0FFF;
	prec					= 10;

	P_CLK	= portOutputRegister(digitalPinToPort(T_CLK));
	B_CLK	= digitalPinToBitMask(T_CLK);
	P_CS	= portOutputRegister(digitalPinToPort(T_CS));
	B_CS	= digitalPinToBitMask(T_CS);
	P_DIN	= portOutputRegister(digitalPinToPort(T_DIN));
	B_DIN	= digitalPinToBitMask(T_DIN);
	P_DOUT	= portInputRegister(digitalPinToPort(T_DOUT));
	B_DOUT	= digitalPinToBitMask(T_DOUT);
	P_IRQ	= portInputRegister(digitalPinToPort(T_IRQ));
	B_IRQ	= digitalPinToBitMask(T_IRQ);

	pinMode(T_CLK,  OUTPUT);
    pinMode(T_CS,   OUTPUT);
    pinMode(T_DIN,  OUTPUT);
    pinMode(T_DOUT, INPUT);
    pinMode(T_IRQ,  OUTPUT);

	sbi(P_CS, B_CS);
	sbi(P_CLK, B_CLK);
	sbi(P_DIN, B_DIN);
	sbi(P_IRQ, B_IRQ);
}

void URTouch_read()
{
	unsigned long tx=0, temp_x=0;
	unsigned long ty=0, temp_y=0;
	unsigned long minx=99999, maxx=0;
	unsigned long miny=99999, maxy=0;
	int datacount=0;

	cbi(P_CS, B_CS);                    

	pinMode(T_IRQ,  INPUT);
	for (int i=0; i<prec; i++)
	{
		if (!rbi(P_IRQ, B_IRQ))
		{
			URTouch_touch_WriteData(0x90);        
			pulse_high(P_CLK, B_CLK);
			temp_x=URTouch_touch_ReadData();

			if (!rbi(P_IRQ, B_IRQ))
			{
				URTouch_touch_WriteData(0xD0);      
				pulse_high(P_CLK, B_CLK);
				temp_y=URTouch_touch_ReadData();

				if ((temp_x>0) && (temp_x<4096) && (temp_y>0) && (temp_y<4096))
				{
					tx+=temp_x;
					ty+=temp_y;
					if (prec>5)
					{
						if (temp_x<minx)
							minx=temp_x;
						if (temp_x>maxx)
							maxx=temp_x;
						if (temp_y<miny)
							miny=temp_y;
						if (temp_y>maxy)
							maxy=temp_y;
					}
					datacount++;
				}
			}
		}
	}
	pinMode(T_IRQ,  OUTPUT);

	if (prec>5)
	{
		tx = tx-(minx+maxx);
		ty = ty-(miny+maxy);
		datacount -= 2;
	}

	sbi(P_CS, B_CS);                    
	if ((datacount==(prec-2)) || (datacount==PREC_LOW))
	{
		if (orient == _default_orientation)
		{
			URTouch_TP_X=ty/datacount;
			URTouch_TP_Y=tx/datacount;
		}
		else
		{
			URTouch_TP_X=tx/datacount;
			URTouch_TP_Y=ty/datacount;
		}
	}
	else
	{
		URTouch_TP_X=-1;
		URTouch_TP_Y=-1;
	}
}

bool URTouch_dataAvailable()
{
	bool avail;
	pinMode(T_IRQ,  INPUT);
	avail = !(rbi(P_IRQ, B_IRQ));
	pinMode(T_IRQ,  OUTPUT);
	return avail;
}

int16_t URTouch_getX()
{
	long c;

	if ((URTouch_TP_X==-1) || (URTouch_TP_Y==-1))
		return -1;
	if (orient == _default_orientation)
	{
		c = (long)((long)(URTouch_TP_X - touch_x_left) * (disp_x_size)) / (long)(touch_x_right - touch_x_left);
		if (c<0)
			c = 0;
		if (c>disp_x_size)
			c = disp_x_size;
	}
	else
	{
		if (_default_orientation == PORTRAIT)
			c = (long)((long)(URTouch_TP_X - touch_y_top) * (-disp_y_size)) / (long)(touch_y_bottom - touch_y_top) + (long)(disp_y_size);
		else
			c = (long)((long)(URTouch_TP_X - touch_y_top) * (disp_y_size)) / (long)(touch_y_bottom - touch_y_top);
		if (c<0)
			c = 0;
		if (c>disp_y_size)
			c = disp_y_size;
	}
	return c;
}

int16_t URTouch_getY()
{
	int c;

	if ((URTouch_TP_X==-1) || (URTouch_TP_Y==-1))
		return -1;
	if (orient == _default_orientation)
	{
		c = (long)((long)(URTouch_TP_Y - touch_y_top) * (disp_y_size)) / (long)(touch_y_bottom - touch_y_top);
		if (c<0)
			c = 0;
		if (c>disp_y_size)
			c = disp_y_size;
	}
	else
	{
		if (_default_orientation == PORTRAIT)
			c = (long)((long)(URTouch_TP_Y - touch_x_left) * (disp_x_size)) / (long)(touch_x_right - touch_x_left);
		else
			c = (long)((long)(URTouch_TP_Y - touch_x_left) * (-disp_x_size)) / (long)(touch_x_right - touch_x_left) + (long)(disp_x_size);
		if (c<0)
			c = 0;
		if (c>disp_x_size)
			c = disp_x_size;
	}
	return c;
}

void URTouch_setPrecision(byte precision)
{
	switch (precision)
	{
		case PREC_LOW:
			prec=1;		// DO NOT CHANGE!
			break;
		case PREC_MEDIUM:
			prec=12;	// Iterations + 2
			break;
		case PREC_HI:
			prec=27;	// Iterations + 2
			break;
		case PREC_EXTREME:
			prec=102;	// Iterations + 2
			break;
		default:
			prec=12;	// Iterations + 2
			break;
	}
}

void URTouch_calibrateRead()
{
	unsigned long tx=0;
	unsigned long ty=0;

	cbi(P_CS, B_CS);                    

	URTouch_touch_WriteData(0x90);        
	pulse_high(P_CLK, B_CLK);
	tx=URTouch_touch_ReadData();

	URTouch_touch_WriteData(0xD0);      
	pulse_high(P_CLK, B_CLK);
	ty=URTouch_touch_ReadData();

	sbi(P_CS, B_CS);                    

	URTouch_TP_X=ty;
	URTouch_TP_Y=tx;
}

