case ST7735S:

	UTFT_LCD_Write_COM(0x11);//Sleep exit 
	delay(120);
 
	//ST7735R Frame Rate
	UTFT_LCD_Write_COM(0xB1); 
	UTFT_LCD_Write_DATA2(0x05);
	UTFT_LCD_Write_DATA2(0x3C);
	UTFT_LCD_Write_DATA2(0x3C); 
	UTFT_LCD_Write_COM(0xB2); 
	UTFT_LCD_Write_DATA2(0x05);
	UTFT_LCD_Write_DATA2(0x3C);
	UTFT_LCD_Write_DATA2(0x3C); 
	UTFT_LCD_Write_COM(0xB3); 
	UTFT_LCD_Write_DATA2(0x05);
	UTFT_LCD_Write_DATA2(0x3C);
	UTFT_LCD_Write_DATA2(0x3C); 
	UTFT_LCD_Write_DATA2(0x05);
	UTFT_LCD_Write_DATA2(0x3C);
	UTFT_LCD_Write_DATA2(0x3C); 

	UTFT_LCD_Write_COM(0xB4); //Column inversion 
	UTFT_LCD_Write_DATA2(0x03); 
 
	//ST7735R Power Sequence
	UTFT_LCD_Write_COM(0xC0); 
	UTFT_LCD_Write_DATA2(0x28);
	UTFT_LCD_Write_DATA2(0x08);
	UTFT_LCD_Write_DATA2(0x04); 
	UTFT_LCD_Write_COM(0xC1);
	UTFT_LCD_Write_DATA2(0xC0); 
	UTFT_LCD_Write_COM(0xC2); 
	UTFT_LCD_Write_DATA2(0x0D);
	UTFT_LCD_Write_DATA2(0x00); 
	UTFT_LCD_Write_COM(0xC3); 
	UTFT_LCD_Write_DATA2(0x8D);
	UTFT_LCD_Write_DATA2(0x2A); 
	UTFT_LCD_Write_COM(0xC4); 
	UTFT_LCD_Write_DATA2(0x8D);
	UTFT_LCD_Write_DATA2(0xEE); 
 
	UTFT_LCD_Write_COM(0xC5); //VCOM 
	UTFT_LCD_Write_DATA2(0x1A); 
 
	UTFT_LCD_Write_COM(0x36); //MX, MY, RGB mode 
	UTFT_LCD_Write_DATA2(0xC0); 

	//ST7735R Gamma Sequence
	UTFT_LCD_Write_COM(0xE0); 
	UTFT_LCD_Write_DATA2(0x03);
	UTFT_LCD_Write_DATA2(0x22); 
	UTFT_LCD_Write_DATA2(0x07);
	UTFT_LCD_Write_DATA2(0x0A); 
	UTFT_LCD_Write_DATA2(0x2E);
	UTFT_LCD_Write_DATA2(0x30); 
	UTFT_LCD_Write_DATA2(0x25);
	UTFT_LCD_Write_DATA2(0x2A); 
	UTFT_LCD_Write_DATA2(0x28);
	UTFT_LCD_Write_DATA2(0x26); 
	UTFT_LCD_Write_DATA2(0x2E);
	UTFT_LCD_Write_DATA2(0x3A);
	UTFT_LCD_Write_DATA2(0x00); 
	UTFT_LCD_Write_DATA2(0x01); 
	UTFT_LCD_Write_DATA2(0x03);
	UTFT_LCD_Write_DATA2(0x13); 
	UTFT_LCD_Write_COM(0xE1); 
	UTFT_LCD_Write_DATA2(0x04);
	UTFT_LCD_Write_DATA2(0x16); 
	UTFT_LCD_Write_DATA2(0x06);
	UTFT_LCD_Write_DATA2(0x0D); 
	UTFT_LCD_Write_DATA2(0x2D);
	UTFT_LCD_Write_DATA2(0x26); 
	UTFT_LCD_Write_DATA2(0x23);
	UTFT_LCD_Write_DATA2(0x27); 
	UTFT_LCD_Write_DATA2(0x27);
	UTFT_LCD_Write_DATA2(0x25); 
	UTFT_LCD_Write_DATA2(0x2D);
	UTFT_LCD_Write_DATA2(0x3B); 
	UTFT_LCD_Write_DATA2(0x00);
	UTFT_LCD_Write_DATA2(0x01); 
	UTFT_LCD_Write_DATA2(0x04);
	UTFT_LCD_Write_DATA2(0x13);  

	//UTFT_LCD_Write_COM(0x2A);
	//UTFT_LCD_Write_DATA2(0x00);
	//UTFT_LCD_Write_DATA2(0x00);
	//UTFT_LCD_Write_DATA2(0x00);
	//UTFT_LCD_Write_DATA2(0x7F);
	//UTFT_LCD_Write_COM(0x2B);
	//UTFT_LCD_Write_DATA2(0x00);
	//UTFT_LCD_Write_DATA2(0x00);
	//UTFT_LCD_Write_DATA2(0x00);
	//UTFT_LCD_Write_DATA2(0x9F);

	UTFT_LCD_Write_COM(0x3A); //65k mode 
	UTFT_LCD_Write_DATA2(0x05); 
	UTFT_LCD_Write_COM(0x29);//Display on
	break;

