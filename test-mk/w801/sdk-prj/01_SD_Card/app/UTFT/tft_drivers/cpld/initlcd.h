case CPLD:
	if (orient==LANDSCAPE)
		orient = PORTRAIT;
	else
		orient = LANDSCAPE;

	UTFT_LCD_Write_COM(0x0F);   
	UTFT_LCD_Write_COM_DATA(0x01,0x0010);
	UTFT_LCD_Write_COM(0x0F);   
	break;
