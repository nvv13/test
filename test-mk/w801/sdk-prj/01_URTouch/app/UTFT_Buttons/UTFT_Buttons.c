/*
  UTFT_Buttons.cpp - Add-on Library for UTFT: Buttons
  Copyright (C)2016 Rinky-Dink Electronics, Henning Karlsen. All right reserved
  
  This library adds simple but easy to use buttons to extend the use
  of the UTFT and URTouch libraries.

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

#include <string.h>

#include "UTFT_Buttons.h"
#include <mod1/UTFT.h>
#include <mod1/URTouch.h>

//static UTFT		*_UTFT;
//static URTouch		*_URTouch;
static button_type	buttons[MAX_BUTTONS];
static word		_color_text, _color_text_inactive, _color_background, _color_border, _color_hilite;
static uint8_t		*_font_text, *_font_symbol;


void UTFT_Buttons_UTFT_Buttons(void)
{
	//_UTFT = ptrUTFT;
	//_URTouch = ptrURTouch;
	UTFT_Buttons_deleteAllButtons();
	_color_text				= VGA_WHITE;
	_color_text_inactive	= VGA_GRAY;
	_color_background		= VGA_BLUE;
	_color_border			= VGA_WHITE;
	_color_hilite			= VGA_RED;
	_font_text				= NULL;
	_font_symbol			= NULL;
}

int UTFT_Buttons_addButton(uint16_t x, uint16_t y, uint16_t width, uint16_t height, char *label, uint16_t flags)
{
	int btcnt = 0;
  
	while (((buttons[btcnt].flags & BUTTON_UNUSED) == 0) && (btcnt<MAX_BUTTONS))
		btcnt++;
  
	if (btcnt == MAX_BUTTONS)
		return -1;
	else
	{
		buttons[btcnt].pos_x  = x;
		buttons[btcnt].pos_y  = y;
		buttons[btcnt].width  = width;
		buttons[btcnt].height = height;
		buttons[btcnt].flags  = flags;
		buttons[btcnt].label  = label;
		buttons[btcnt].data   = NULL;
		return btcnt;
	}
}

int UTFT_Buttons_addButton2(uint16_t x, uint16_t y, uint16_t width, uint16_t height, bitmapdatatype data, uint16_t flags)
{
	int btcnt = 0;
  
	while (((buttons[btcnt].flags & BUTTON_UNUSED) == 0) && (btcnt<MAX_BUTTONS))
		btcnt++;
  
	if (btcnt == MAX_BUTTONS)
		return -1;
	else
	{
		buttons[btcnt].pos_x  = x;
		buttons[btcnt].pos_y  = y;
		buttons[btcnt].width  = width;
		buttons[btcnt].height = height;
		buttons[btcnt].flags  = flags | BUTTON_BITMAP;
		buttons[btcnt].label  = NULL;
		buttons[btcnt].data   = data;
		return btcnt;
	}
}


void UTFT_Buttons_drawButtons()
{
	for (int i=0;i<MAX_BUTTONS;i++)
	{
		if ((buttons[i].flags & BUTTON_UNUSED) == 0)
			UTFT_Buttons_drawButton(i);
	}
}

void UTFT_Buttons_drawButton(int buttonID)
{
	int		text_x, text_y;
	uint8_t	*_font_current = UTFT_getFont();;
	word	_current_color = UTFT_getColor();
	word	_current_back  = UTFT_getBackColor();

	if (buttons[buttonID].flags & BUTTON_BITMAP)
	{
		UTFT_drawBitmap(buttons[buttonID].pos_x, buttons[buttonID].pos_y, buttons[buttonID].width, buttons[buttonID].height, buttons[buttonID].data,0);
		if (!(buttons[buttonID].flags & BUTTON_NO_BORDER))
		{
			if ((buttons[buttonID].flags & BUTTON_DISABLED))
				UTFT_setColor2(_color_text_inactive);
			else
				UTFT_setColor2(_color_border);
			UTFT_drawRect(buttons[buttonID].pos_x, buttons[buttonID].pos_y, buttons[buttonID].pos_x+buttons[buttonID].width, buttons[buttonID].pos_y+buttons[buttonID].height);
		}
	}
	else
	{
		UTFT_setColor2(_color_background);
		UTFT_fillRoundRect(buttons[buttonID].pos_x, buttons[buttonID].pos_y, buttons[buttonID].pos_x+buttons[buttonID].width, buttons[buttonID].pos_y+buttons[buttonID].height);
		UTFT_setColor2(_color_border);
		UTFT_drawRoundRect(buttons[buttonID].pos_x, buttons[buttonID].pos_y, buttons[buttonID].pos_x+buttons[buttonID].width, buttons[buttonID].pos_y+buttons[buttonID].height);
		if (buttons[buttonID].flags & BUTTON_DISABLED)
			UTFT_setColor2(_color_text_inactive);
		else
			UTFT_setColor2(_color_text);
		if (buttons[buttonID].flags & BUTTON_SYMBOL)
		{
			UTFT_setFont(_font_symbol);
			text_x = (buttons[buttonID].width/2) - (UTFT_getFontXsize()/2) + buttons[buttonID].pos_x;
			text_y = (buttons[buttonID].height/2) - (UTFT_getFontYsize()/2) + buttons[buttonID].pos_y;
		}
		else
		{
			UTFT_setFont(_font_text);
			text_x = ((buttons[buttonID].width/2) - ((strlen(buttons[buttonID].label) * UTFT_getFontXsize())/2)) + buttons[buttonID].pos_x;
			text_y = (buttons[buttonID].height/2) - (UTFT_getFontYsize()/2) + buttons[buttonID].pos_y;
		}
		UTFT_setBackColor2(_color_background);
		UTFT_print(buttons[buttonID].label, text_x, text_y,0);
		if ((buttons[buttonID].flags & BUTTON_SYMBOL) && (buttons[buttonID].flags & BUTTON_SYMBOL_REP_3X))
		{
			UTFT_print(buttons[buttonID].label, text_x-UTFT_getFontXsize(), text_y,0);
			UTFT_print(buttons[buttonID].label, text_x+UTFT_getFontXsize(), text_y,0);
		}
	}
	UTFT_setFont(_font_current);
	UTFT_setColor2(_current_color);
	UTFT_setBackColor2(_current_back);
}

void UTFT_Buttons_enableButton(int buttonID, boolean redraw)
{
	if (!(buttons[buttonID].flags & BUTTON_UNUSED))
	{
		buttons[buttonID].flags = buttons[buttonID].flags & ~BUTTON_DISABLED;
		if (redraw)
			UTFT_Buttons_drawButton(buttonID);
	}
}

void UTFT_Buttons_disableButton(int buttonID, boolean redraw)
{
	if (!(buttons[buttonID].flags & BUTTON_UNUSED))
	{
		buttons[buttonID].flags = buttons[buttonID].flags | BUTTON_DISABLED;
		if (redraw)
			UTFT_Buttons_drawButton(buttonID);
	}
}

void UTFT_Buttons_relabelButton(int buttonID, char *label, boolean redraw)
{
	if (!(buttons[buttonID].flags & BUTTON_UNUSED))
	{
		buttons[buttonID].label = label;
		if (redraw)
			UTFT_Buttons_drawButton(buttonID);
	}
}

boolean UTFT_Buttons_buttonEnabled(int buttonID)
{
	return !(buttons[buttonID].flags & BUTTON_DISABLED);
}
    
void UTFT_Buttons_deleteButton(int buttonID)
{
	if (!(buttons[buttonID].flags & BUTTON_UNUSED))
		buttons[buttonID].flags = BUTTON_UNUSED;
}

void UTFT_Buttons_deleteAllButtons()
{
	for (int i=0;i<MAX_BUTTONS;i++)
	{
		buttons[i].pos_x=0;
		buttons[i].pos_y=0;
		buttons[i].width=0;
		buttons[i].height=0;
		buttons[i].flags=BUTTON_UNUSED;
		buttons[i].label="";
	}
}

int UTFT_Buttons_checkButtons()
{
    if (URTouch_dataAvailable() == true)
    {
		URTouch_read();
		int		result = -1;
		int		touch_x = URTouch_getX();
		int		touch_y = URTouch_getY();
		word	_current_color = UTFT_getColor();
  
		for (int i=0;i<MAX_BUTTONS;i++)
		{
			if (((buttons[i].flags & BUTTON_UNUSED) == 0) && ((buttons[i].flags & BUTTON_DISABLED) == 0) && (result == -1))
			{
				if ((touch_x >= buttons[i].pos_x) && (touch_x <= (buttons[i].pos_x + buttons[i].width)) && (touch_y >= buttons[i].pos_y) && (touch_y <= (buttons[i].pos_y + buttons[i].height)))
					result = i;
			}
		}
		if (result != -1)
		{
			if (!(buttons[result].flags & BUTTON_NO_BORDER))
			{
				UTFT_setColor2(_color_hilite);
				if (buttons[result].flags & BUTTON_BITMAP)
					UTFT_drawRect(buttons[result].pos_x, buttons[result].pos_y, buttons[result].pos_x+buttons[result].width, buttons[result].pos_y+buttons[result].height);
				else
					UTFT_drawRoundRect(buttons[result].pos_x, buttons[result].pos_y, buttons[result].pos_x+buttons[result].width, buttons[result].pos_y+buttons[result].height);
			}
		}

		while (URTouch_dataAvailable() == true) {};

		if (result != -1)
		{
			if (!(buttons[result].flags & BUTTON_NO_BORDER))
			{
				UTFT_setColor2(_color_border);
				if (buttons[result].flags & BUTTON_BITMAP)
					UTFT_drawRect(buttons[result].pos_x, buttons[result].pos_y, buttons[result].pos_x+buttons[result].width, buttons[result].pos_y+buttons[result].height);
				else
					UTFT_drawRoundRect(buttons[result].pos_x, buttons[result].pos_y, buttons[result].pos_x+buttons[result].width, buttons[result].pos_y+buttons[result].height);
			}
		}

		UTFT_setColor2(_current_color);
		return result;
	}
	else
		return -1;
}

void UTFT_Buttons_setTextFont(uint8_t* font)
{
	_font_text = font;
}

void UTFT_Buttons_setSymbolFont(uint8_t* font)
{
	_font_symbol = font;
}

void UTFT_Buttons_setButtonColors(word atxt, word iatxt, word brd, word brdhi, word back)
{
	_color_text				= atxt;
	_color_text_inactive	= iatxt;
	_color_background		= back;
	_color_border			= brd;
	_color_hilite			= brdhi;
}