/*
case TFT00_96SP+100: // off case

    UTFT_LCD_Write_COM(0x11);//Sleep exit
    delay(120);
    //ST7735R Frame Rate
    UTFT_LCD_Write_COM(0xB1);  //Normal mode
    UTFT_LCD_Write_DATA2(0x05);
    UTFT_LCD_Write_DATA2(0x3C);
    UTFT_LCD_Write_DATA2(0x3C); 
    UTFT_LCD_Write_COM(0xB2);   //Idle mode
    UTFT_LCD_Write_DATA2(0x05);
    UTFT_LCD_Write_DATA2(0x3C);
    UTFT_LCD_Write_DATA2(0x3C); 
    UTFT_LCD_Write_COM(0xB3);   //Partial mode
    UTFT_LCD_Write_DATA2(0x05);
    UTFT_LCD_Write_DATA2(0x3C);
    UTFT_LCD_Write_DATA2(0x3C); 
    UTFT_LCD_Write_DATA2(0x05);
    UTFT_LCD_Write_DATA2(0x3C);
    UTFT_LCD_Write_DATA2(0x3C); 
    UTFT_LCD_Write_COM(0xB4); //Column inversion, Dot inversion 
    UTFT_LCD_Write_DATA2(0x03); 
        delay(10);
 
    //ST7735R Power Sequence
    UTFT_LCD_Write_COM(0xC0);   //AVDD GVDD
    UTFT_LCD_Write_DATA2(0x28);  //0xAB
    UTFT_LCD_Write_DATA2(0x08);  //0x0B
    UTFT_LCD_Write_DATA2(0x04);  //0x04
    UTFT_LCD_Write_COM(0xC1);  //VGH VGL
    UTFT_LCD_Write_DATA2(0xC0);  //0xC5
    UTFT_LCD_Write_COM(0xC2); //Normal Mode
    UTFT_LCD_Write_DATA2(0x0D);
    UTFT_LCD_Write_DATA2(0x00); 
    UTFT_LCD_Write_COM(0xC3);  //Idle
    UTFT_LCD_Write_DATA2(0x8D);
    UTFT_LCD_Write_DATA2(0x2A); //0x6A
    UTFT_LCD_Write_COM(0xC4);  //Partial+Full
    UTFT_LCD_Write_DATA2(0x8D);
    UTFT_LCD_Write_DATA2(0xEE); 
 
    UTFT_LCD_Write_COM(0xC5); //VCOM 
    UTFT_LCD_Write_DATA2(0x1A); //0x0F 0x0E

    UTFT_LCD_Write_COM(0x36); //MX, MY, RGB mode ST77xx_Cmd_MADCTL 0x36 Memory data access control 
//    UTFT_LCD_Write_DATA2(0xC0); default
//    UTFT_LCD_Write_DATA2(0xC8); 

#define ST77XX_MADCTL_MY 0x80
#define ST77XX_MADCTL_MX 0x40 //0x36 // was 0x40 - default!
#define ST77XX_MADCTL_MV 0x20
#define ST77XX_MADCTL_ML 0x10
#define ST77XX_MADCTL_RGB 0x00
    UTFT_LCD_Write_DATA2(ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST77XX_MADCTL_RGB); // rotation(0)
//    UTFT_LCD_Write_DATA2(ST77XX_MADCTL_MY | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB); // rotation(1)
//    UTFT_LCD_Write_DATA2(ST77XX_MADCTL_RGB); // rotation(2)
//    UTFT_LCD_Write_DATA2(ST77XX_MADCTL_MX | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB); // rotation(3)

//Hi Boyeen. Thanks to my seller the problem is solved. 
//Change in Adafruit-ST7735-Library : 
//Adafruit_ST7xx.h -> #define ST77XX_MADCTL_MX 0x36 // was 0x40. 
//This solved the problem! I use: tft.initR(INITR_MINI160x80);. 
//Now I can use the full graphicstest.ino for this blue tab board. 
//Look in Sitronics ST7735S V1.3 pg 142.

*/
/*
    lcd_write_cmd(conf,0x36);
    if(conf->direction==0)lcd_write_data(conf,0x08); Rotation?
    else if(conf->direction==1)lcd_write_data(conf,0xC8);
    else if(conf->direction==2)lcd_write_data(conf,0x78);
    else lcd_write_data(conf,0xA8);
*/
/*
        delay(10);
    //ST7735R Gamma Sequence
    UTFT_LCD_Write_COM(0xE0); //positive gamma
    //ST7735_GMCTRP1, 16      ,       //  1: Gamma Adjustments (pos. polarity), 16 args + delay:
    UTFT_LCD_Write_DATA2(0x02);
    UTFT_LCD_Write_DATA2(0x1c);
    UTFT_LCD_Write_DATA2(0x07);
    UTFT_LCD_Write_DATA2(0x12);
    UTFT_LCD_Write_DATA2(0x37);
    UTFT_LCD_Write_DATA2(0x32);
    UTFT_LCD_Write_DATA2(0x29);
    UTFT_LCD_Write_DATA2(0x2d);
    UTFT_LCD_Write_DATA2(0x29);
    UTFT_LCD_Write_DATA2(0x25);
    UTFT_LCD_Write_DATA2(0x2B);
    UTFT_LCD_Write_DATA2(0x39);
    UTFT_LCD_Write_DATA2(0x00);
    UTFT_LCD_Write_DATA2(0x01);
    UTFT_LCD_Write_DATA2(0x03);
    UTFT_LCD_Write_DATA2(0x10);
//    ST7735_GMCTRN1, 16      ,       //  2: Gamma Adjustments (neg. polarity), 16 args + delay:
    UTFT_LCD_Write_COM(0xE1); 
    UTFT_LCD_Write_DATA2(0x03);
    UTFT_LCD_Write_DATA2(0x1d);
    UTFT_LCD_Write_DATA2(0x07);
    UTFT_LCD_Write_DATA2(0x06);
    UTFT_LCD_Write_DATA2(0x2E);
    UTFT_LCD_Write_DATA2(0x2C);
    UTFT_LCD_Write_DATA2(0x29);
    UTFT_LCD_Write_DATA2(0x2D);
    UTFT_LCD_Write_DATA2(0x2E);
    UTFT_LCD_Write_DATA2(0x2E);
    UTFT_LCD_Write_DATA2(0x37);
    UTFT_LCD_Write_DATA2(0x3F);
    UTFT_LCD_Write_DATA2(0x00);
    UTFT_LCD_Write_DATA2(0x00);
    UTFT_LCD_Write_DATA2(0x02);
    UTFT_LCD_Write_DATA2(0x10);
        delay(100);
//    ST77XX_NORON,     ST_CMD_DELAY, //  3: Normal display on, no args, w/delay
//      10,                           //     10 ms delay
//    ST77XX_DISPON,    ST_CMD_DELAY, //  4: Main screen turn on, no args w/delay
//      100 };                        //     100 ms delay
*/
/*
    lcd_write_cmd(conf,0xFC);
    lcd_write_data(conf,0x80);
*/
//    UTFT_LCD_Write_COM(0xFC); //?
//    UTFT_LCD_Write_DATA2(0x80);
/*
    UTFT_LCD_Write_COM(0x2A); //Set Column Address
    UTFT_LCD_Write_DATA2(0x00);
    UTFT_LCD_Write_DATA2(0x00);
    UTFT_LCD_Write_DATA2(0x00);
    UTFT_LCD_Write_DATA2(0x4F);
    UTFT_LCD_Write_COM(0x2B); //Set Page Address
    UTFT_LCD_Write_DATA2(0x00);
    UTFT_LCD_Write_DATA2(0x00);
    UTFT_LCD_Write_DATA2(0x00);
    UTFT_LCD_Write_DATA2(0x9F);
*/
/*
    UTFT_LCD_Write_COM(0x3A); //65k mode ST77xx_Cmd_COLMOD
#define ST7735_ColorMode_12bit  0x03
#define ST7735_ColorMode_16bit  0x05
#define ST7735_ColorMode_18bit  0x06
    UTFT_LCD_Write_DATA2(ST7735_ColorMode_16bit); //       ST7735_ColorMode_16bit   
//--------------------------------------orent disp grad-----------------------------// 
//    UTFT_LCD_Write_COM(0x36); //-- orent disp grad
//    UTFT_LCD_Write_DATA2(0x08);//-- direction=0  
//                0x000300C8, -- direction=1 90
//                0x00030078, -- direction=2 180 
//                0x000300A8, -- direction=3 270
        delay(500);
    UTFT_LCD_Write_COM(0x29);//Display on
        delay(100);
	break;

case TFT00_96SP+124: // off case
//   https://github.com/Bodmer/TFT_eSPI/discussions/1310?sort=new#discussioncomment-1246522 

	UTFT_LCD_Write_COM(0xfe);
	UTFT_LCD_Write_COM(0xfe);
	UTFT_LCD_Write_COM(0xfe);
	UTFT_LCD_Write_COM(0xef);

	UTFT_LCD_Write_COM(0xb3);
	UTFT_LCD_Write_DATA2(0x03);

	UTFT_LCD_Write_COM(0x36);
	UTFT_LCD_Write_DATA2(0xd8);

	UTFT_LCD_Write_COM(0x3a);
	UTFT_LCD_Write_DATA2(0x05);

	UTFT_LCD_Write_COM(0xb6);
	UTFT_LCD_Write_DATA2(0x11);

	UTFT_LCD_Write_COM(0xac);
	UTFT_LCD_Write_DATA2(0x0b);

	UTFT_LCD_Write_COM(0xb4);
	UTFT_LCD_Write_DATA2(0x21);

	UTFT_LCD_Write_COM(0xb1);
	UTFT_LCD_Write_DATA2(0xc0);

	UTFT_LCD_Write_COM(0xe6);
	UTFT_LCD_Write_DATA2(0x50);
	UTFT_LCD_Write_DATA2(0x43);

	UTFT_LCD_Write_COM(0xe7);
	UTFT_LCD_Write_DATA2(0x56);
	UTFT_LCD_Write_DATA2(0x43);

	UTFT_LCD_Write_COM(0xF0);
	UTFT_LCD_Write_DATA2(0x1f);
	UTFT_LCD_Write_DATA2(0x41);
	UTFT_LCD_Write_DATA2(0x1B);
	UTFT_LCD_Write_DATA2(0x55);
	UTFT_LCD_Write_DATA2(0x36);
	UTFT_LCD_Write_DATA2(0x3d);
	UTFT_LCD_Write_DATA2(0x3e);
	UTFT_LCD_Write_DATA2(0x0);
	UTFT_LCD_Write_DATA2(0x16);
	UTFT_LCD_Write_DATA2(0x08);
	UTFT_LCD_Write_DATA2(0x09);
	UTFT_LCD_Write_DATA2(0x15);
	UTFT_LCD_Write_DATA2(0x14);
	UTFT_LCD_Write_DATA2(0xf);

	UTFT_LCD_Write_COM(0xF1);
	UTFT_LCD_Write_DATA2(0x1f);
	UTFT_LCD_Write_DATA2(0x41);
	UTFT_LCD_Write_DATA2(0x1B);
	UTFT_LCD_Write_DATA2(0x55);
	UTFT_LCD_Write_DATA2(0x36);
	UTFT_LCD_Write_DATA2(0x3d);
	UTFT_LCD_Write_DATA2(0x3e);
	UTFT_LCD_Write_DATA2(0x0);
	UTFT_LCD_Write_DATA2(0x16);
	UTFT_LCD_Write_DATA2(0x08);
	UTFT_LCD_Write_DATA2(0x09);
	UTFT_LCD_Write_DATA2(0x15);
	UTFT_LCD_Write_DATA2(0x14);
	UTFT_LCD_Write_DATA2(0xf);

	UTFT_LCD_Write_COM(0xfe);
	UTFT_LCD_Write_COM(0xff);

	UTFT_LCD_Write_COM(0x35);
	UTFT_LCD_Write_DATA2(0x00);

	UTFT_LCD_Write_COM(0x44);
	UTFT_LCD_Write_DATA2(0x00);

	UTFT_LCD_Write_COM(0x11);
        delay(120);
	UTFT_LCD_Write_COM(0x29);
*/
/*
Lcd_WriteIndex(0x2A); //Set Column Address
Lcd_WriteData(0x00);
Lcd_WriteData(0x18);
Lcd_WriteData(0x00);
Lcd_WriteData(0x67);
Lcd_WriteIndex(0x2B); //Set Page Address
Lcd_WriteData(0x00);
Lcd_WriteData(0x00);
Lcd_WriteData(0x00);
Lcd_WriteData(0x9f);
Lcd_WriteIndex(0x2c);
*/
//	break;








