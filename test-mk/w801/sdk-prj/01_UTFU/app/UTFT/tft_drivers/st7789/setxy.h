case TFT01_3SP:
 // LOG("TFT01_3SP set CASET RASET\n"); 
 // Смещение матрицы относительно строк/столбцов контроллера
#define TFT130_COL_SHIFT_PIXEL 0
#define TFT130_ROW_SHIFT_PIXEL 0
#define ST77XX_CASET 0x2A
	UTFT_LCD_Write_COM(ST77XX_CASET); //Set Column Address
  	UTFT_LCD_Write_DATA2((x1 + TFT130_COL_SHIFT_PIXEL)>>8);
  	UTFT_LCD_Write_DATA2((x1 + TFT130_COL_SHIFT_PIXEL) & 0xFF);
  	UTFT_LCD_Write_DATA2((x2 + TFT130_COL_SHIFT_PIXEL)>>8);
  	UTFT_LCD_Write_DATA2((x2 + TFT130_COL_SHIFT_PIXEL) & 0xFF);
#define ST77XX_RASET 0x2B
	UTFT_LCD_Write_COM(ST77XX_RASET);  //Set Page Address
  	UTFT_LCD_Write_DATA2((y1 + TFT130_ROW_SHIFT_PIXEL)>>8);
  	UTFT_LCD_Write_DATA2((y1 + TFT130_ROW_SHIFT_PIXEL) & 0xFF);
  	UTFT_LCD_Write_DATA2((y2 + TFT130_ROW_SHIFT_PIXEL)>>8);
  	UTFT_LCD_Write_DATA2((y2 + TFT130_ROW_SHIFT_PIXEL) & 0xFF);
#define ST77XX_RAMWR 0x2C
	UTFT_LCD_Write_COM(ST77XX_RAMWR); 
	break;

case TFT01_47V89:
#define TFT1_47V89_COL_SHIFT_PIXEL 34
#define TFT1_47V89_ROW_SHIFT_PIXEL 0
#define ST77XX_CASET 0x2A
	UTFT_LCD_Write_COM(ST77XX_CASET); //Set Column Address
  	UTFT_LCD_Write_DATA2((x1 + TFT1_47V89_COL_SHIFT_PIXEL)>>8);
  	UTFT_LCD_Write_DATA2((x1 + TFT1_47V89_COL_SHIFT_PIXEL) & 0xFF);
  	UTFT_LCD_Write_DATA2((x2 + TFT1_47V89_COL_SHIFT_PIXEL)>>8);
  	UTFT_LCD_Write_DATA2((x2 + TFT1_47V89_COL_SHIFT_PIXEL) & 0xFF);
#define ST77XX_RASET 0x2B
	UTFT_LCD_Write_COM(ST77XX_RASET);  //Set Page Address
  	UTFT_LCD_Write_DATA2((y1 + TFT1_47V89_ROW_SHIFT_PIXEL)>>8);
  	UTFT_LCD_Write_DATA2((y1 + TFT1_47V89_ROW_SHIFT_PIXEL) & 0xFF);
  	UTFT_LCD_Write_DATA2((y2 + TFT1_47V89_ROW_SHIFT_PIXEL)>>8);
  	UTFT_LCD_Write_DATA2((y2 + TFT1_47V89_ROW_SHIFT_PIXEL) & 0xFF);
#define ST77XX_RAMWR 0x2C
	UTFT_LCD_Write_COM(ST77XX_RAMWR); 
	break;


case TFT01_14V89:
#define TFT1_14V89_COL_SHIFT_PIXEL 53
#define TFT1_14V89_ROW_SHIFT_PIXEL 40
#define ST77XX_CASET 0x2A
	UTFT_LCD_Write_COM(ST77XX_CASET); //Set Column Address
  	UTFT_LCD_Write_DATA2((x1 + TFT1_14V89_COL_SHIFT_PIXEL)>>8);
  	UTFT_LCD_Write_DATA2((x1 + TFT1_14V89_COL_SHIFT_PIXEL) & 0xFF);
  	UTFT_LCD_Write_DATA2((x2 + TFT1_14V89_COL_SHIFT_PIXEL)>>8);
  	UTFT_LCD_Write_DATA2((x2 + TFT1_14V89_COL_SHIFT_PIXEL) & 0xFF);
#define ST77XX_RASET 0x2B
	UTFT_LCD_Write_COM(ST77XX_RASET);  //Set Page Address
  	UTFT_LCD_Write_DATA2((y1 + TFT1_14V89_ROW_SHIFT_PIXEL)>>8);
  	UTFT_LCD_Write_DATA2((y1 + TFT1_14V89_ROW_SHIFT_PIXEL) & 0xFF);
  	UTFT_LCD_Write_DATA2((y2 + TFT1_14V89_ROW_SHIFT_PIXEL)>>8);
  	UTFT_LCD_Write_DATA2((y2 + TFT1_14V89_ROW_SHIFT_PIXEL) & 0xFF);
#define ST77XX_RAMWR 0x2C
	UTFT_LCD_Write_COM(ST77XX_RAMWR); 
	break;


case TFT01_69V89:
#define TFT1_69V89_COL_SHIFT_PIXEL 0
#define TFT1_69V89_ROW_SHIFT_PIXEL 20
#define ST77XX_CASET 0x2A
	UTFT_LCD_Write_COM(ST77XX_CASET); //Set Column Address
  	UTFT_LCD_Write_DATA2((x1 + TFT1_69V89_COL_SHIFT_PIXEL)>>8);
  	UTFT_LCD_Write_DATA2((x1 + TFT1_69V89_COL_SHIFT_PIXEL) & 0xFF);
  	UTFT_LCD_Write_DATA2((x2 + TFT1_69V89_COL_SHIFT_PIXEL)>>8);
  	UTFT_LCD_Write_DATA2((x2 + TFT1_69V89_COL_SHIFT_PIXEL) & 0xFF);
#define ST77XX_RASET 0x2B
	UTFT_LCD_Write_COM(ST77XX_RASET);  //Set Page Address
  	UTFT_LCD_Write_DATA2((y1 + TFT1_69V89_ROW_SHIFT_PIXEL)>>8);
  	UTFT_LCD_Write_DATA2((y1 + TFT1_69V89_ROW_SHIFT_PIXEL) & 0xFF);
  	UTFT_LCD_Write_DATA2((y2 + TFT1_69V89_ROW_SHIFT_PIXEL)>>8);
  	UTFT_LCD_Write_DATA2((y2 + TFT1_69V89_ROW_SHIFT_PIXEL) & 0xFF);
#define ST77XX_RAMWR 0x2C
	UTFT_LCD_Write_COM(ST77XX_RAMWR); 
	break;