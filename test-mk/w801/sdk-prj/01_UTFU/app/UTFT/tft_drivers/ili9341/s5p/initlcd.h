case ILI9341_S5P:
case TFT2_4SP_9341:
case TFT3_2SP_9341:

    UTFT_LCD_Write_COM(0xCB);  
    UTFT_LCD_Write_DATA2(0x39); 
    UTFT_LCD_Write_DATA2(0x2C); 
    UTFT_LCD_Write_DATA2(0x00); 
    UTFT_LCD_Write_DATA2(0x34); 
    UTFT_LCD_Write_DATA2(0x02); 

    UTFT_LCD_Write_COM(0xCF);  
    UTFT_LCD_Write_DATA2(0x00); 
    UTFT_LCD_Write_DATA2(0XC1); 
    UTFT_LCD_Write_DATA2(0X30); 

    UTFT_LCD_Write_COM(0xE8);  
    UTFT_LCD_Write_DATA2(0x85); 
    UTFT_LCD_Write_DATA2(0x00); 
    UTFT_LCD_Write_DATA2(0x78); 

    UTFT_LCD_Write_COM(0xEA);  
    UTFT_LCD_Write_DATA2(0x00); 
    UTFT_LCD_Write_DATA2(0x00); 
 
    UTFT_LCD_Write_COM(0xED);  
    UTFT_LCD_Write_DATA2(0x64); 
    UTFT_LCD_Write_DATA2(0x03); 
    UTFT_LCD_Write_DATA2(0X12); 
    UTFT_LCD_Write_DATA2(0X81); 

    UTFT_LCD_Write_COM(0xF7);  
    UTFT_LCD_Write_DATA2(0x20); 
  
    UTFT_LCD_Write_COM(0xC0);    //Power control 
    UTFT_LCD_Write_DATA2(0x23);   //VRH[5:0] 
 
    UTFT_LCD_Write_COM(0xC1);    //Power control 
    UTFT_LCD_Write_DATA2(0x10);   //SAP[2:0];BT[3:0] 

    UTFT_LCD_Write_COM(0xC5);    //VCM control 
    UTFT_LCD_Write_DATA2(0x3e);   //Contrast
    UTFT_LCD_Write_DATA2(0x28); 
 
    UTFT_LCD_Write_COM(0xC7);    //VCM control2 
    UTFT_LCD_Write_DATA2(0x86);   //--
 
    UTFT_LCD_Write_COM(0x36);    // Memory Access Control 
//    UTFT_LCD_Write_DATA2(0x48);   

#define ST77XX_MADCTL_MY 0x80
#define ST77XX_MADCTL_MX 0x40
#define ST77XX_MADCTL_MV 0x20
#define ST77XX_MADCTL_ML 0x10
#define ST7789_MADCTL_RGB 0x00
#define ST7735_MADCTL_BGR 0x08
  switch(display_model)
   {
   case TFT2_4SP_9341: //UTFT_LCD_Write_DATA2(ST7789_MADCTL_RGB);break; // rotation(2)
  //    UTFT_LCD_Write_DATA2(ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST7735_MADCTL_BGR);break; // rotation(0)
  //  UTFT_LCD_Write_DATA2(ST77XX_MADCTL_MY | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB);break; // rotation(1)
   // UTFT_LCD_Write_DATA2(ST77XX_MADCTL_MX | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB);break; // rotation(3)
//    UTFT_LCD_Write_DATA2(ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST7789_MADCTL_RGB);break; // rotation(0)  c?ase TFT01_47V89
   UTFT_LCD_Write_DATA2(0x20);break;   
   case TFT3_2SP_9341:
   //UTFT_LCD_Write_DATA2(ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST7735_MADCTL_BGR);break; // rotation(0)
   //UTFT_LCD_Write_DATA2(ST77XX_MADCTL_MY | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB);break; // rotation(1)
   UTFT_LCD_Write_DATA2(ST77XX_MADCTL_MX | ST7735_MADCTL_BGR);break; // rotation(2)
   // UTFT_LCD_Write_DATA2(ST77XX_MADCTL_MX | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB);break; // rotation(3)
   default:
   UTFT_LCD_Write_DATA2(0x48);   
   }