case TFT00_96SP: // on case

// 7735R init, part 1 (red or green tab)
// 15 commands in list:
#define ST77XX_SWRESET 0x01
    UTFT_LCD_Write_COM(ST77XX_SWRESET);
 //  1: Software reset, 0 args, w/delay
 //     150 ms delay
    delay(150);

#define ST77XX_SLPOUT 0x11
    UTFT_LCD_Write_COM(ST77XX_SLPOUT);
 //  2: Out of sleep mode, 0 args, w/delay
//     500 ms delay
    delay(500);


// https://arduino.ru/forum/apparatnye-voprosy/st7735s-096-80x160-spi-krivo-rabotaet#comment-619145
//    UTFT_LCD_Write_COM(0xfe); //
//    UTFT_LCD_Write_COM(0xef); // | Переключаем разрешение
//    UTFT_LCD_Write_COM(0xb6);UTFT_LCD_Write_DATA2(0x11); // | экрана 80x160
//    UTFT_LCD_Write_COM(0xac);UTFT_LCD_Write_DATA2(0x0b); ///
//https://arduino.ru/forum/apparatnye-voprosy/st7735s-096-80x160-spi-krivo-rabotaet#comment-619327



#define ST7735_FRMCTR1 0xB1
UTFT_LCD_Write_COM(ST7735_FRMCTR1);//  3: Framerate ctrl - normal mode, 3 arg:
UTFT_LCD_Write_DATA2(0x01);UTFT_LCD_Write_DATA2(0x2C);UTFT_LCD_Write_DATA2(0x2D);//     Rate = fosc/(1x2+40) * (LINE+2C+2D)

