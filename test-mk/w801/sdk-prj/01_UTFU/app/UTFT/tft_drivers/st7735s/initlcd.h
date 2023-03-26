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
case TFT00_96SP:


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
//	UTFT_LCD_Write_DATA2(0xC0); 
	UTFT_LCD_Write_DATA2(0xC8); 

#define ST77xx_MADCTL_MY        0x80
#define ST77xx_MADCTL_MX        0x40
#define ST77xx_MADCTL_MV        0x20
#define ST77xx_MADCTL_ML        0x10

#define ST77xx_MADCTL_BGR       0x08
#define ST77xx_MADCTL_MH        0x04

#define ST77XX_MADCTL_RGB       0x00

//	UTFT_LCD_Write_DATA2(ST77xx_MADCTL_MX | ST77xx_MADCTL_BGR); 
//UTFT_LCD_Write_DATA2(0xC8); 
//UTFT_LCD_Write_DATA2(0x08); 

/*
	lcd_write_cmd(conf,0x36);
    if(conf->direction==0)lcd_write_data(conf,0x08); Rotation?
    else if(conf->direction==1)lcd_write_data(conf,0xC8);
    else if(conf->direction==2)lcd_write_data(conf,0x78);
    else lcd_write_data(conf,0xA8);
*/

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


/*
	lcd_write_cmd(conf,0xFC);
	lcd_write_data(conf,0x80);
*/
//	UTFT_LCD_Write_COM(0xFC); //?
//	UTFT_LCD_Write_DATA2(0x80);

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

	UTFT_LCD_Write_COM(0x3A); //65k mode ST77xx_Cmd_COLMOD
#define ST7735_ColorMode_12bit  0x03
#define ST7735_ColorMode_16bit  0x05
#define ST7735_ColorMode_18bit  0x06
	UTFT_LCD_Write_DATA2(ST7735_ColorMode_16bit); //       ST7735_ColorMode_16bit   
//--------------------------------------orent disp grad-----------------------------// 
//	UTFT_LCD_Write_COM(0x36); //-- orent disp grad
//	UTFT_LCD_Write_DATA2(0x08);//-- direction=0  
//                0x000300C8, -- direction=1 90
//                0x00030078, -- direction=2 180 
//                0x000300A8, -- direction=3 270
        delay(500);


	UTFT_LCD_Write_COM(0x29);//Display on
        delay(100);
	break;

