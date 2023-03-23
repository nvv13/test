case ST7735S:
case TFT00_96SP:
	UTFT_LCD_Write_COM(0x2a); //Set Column Address
  	UTFT_LCD_Write_DATA2(x1>>8);
  	UTFT_LCD_Write_DATA2(x1 & 0xFF);
  	UTFT_LCD_Write_DATA2(x2>>8);
  	UTFT_LCD_Write_DATA2(x2 & 0xFF);
	UTFT_LCD_Write_COM(0x2b);  //Set Page Address
  	UTFT_LCD_Write_DATA2(y1>>8);
  	UTFT_LCD_Write_DATA2(y1 & 0xFF);
  	UTFT_LCD_Write_DATA2(y2>>8);
  	UTFT_LCD_Write_DATA2(y2 & 0xFF);
	UTFT_LCD_Write_COM(0x2c); 
	break;