#define ST7735_FRMCTR2 0xB2
UTFT_LCD_Write_COM(ST7735_FRMCTR2);//  4: Framerate ctrl - idle mode, 3 args:
UTFT_LCD_Write_DATA2(0x01);UTFT_LCD_Write_DATA2(0x2C);UTFT_LCD_Write_DATA2(0x2D);//     Rate = fosc/(1x2+40) * (LINE+2C+2D)

#define ST7735_FRMCTR3 0xB3
UTFT_LCD_Write_COM(ST7735_FRMCTR3);//  5: Framerate - partial mode, 6 args:
UTFT_LCD_Write_DATA2(0x01);UTFT_LCD_Write_DATA2(0x2C);UTFT_LCD_Write_DATA2(0x2D);//     Dot inversion mode
UTFT_LCD_Write_DATA2(0x01);UTFT_LCD_Write_DATA2(0x2C);UTFT_LCD_Write_DATA2(0x2D);//     Line inversion mode

#define ST7735_INVCTR 0xB4
UTFT_LCD_Write_COM(ST7735_INVCTR); //  6: Display inversion ctrl, 1 arg:
UTFT_LCD_Write_DATA2(0x07); //     No inversion

#define ST7735_PWCTR1 0xC0
UTFT_LCD_Write_COM(ST7735_PWCTR1);//  7: Power control, 3 args, no delay:
UTFT_LCD_Write_DATA2(0xA2);
UTFT_LCD_Write_DATA2(0x02); //     -4.6V
UTFT_LCD_Write_DATA2(0x84); //     AUTO mode

