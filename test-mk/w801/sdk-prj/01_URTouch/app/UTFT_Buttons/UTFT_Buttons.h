/*
  UTFT_Buttons.h - Add-on Library for UTFT: Buttons
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

#ifndef UTFT_Buttons_h
#define UTFT_Buttons_h

#define UTFT_BUTTONS_VERSION	103


#include <mod1/UTFT.h>
#include <mod1/URTouch.h>
#if ((!defined(UTFT_VERSION)) || (UTFT_VERSION<241))
	#error : You will need UTFT v2.41 or higher to use this add-on library...
#endif

#include "hardware/sky/HW_SKY_defines.h"

#define MAX_BUTTONS	20	// Maximum number of buttons available at one time

// Define presets for button status
#define BUTTON_DISABLED			0x0001
#define BUTTON_SYMBOL			0x0002
#define BUTTON_SYMBOL_REP_3X	0x0004
#define BUTTON_BITMAP			0x0008	
#define BUTTON_NO_BORDER		0x0010
#define BUTTON_UNUSED			0x8000

typedef struct
{
  uint16_t			pos_x, pos_y, width, height;
  uint16_t			flags;
  char				*label;
  bitmapdatatype	data;
} button_type;

		void UTFT_Buttons_UTFT_Buttons(void);

		int	UTFT_Buttons_addButton(uint16_t x, uint16_t y, uint16_t width, uint16_t height, char *label, uint16_t flags);//, uint16_t flags=0
		int	UTFT_Buttons_addButton2(uint16_t x, uint16_t y, uint16_t width, uint16_t height, bitmapdatatype data, uint16_t flags);//, uint16_t flags=0

		void	UTFT_Buttons_drawButtons();
		void	UTFT_Buttons_drawButton(int buttonID);
		void	UTFT_Buttons_enableButton(int buttonID, boolean redraw);//, boolean redraw=false
		void	UTFT_Buttons_disableButton(int buttonID, boolean redraw);//, boolean redraw=false
		void	UTFT_Buttons_relabelButton(int buttonID, char *label, boolean redraw);//, boolean redraw=false
		boolean	UTFT_Buttons_buttonEnabled(int buttonID);
		void	UTFT_Buttons_deleteButton(int buttonID);
		void	UTFT_Buttons_deleteAllButtons();
		int		UTFT_Buttons_checkButtons();
		void	UTFT_Buttons_setTextFont(uint8_t* font);
		void	UTFT_Buttons_setSymbolFont(uint8_t* font);
		void	UTFT_Buttons_setButtonColors(word atxt, word iatxt, word brd, word brdhi, word back);


#endif
