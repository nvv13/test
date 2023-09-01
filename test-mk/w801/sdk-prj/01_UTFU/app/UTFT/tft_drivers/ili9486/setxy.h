case ILI9486:
case TFT3_5SP_9488:
	UTFT_LCD_Write_COM(0x2a); 
	UTFT_LCD_Write_DATA2(x1>>8);
	UTFT_LCD_Write_DATA2(x1);
	UTFT_LCD_Write_DATA2(x2>>8);
	UTFT_LCD_Write_DATA2(x2);
	UTFT_LCD_Write_COM(0x2b); 
	UTFT_LCD_Write_DATA2(y1>>8);
	UTFT_LCD_Write_DATA2(y1);
	UTFT_LCD_Write_DATA2(y2>>8);
	UTFT_LCD_Write_DATA2(y2);
	UTFT_LCD_Write_COM(0x2c); 
	break;

case MHS3528:

      if(_SPI_16bit)
        {// SPI 16bit
	LCD_WR_REG(0x2a); 
	UTFT_LCD_Write_DATA (0x00 ,x1>>8);
	UTFT_LCD_Write_DATA (0x00 ,x1);
	UTFT_LCD_Write_DATA (0x00 ,x2>>8);
	UTFT_LCD_Write_DATA (0x00 ,x2);
	LCD_WR_REG(0x2b); 
	UTFT_LCD_Write_DATA (0x00 ,y1>>8);
	UTFT_LCD_Write_DATA (0x00 ,y1);
	UTFT_LCD_Write_DATA (0x00 ,y2>>8);
	UTFT_LCD_Write_DATA (0x00 ,y2);
	LCD_WR_REG(0x2c); 
        }
        else
        {
	UTFT_LCD_Write_COM(0x2a); 
	UTFT_LCD_Write_DATA2(x1>>8);
	UTFT_LCD_Write_DATA2(x1);
	UTFT_LCD_Write_DATA2(x2>>8);
	UTFT_LCD_Write_DATA2(x2);
	UTFT_LCD_Write_COM(0x2b); 
	UTFT_LCD_Write_DATA2(y1>>8);
	UTFT_LCD_Write_DATA2(y1);
	UTFT_LCD_Write_DATA2(y2>>8);
	UTFT_LCD_Write_DATA2(y2);
	UTFT_LCD_Write_COM(0x2c); 
        }
	break;