#define ST7735_PWCTR2 0xC1
UTFT_LCD_Write_COM(ST7735_PWCTR2);//  8: Power control, 1 arg, no delay:
UTFT_LCD_Write_DATA2(0xC5); //     VGH25=2.4C VGSEL=-10 VGH=3 * AVDD

#define ST7735_PWCTR3 0xC2
UTFT_LCD_Write_COM(ST7735_PWCTR3); //  9: Power control, 2 args, no delay:
UTFT_LCD_Write_DATA2(0x0A); //     Opamp current small
UTFT_LCD_Write_DATA2(0x00); //     Boost frequency

#define ST7735_PWCTR4 0xC3
UTFT_LCD_Write_COM(ST7735_PWCTR4);// 10: Power control, 2 args, no delay:
UTFT_LCD_Write_DATA2(0x8A);//     BCLK/2,
UTFT_LCD_Write_DATA2(0x2A);//     opamp current small & medium low

#define ST7735_PWCTR5 0xC4
UTFT_LCD_Write_COM(ST7735_PWCTR5);// 11: Power control, 2 args, no delay:
UTFT_LCD_Write_DATA2(0x8A);UTFT_LCD_Write_DATA2(0xEE);

#define ST7735_VMCTR1 0xC5
UTFT_LCD_Write_COM(ST7735_VMCTR1);// 12: Power control, 1 arg, no delay:
UTFT_LCD_Write_DATA2(0x0E);

