case MSP3526:
case TFT02_0V89:


//	#define lcddev_setxcmd 0x2A
//	#define lcddev_setycmd 0x2B
//	#define lcddev_wramcmd 0x2C
//	#define lcddev_rramcmd 0x2E

//	UTFT_LCD_Write_COM(lcddev_setxcmd);LCD_WR_DATA(x1>>8);  
//	UTFT_LCD_Write_COM(lcddev_setxcmd+1);LCD_WR_DATA(x1&0XFF);	  
//	UTFT_LCD_Write_COM(lcddev_setxcmd+2);LCD_WR_DATA(x2>>8);   
//	UTFT_LCD_Write_COM(lcddev_setxcmd+3);LCD_WR_DATA(x2&0XFF);   
//	UTFT_LCD_Write_COM(lcddev_setycmd);LCD_WR_DATA(y1>>8);   
//	UTFT_LCD_Write_COM(lcddev_setycmd+1);LCD_WR_DATA(y1&0XFF);  
//	UTFT_LCD_Write_COM(lcddev_setycmd+2);LCD_WR_DATA(y2>>8);   
//	UTFT_LCD_Write_COM(lcddev_setycmd+3);LCD_WR_DATA(y2&0XFF); 

//	UTFT_LCD_Write_COM(lcddev_wramcmd);

#define MSP3526_COL_SHIFT_PIXEL 0
#define MSP3526_ROW_SHIFT_PIXEL 0 //160

#define ST77XX_CASET 0x2A
	UTFT_LCD_Write_COM(ST77XX_CASET); //Set Column Address
  	UTFT_LCD_Write_DATA2((x1 + MSP3526_COL_SHIFT_PIXEL)>>8);
  	UTFT_LCD_Write_DATA2((x1 + MSP3526_COL_SHIFT_PIXEL) & 0xFF);
  	UTFT_LCD_Write_DATA2((x2 + MSP3526_COL_SHIFT_PIXEL)>>8);
  	UTFT_LCD_Write_DATA2((x2 + MSP3526_COL_SHIFT_PIXEL) & 0xFF);
#define ST77XX_RASET 0x2B
	UTFT_LCD_Write_COM(ST77XX_RASET);  //Set Page Address
  	UTFT_LCD_Write_DATA2((y1 + MSP3526_ROW_SHIFT_PIXEL)>>8);
  	UTFT_LCD_Write_DATA2((y1 + MSP3526_ROW_SHIFT_PIXEL) & 0xFF);
  	UTFT_LCD_Write_DATA2((y2 + MSP3526_ROW_SHIFT_PIXEL)>>8);
  	UTFT_LCD_Write_DATA2((y2 + MSP3526_ROW_SHIFT_PIXEL) & 0xFF);
#define ST77XX_RAMWR 0x2C
	UTFT_LCD_Write_COM(ST77XX_RAMWR); 

	break;	