//#define ST77XX_MADCTL_RGB 0x00
//#define ST7735_MADCTL_BGR 0x08
//    UTFT_LCD_Write_COM(ST77XX_MADCTL);
//    UTFT_LCD_Write_DATA2(ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST7735_MADCTL_BGR); // rotation(0)
//    UTFT_LCD_Write_DATA2(ST77XX_MADCTL_MY | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB); // rotation(1)
//    UTFT_LCD_Write_DATA2(ST77XX_MADCTL_RGB); // rotation(2)
//    UTFT_LCD_Write_DATA2(ST77XX_MADCTL_MX | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB); // rotation(3)


//    UTFT_LCD_Write_DATA2(ST7789_MADCTL_RGB); // rotation(2)

//    UTFT_LCD_Write_DATA2(ST77XX_MADCTL_MY | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB); // rotation(1)
//    UTFT_LCD_Write_DATA2(ST77XX_MADCTL_RGB); // rotation(2)
//    UTFT_LCD_Write_DATA2(ST77XX_MADCTL_MX | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB); // rotation(3)


    UTFT_LCD_Write_COM(0x3A);    
    UTFT_LCD_Write_DATA2(0x55); 

    UTFT_LCD_Write_COM(0xB1);    
    UTFT_LCD_Write_DATA2(0x00);  
    UTFT_LCD_Write_DATA2(0x18); 
 
    UTFT_LCD_Write_COM(0xB6);    // Display Function Control 
    UTFT_LCD_Write_DATA2(0x08); 
    UTFT_LCD_Write_DATA2(0x82);
    UTFT_LCD_Write_DATA2(0x27);  

    UTFT_LCD_Write_COM(0x11);    //Exit Sleep 
    delay(120); 
				
    UTFT_LCD_Write_COM(0x29);    //Display on 
    UTFT_LCD_Write_COM(0x2c); 
	break;


case MSP2834:
case MSP3223:
	//LCD_RESET(); 
