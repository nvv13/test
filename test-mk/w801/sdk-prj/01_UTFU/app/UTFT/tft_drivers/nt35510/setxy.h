case TFT_397T_NT35510:

//void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
//{	

	#define lcddev_setxcmd 0x2A00
	#define lcddev_setycmd 0x2B00
	#define lcddev_wramcmd 0x2C00
	#define lcddev_rramcmd 0x2E00


	LCD_WR_REG(lcddev_setxcmd);LCD_WR_DATA(x1>>8);  
	LCD_WR_REG(lcddev_setxcmd+1);LCD_WR_DATA(x1&0XFF);	  
	LCD_WR_REG(lcddev_setxcmd+2);LCD_WR_DATA(x2>>8);   
	LCD_WR_REG(lcddev_setxcmd+3);LCD_WR_DATA(x2&0XFF);   
	LCD_WR_REG(lcddev_setycmd);LCD_WR_DATA(y1>>8);   
	LCD_WR_REG(lcddev_setycmd+1);LCD_WR_DATA(y1&0XFF);  
	LCD_WR_REG(lcddev_setycmd+2);LCD_WR_DATA(y2>>8);   
	LCD_WR_REG(lcddev_setycmd+3);LCD_WR_DATA(y2&0XFF); 

	//LCD_WriteRAM_Prepare();	//¿ªÊ¼Ð´ÈëGRAM			
	LCD_WR_REG(lcddev_wramcmd);

//}   


	break;	