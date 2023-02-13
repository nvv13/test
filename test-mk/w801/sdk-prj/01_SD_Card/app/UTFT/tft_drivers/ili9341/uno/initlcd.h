case ILI9341_UNO:

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
    UTFT_LCD_Write_DATA2(0x48);   

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
    UTFT_LCD_Write_COM(0x2C); 
	break;