//*************2.8 ILI9341 IPS initialization**********//	
//**************3.2 ILI9341 IPS initialization************//
	UTFT_LCD_Write_COM(0xCF);  
	UTFT_LCD_Write_DATA2(0x00); 
	UTFT_LCD_Write_DATA2(0xC1); 
	UTFT_LCD_Write_DATA2(0x30); 
 
	UTFT_LCD_Write_COM(0xED);  
	UTFT_LCD_Write_DATA2(0x64); 
	UTFT_LCD_Write_DATA2(0x03); 
	UTFT_LCD_Write_DATA2(0X12); 
	UTFT_LCD_Write_DATA2(0X81); 
 
	UTFT_LCD_Write_COM(0xE8);  
	UTFT_LCD_Write_DATA2(0x85); 
	UTFT_LCD_Write_DATA2(0x00); 
	UTFT_LCD_Write_DATA2(0x78); 

	UTFT_LCD_Write_COM(0xCB);  
	UTFT_LCD_Write_DATA2(0x39); 
	UTFT_LCD_Write_DATA2(0x2C); 
	UTFT_LCD_Write_DATA2(0x00); 
	UTFT_LCD_Write_DATA2(0x34); 
	UTFT_LCD_Write_DATA2(0x02); 
	
	UTFT_LCD_Write_COM(0xF7);  
	UTFT_LCD_Write_DATA2(0x20); 
 
	UTFT_LCD_Write_COM(0xEA);  
	UTFT_LCD_Write_DATA2(0x00); 
	UTFT_LCD_Write_DATA2(0x00); 

	UTFT_LCD_Write_COM(0xC0);       //Power control 
	UTFT_LCD_Write_DATA2(0x13);     //VRH[5:0] 
 
	UTFT_LCD_Write_COM(0xC1);       //Power control 
	UTFT_LCD_Write_DATA2(0x13);     //SAP[2:0];BT[3:0] 
 
	UTFT_LCD_Write_COM(0xC5);       //VCM control 
	if(display_model==MSP3223)
          UTFT_LCD_Write_DATA2(0x1C);   //22
          else
          UTFT_LCD_Write_DATA2(0x22);   //22
	UTFT_LCD_Write_DATA2(0x35);   //35
 
	UTFT_LCD_Write_COM(0xC7);       //VCM control2 
        if(display_model==MSP3223)
          UTFT_LCD_Write_DATA2(0xC8);  //22
          else
   	  UTFT_LCD_Write_DATA2(0xBD);  //AF

	UTFT_LCD_Write_COM(0x21);

	UTFT_LCD_Write_COM(0x36);       // Memory Access Control 
	UTFT_LCD_Write_DATA2(0x08); 

	UTFT_LCD_Write_COM(0xB6);  
	UTFT_LCD_Write_DATA2(0x0A); 
	UTFT_LCD_Write_DATA2(0xA2); 

	UTFT_LCD_Write_COM(0x3A);       
	UTFT_LCD_Write_DATA2(0x55); 

	UTFT_LCD_Write_COM(0xF6);  //Interface Control
	UTFT_LCD_Write_DATA2(0x01); 
	UTFT_LCD_Write_DATA2(0x30);  //MCU

	UTFT_LCD_Write_COM(0xB1);       //VCM control 
	UTFT_LCD_Write_DATA2(0x00); 
	UTFT_LCD_Write_DATA2(0x1B); 
 
	UTFT_LCD_Write_COM(0xF2);       // 3Gamma Function Disable 
	UTFT_LCD_Write_DATA2(0x00); 
 
	UTFT_LCD_Write_COM(0x26);       //Gamma curve selected 
	UTFT_LCD_Write_DATA2(0x01); 
 
	UTFT_LCD_Write_COM(0xE0);       //Set Gamma 
	UTFT_LCD_Write_DATA2(0x0F); 
	UTFT_LCD_Write_DATA2(0x35); 
	UTFT_LCD_Write_DATA2(0x31); 
	UTFT_LCD_Write_DATA2(0x0B); 
	UTFT_LCD_Write_DATA2(0x0E); 
	UTFT_LCD_Write_DATA2(0x06); 
	UTFT_LCD_Write_DATA2(0x49); 
	UTFT_LCD_Write_DATA2(0xA7); 
	UTFT_LCD_Write_DATA2(0x33); 
	UTFT_LCD_Write_DATA2(0x07); 
	UTFT_LCD_Write_DATA2(0x0F); 
	UTFT_LCD_Write_DATA2(0x03); 
	UTFT_LCD_Write_DATA2(0x0C); 
	UTFT_LCD_Write_DATA2(0x0A); 
	UTFT_LCD_Write_DATA2(0x00); 
 
	UTFT_LCD_Write_COM(0XE1);       //Set Gamma 
	UTFT_LCD_Write_DATA2(0x00); 
	UTFT_LCD_Write_DATA2(0x0A); 
	UTFT_LCD_Write_DATA2(0x0F); 
	UTFT_LCD_Write_DATA2(0x04); 
	UTFT_LCD_Write_DATA2(0x11); 
	UTFT_LCD_Write_DATA2(0x08); 
	UTFT_LCD_Write_DATA2(0x36); 
	UTFT_LCD_Write_DATA2(0x58); 
	UTFT_LCD_Write_DATA2(0x4D); 
	UTFT_LCD_Write_DATA2(0x07); 
	UTFT_LCD_Write_DATA2(0x10); 
	UTFT_LCD_Write_DATA2(0x0C); 
	UTFT_LCD_Write_DATA2(0x32); 
	UTFT_LCD_Write_DATA2(0x34); 
	UTFT_LCD_Write_DATA2(0x0F); 

	UTFT_LCD_Write_COM(0x11);       //Exit Sleep 
	delay(120); 
	UTFT_LCD_Write_COM(0x29);       //Display on 
        //LCD_direction(USE_HORIZONTAL);//
	break;

