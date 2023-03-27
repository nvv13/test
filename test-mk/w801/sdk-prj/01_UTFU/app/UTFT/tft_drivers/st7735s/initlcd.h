case ST7735S:
case TFT00_96SP:

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
case TFT00_96SP+1:
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
	break;

