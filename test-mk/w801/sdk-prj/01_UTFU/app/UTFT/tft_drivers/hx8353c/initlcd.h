case HX8353C:
	UTFT_LCD_Write_COM(0xB9);//SETEXTC
		UTFT_LCD_Write_DATA2(0xFF);
		UTFT_LCD_Write_DATA2(0x83);
		UTFT_LCD_Write_DATA2(0x53);

		UTFT_LCD_Write_COM(0xB0);//RADJ
		UTFT_LCD_Write_DATA2(0x3C);
		UTFT_LCD_Write_DATA2(0x01);

		UTFT_LCD_Write_COM(0xB6);//VCOM
		UTFT_LCD_Write_DATA2(0x94);
		UTFT_LCD_Write_DATA2(0x6C);
		UTFT_LCD_Write_DATA2(0x50);   

		UTFT_LCD_Write_COM(0xB1);//PWR
		UTFT_LCD_Write_DATA2(0x00);
		UTFT_LCD_Write_DATA2(0x01);
		UTFT_LCD_Write_DATA2(0x1B);
		UTFT_LCD_Write_DATA2(0x03);
		UTFT_LCD_Write_DATA2(0x01);
		UTFT_LCD_Write_DATA2(0x08);
		UTFT_LCD_Write_DATA2(0x77);
		UTFT_LCD_Write_DATA2(0x89);

		UTFT_LCD_Write_COM(0xE0); //Gamma setting for tpo Panel
		UTFT_LCD_Write_DATA2(0x50);
		UTFT_LCD_Write_DATA2(0x77);
		UTFT_LCD_Write_DATA2(0x40);
		UTFT_LCD_Write_DATA2(0x08);
		UTFT_LCD_Write_DATA2(0xBF);
		UTFT_LCD_Write_DATA2(0x00);
		UTFT_LCD_Write_DATA2(0x03);
		UTFT_LCD_Write_DATA2(0x0F);
		UTFT_LCD_Write_DATA2(0x00);
		UTFT_LCD_Write_DATA2(0x01);
		UTFT_LCD_Write_DATA2(0x73);
		UTFT_LCD_Write_DATA2(0x00);
		UTFT_LCD_Write_DATA2(0x72);
		UTFT_LCD_Write_DATA2(0x03);
		UTFT_LCD_Write_DATA2(0xB0);
		UTFT_LCD_Write_DATA2(0x0F);
		UTFT_LCD_Write_DATA2(0x08);
		UTFT_LCD_Write_DATA2(0x00);
		UTFT_LCD_Write_DATA2(0x0F);

		UTFT_LCD_Write_COM(0x3A);   
		UTFT_LCD_Write_DATA2(0x05);  //05 
		UTFT_LCD_Write_COM(0x36);    
		UTFT_LCD_Write_DATA2(0xC0); //83  //0B 

		UTFT_LCD_Write_COM(0x11); // SLPOUT  
		delay(150);

		UTFT_LCD_Write_COM(0x29);    // display on

		delay(150);
		UTFT_LCD_Write_COM(0x2D);  //Look up table

		for(int j=0;j<32;j++)
		{ UTFT_LCD_Write_DATA2(2*j); } //Red
		for(int j=0;j<64;j++)
		{ UTFT_LCD_Write_DATA2(1*j); } //Green
		for(int j=0;j<32;j++)
		{ UTFT_LCD_Write_DATA2(2*j); } //Blue 

		UTFT_LCD_Write_COM(0x2c);  
		delay(150);	 
	break;
