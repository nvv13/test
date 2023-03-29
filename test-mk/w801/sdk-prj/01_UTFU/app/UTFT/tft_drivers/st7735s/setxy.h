case ST7735S:
#define ST77XX_CASET 0x2A
	UTFT_LCD_Write_COM(ST77XX_CASET); //Set Column Address
  	UTFT_LCD_Write_DATA2(x1>>8);
  	UTFT_LCD_Write_DATA2(x1 & 0xFF);
  	UTFT_LCD_Write_DATA2(x2>>8);
  	UTFT_LCD_Write_DATA2(x2 & 0xFF);
#define ST77XX_RASET 0x2B
	UTFT_LCD_Write_COM(ST77XX_RASET);  //Set Page Address
  	UTFT_LCD_Write_DATA2(y1>>8);
  	UTFT_LCD_Write_DATA2(y1 & 0xFF);
  	UTFT_LCD_Write_DATA2(y2>>8);
  	UTFT_LCD_Write_DATA2(y2 & 0xFF);
#define ST77XX_RAMWR 0x2C
	UTFT_LCD_Write_COM(ST77XX_RAMWR); 
	break;
case TFT00_96SP:
 // Смещение матрицы относительно строк/столбцов контроллера
#define TFT096_COL_SHIFT_PIXEL 26
#define TFT096_ROW_SHIFT_PIXEL 1
#define ST77XX_CASET 0x2A
	UTFT_LCD_Write_COM(ST77XX_CASET); //Set Column Address
  	UTFT_LCD_Write_DATA2((x1 + TFT096_COL_SHIFT_PIXEL)>>8);
  	UTFT_LCD_Write_DATA2((x1 + TFT096_COL_SHIFT_PIXEL) & 0xFF);
  	UTFT_LCD_Write_DATA2((x2 + TFT096_COL_SHIFT_PIXEL)>>8);
  	UTFT_LCD_Write_DATA2((x2 + TFT096_COL_SHIFT_PIXEL) & 0xFF);
#define ST77XX_RASET 0x2B
	UTFT_LCD_Write_COM(ST77XX_RASET);  //Set Page Address
  	UTFT_LCD_Write_DATA2((y1 + TFT096_ROW_SHIFT_PIXEL)>>8);
  	UTFT_LCD_Write_DATA2((y1 + TFT096_ROW_SHIFT_PIXEL) & 0xFF);
  	UTFT_LCD_Write_DATA2((y2 + TFT096_ROW_SHIFT_PIXEL)>>8);
  	UTFT_LCD_Write_DATA2((y2 + TFT096_ROW_SHIFT_PIXEL) & 0xFF);
#define ST77XX_RAMWR 0x2C
	UTFT_LCD_Write_COM(ST77XX_RAMWR); 
	break;


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