#define ST77XX_INVOFF 0x20
UTFT_LCD_Write_COM(ST77XX_INVOFF);// 13: Don't invert display, no args

#define ST77XX_MADCTL 0x36
UTFT_LCD_Write_COM(ST77XX_MADCTL);// 14: Mem access ctl (directions), 1 arg:
UTFT_LCD_Write_DATA2(0xC8); //     row/col addr, bottom-top refresh

#define ST77XX_COLMOD 0x3A
UTFT_LCD_Write_COM(ST77XX_COLMOD);// 15: set color mode, 1 arg, no delay:
UTFT_LCD_Write_DATA2(0x05); //     16-bit color


//   options == INITR_MINI160x80

//    _height = ST7735_TFTWIDTH_80;
//    _width = ST7735_TFTHEIGHT_160;

//#define TFT096_COL_SHIFT_PIXEL 24
 // 7735R init, part 2 (mini 160x80)
 //  2 commands in list:
//#define ST77XX_CASET 0x2A
//UTFT_LCD_Write_COM(ST77XX_CASET);//  1: Column addr set, 4 args, no delay:
//UTFT_LCD_Write_DATA2(0x00);UTFT_LCD_Write_DATA2(0x00 + TFT096_COL_SHIFT_PIXEL);//     XSTART = 0
//UTFT_LCD_Write_DATA2(0x00);UTFT_LCD_Write_DATA2(0x4F + TFT096_COL_SHIFT_PIXEL);//     XEND = 79
//#define ST77XX_RASET 0x2B
//UTFT_LCD_Write_COM(ST77XX_RASET);//  2: Row addr set, 4 args, no delay:
//UTFT_LCD_Write_DATA2(0x00);UTFT_LCD_Write_DATA2(0x00);//     XSTART = 0
//UTFT_LCD_Write_DATA2(0x00);UTFT_LCD_Write_DATA2(0x9F);//     XEND = 159


