case PCF8833:
	UTFT_LCD_Write_COM(0x2a); 
  	UTFT_LCD_Write_DATA2(x1);
  	UTFT_LCD_Write_DATA2(x2);
	UTFT_LCD_Write_COM(0x2b); 
  	UTFT_LCD_Write_DATA2(y1);
  	UTFT_LCD_Write_DATA2(y2);
	UTFT_LCD_Write_COM(0x2c); 
	break;
