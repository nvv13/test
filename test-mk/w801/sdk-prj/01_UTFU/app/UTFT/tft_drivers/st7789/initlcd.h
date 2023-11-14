case TFT01_3SP:
case TFT01_47V89:
case TFT01_14V89:
case TFT01_69V89:
//case TFT02_0V89:
   //LOG("TFT01_3SP init 9 commands in list\n"); 
         // Init commands for 7789 screens
         //  9 commands in list:
#define ST77XX_SWRESET 0x01
    UTFT_LCD_Write_COM(ST77XX_SWRESET);
 //  1: Software reset, 0 args, w/delay
 //     150 ms delay
    delay(150);

#define ST77XX_SLPOUT 0x11
    UTFT_LCD_Write_COM(ST77XX_SLPOUT);
 //  2: Out of sleep mode, 0 args, w/delay
//     10 ms delay
    delay(500);

    UTFT_LCD_Write_COM(ST77XX_COLMOD);//  3: Set color mode, 1 arg + delay:
    UTFT_LCD_Write_DATA2(0x55);      //     16-bit color
    delay(10);                      //     10 ms delay

    UTFT_LCD_Write_COM(ST77XX_MADCTL);              //  4: Mem access ctrl (directions), 1 arg:
    UTFT_LCD_Write_DATA2(0x00);                       //     Row/col addr, bottom-top refresh

/*
#define ST77XX_CASET 0x2A
    UTFT_LCD_Write_COM(ST77XX_CASET);   //  5: Column addr set, 4 args, no delay:
    UTFT_LCD_Write_DATA2(0x00);
    UTFT_LCD_Write_DATA2(0);        //     XSTART = 0
    UTFT_LCD_Write_DATA2(0);
   UTFT_LCD_Write_DATA2(240);  //     XEND = 240

#define ST77XX_RASET 0x2B
    UTFT_LCD_Write_COM(ST77XX_RASET);           //  6: Row addr set, 4 args, no delay:
   UTFT_LCD_Write_DATA2(0x00);
   UTFT_LCD_Write_DATA2(0);             //     YSTART = 0
   UTFT_LCD_Write_DATA2(240>>8);
   UTFT_LCD_Write_DATA2(240&0xFF);  //     YEND = 240
*/

    UTFT_LCD_Write_COM(ST77XX_INVON);//  7: hack
    delay(10);
    UTFT_LCD_Write_COM(ST77XX_NORON);//  8: Normal display on, no args, w/delay
    delay(10);//     10 ms delay
    UTFT_LCD_Write_COM(ST77XX_DISPON);//  9: Main screen turn on, no args, delay
    delay(500); //    10 ms delay

    UTFT_LCD_Write_COM(ST77XX_MADCTL);
#define ST77XX_MADCTL_MY 0x80
#define ST77XX_MADCTL_MX 0x40
#define ST77XX_MADCTL_MV 0x20
#define ST77XX_MADCTL_ML 0x10
#define ST7789_MADCTL_RGB 0x00
  switch(display_model)
   {
   case TFT01_3SP: UTFT_LCD_Write_DATA2(ST7789_MADCTL_RGB);break; // rotation(2)
   case TFT01_47V89: 
   case TFT01_14V89: 
   case TFT01_69V89:
   case TFT02_0V89:
    UTFT_LCD_Write_DATA2(ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST7789_MADCTL_RGB);break; // rotation(0)  case TFT01_47V89
   default:
   UTFT_LCD_Write_DATA2(ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST7735_MADCTL_BGR); // rotation(0)  case TFT01_14V89:
   }


//    UTFT_LCD_Write_DATA2(ST7789_MADCTL_RGB); // rotation(2)

//    UTFT_LCD_Write_DATA2(ST77XX_MADCTL_MY | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB); // rotation(1)
//    UTFT_LCD_Write_DATA2(ST77XX_MADCTL_RGB); // rotation(2)
//    UTFT_LCD_Write_DATA2(ST77XX_MADCTL_MX | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB); // rotation(3)
//UTFT_LCD_Write_DATA2(0x70);


	break;