//    _colstart = 24;
//    _rowstart = 0;

// 7735R init, part 3 (red or green tab)
//  4 commands in list:
#define ST7735_GMCTRP1 0xE0
UTFT_LCD_Write_COM(ST7735_GMCTRP1);//  1: Gamma Adjustments (pos. polarity), 16 args + delay:
UTFT_LCD_Write_DATA2(0x02);UTFT_LCD_Write_DATA2(0x1c);UTFT_LCD_Write_DATA2(0x07);UTFT_LCD_Write_DATA2(0x12);//     (Not entirely necessary, but provides
UTFT_LCD_Write_DATA2(0x37);UTFT_LCD_Write_DATA2(0x32);UTFT_LCD_Write_DATA2(0x29);UTFT_LCD_Write_DATA2(0x2d);//      accurate colors)
UTFT_LCD_Write_DATA2(0x29);UTFT_LCD_Write_DATA2(0x25);UTFT_LCD_Write_DATA2(0x2B);UTFT_LCD_Write_DATA2(0x39);
UTFT_LCD_Write_DATA2(0x00);UTFT_LCD_Write_DATA2(0x01);UTFT_LCD_Write_DATA2(0x03);UTFT_LCD_Write_DATA2(0x10);
#define ST7735_GMCTRN1 0xE1
UTFT_LCD_Write_COM(ST7735_GMCTRN1);//  2: Gamma Adjustments (neg. polarity), 16 args + delay:
UTFT_LCD_Write_DATA2(0x03);UTFT_LCD_Write_DATA2(0x1d);UTFT_LCD_Write_DATA2(0x07);UTFT_LCD_Write_DATA2(0x06);//     (Not entirely necessary, but provides
UTFT_LCD_Write_DATA2(0x2E);UTFT_LCD_Write_DATA2(0x2C);UTFT_LCD_Write_DATA2(0x29);UTFT_LCD_Write_DATA2(0x2D);//      accurate colors)
UTFT_LCD_Write_DATA2(0x2E);UTFT_LCD_Write_DATA2(0x2E);UTFT_LCD_Write_DATA2(0x37);UTFT_LCD_Write_DATA2(0x3F);
UTFT_LCD_Write_DATA2(0x00);UTFT_LCD_Write_DATA2(0x00);UTFT_LCD_Write_DATA2(0x02);UTFT_LCD_Write_DATA2(0x10);

#define ST77XX_NORON 0x13
UTFT_LCD_Write_COM(ST77XX_NORON);//  3: Normal display on, no args, w/delay
    delay(10);                           //     10 ms delay

#define ST77XX_DISPON 0x29
UTFT_LCD_Write_COM(ST77XX_DISPON);//  4: Main screen turn on, no args w/delay
    delay(100);                        //     100 ms delay


UTFT_LCD_Write_COM(ST77XX_MADCTL);
UTFT_LCD_Write_DATA2(0xC0);


//    tabcolor = options;
#define ST77XX_MADCTL_MY 0x80
#define ST77XX_MADCTL_MX 0x40
#define ST77XX_MADCTL_MV 0x20
#define ST77XX_MADCTL_ML 0x10
#define ST77XX_MADCTL_RGB 0x00
#define ST7735_MADCTL_BGR 0x08
    UTFT_LCD_Write_COM(ST77XX_MADCTL);
    UTFT_LCD_Write_DATA2(ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST77XX_MADCTL_RGB); // rotation(0)
//    UTFT_LCD_Write_DATA2(ST77XX_MADCTL_MY | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB); // rotation(1)
//    UTFT_LCD_Write_DATA2(ST77XX_MADCTL_RGB); // rotation(2)
//    UTFT_LCD_Write_DATA2(ST77XX_MADCTL_MX | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB); // rotation(3)



	break;
