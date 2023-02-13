case ILI9341_UNO:
	UTFT_LCD_Write_COM(0x2A); //column
	UTFT_LCD_Write_DATA2(x1>>8);
	UTFT_LCD_Write_DATA2(x1);
	UTFT_LCD_Write_DATA2(x2>>8);
	UTFT_LCD_Write_DATA2(x2);
	UTFT_LCD_Write_COM(0x2B); //page
	UTFT_LCD_Write_DATA2(y1>>8);
	UTFT_LCD_Write_DATA2(y1);
	UTFT_LCD_Write_DATA2(y2>>8);
	UTFT_LCD_Write_DATA2(y2);
	UTFT_LCD_Write_COM(0x2C); //write
	break;	