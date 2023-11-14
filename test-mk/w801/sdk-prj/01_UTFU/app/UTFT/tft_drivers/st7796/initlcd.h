case MSP3526:
case TFT02_0V89:


//*************3.5 ST7796S IPS**********//	
	UTFT_LCD_Write_COM(0x11);     

	delay(120);                //Delay 120ms

	UTFT_LCD_Write_COM(0x36);     // Memory Data Access Control MY,MX~~
	UTFT_LCD_Write_DATA2(0x48);   

	UTFT_LCD_Write_COM(0x3A);     
	UTFT_LCD_Write_DATA2(0x55);   //UTFT_LCD_Write_DATA2(0x66);

	UTFT_LCD_Write_COM(0xF0);     // Command Set Control
	UTFT_LCD_Write_DATA2(0xC3);   

	UTFT_LCD_Write_COM(0xF0);     
	UTFT_LCD_Write_DATA2(0x96);   

	UTFT_LCD_Write_COM(0xB4);     
	UTFT_LCD_Write_DATA2(0x01);   

	UTFT_LCD_Write_COM(0xB7);     
	UTFT_LCD_Write_DATA2(0xC6);   

	//UTFT_LCD_Write_COM(0xB9);     
	//UTFT_LCD_Write_DATA2(0x02);
	//UTFT_LCD_Write_DATA2(0xE0);

	UTFT_LCD_Write_COM(0xC0);     
	UTFT_LCD_Write_DATA2(0x80);   
	UTFT_LCD_Write_DATA2(0x45);   

	UTFT_LCD_Write_COM(0xC1);     
	UTFT_LCD_Write_DATA2(0x13);   //18  //00

	UTFT_LCD_Write_COM(0xC2);     
	UTFT_LCD_Write_DATA2(0xA7);   

	UTFT_LCD_Write_COM(0xC5);     
	UTFT_LCD_Write_DATA2(0x0A);   

	UTFT_LCD_Write_COM(0xE8);     
	UTFT_LCD_Write_DATA2(0x40);
	UTFT_LCD_Write_DATA2(0x8A);
	UTFT_LCD_Write_DATA2(0x00);
	UTFT_LCD_Write_DATA2(0x00);
	UTFT_LCD_Write_DATA2(0x29);
	UTFT_LCD_Write_DATA2(0x19);
	UTFT_LCD_Write_DATA2(0xA5);
	UTFT_LCD_Write_DATA2(0x33);

	UTFT_LCD_Write_COM(0xE0);
	UTFT_LCD_Write_DATA2(0xD0);
	UTFT_LCD_Write_DATA2(0x08);
	UTFT_LCD_Write_DATA2(0x0F);
	UTFT_LCD_Write_DATA2(0x06);
	UTFT_LCD_Write_DATA2(0x06);
	UTFT_LCD_Write_DATA2(0x33);
	UTFT_LCD_Write_DATA2(0x30);
	UTFT_LCD_Write_DATA2(0x33);
	UTFT_LCD_Write_DATA2(0x47);
	UTFT_LCD_Write_DATA2(0x17);
	UTFT_LCD_Write_DATA2(0x13);
	UTFT_LCD_Write_DATA2(0x13);
	UTFT_LCD_Write_DATA2(0x2B);
	UTFT_LCD_Write_DATA2(0x31);

	UTFT_LCD_Write_COM(0xE1);
	UTFT_LCD_Write_DATA2(0xD0);
	UTFT_LCD_Write_DATA2(0x0A);
	UTFT_LCD_Write_DATA2(0x11);
	UTFT_LCD_Write_DATA2(0x0B);
	UTFT_LCD_Write_DATA2(0x09);
	UTFT_LCD_Write_DATA2(0x07);
	UTFT_LCD_Write_DATA2(0x2F);
	UTFT_LCD_Write_DATA2(0x33);
	UTFT_LCD_Write_DATA2(0x47);
	UTFT_LCD_Write_DATA2(0x38);
	UTFT_LCD_Write_DATA2(0x15);
	UTFT_LCD_Write_DATA2(0x16);
	UTFT_LCD_Write_DATA2(0x2C);
	UTFT_LCD_Write_DATA2(0x32);
	 

	UTFT_LCD_Write_COM(0xF0);     
	UTFT_LCD_Write_DATA2(0x3C);   

	UTFT_LCD_Write_COM(0xF0);     
	UTFT_LCD_Write_DATA2(0x69);   

	delay(120);                

	UTFT_LCD_Write_COM(0x21);     

	UTFT_LCD_Write_COM(0x29); 

//  LCD_direction(USE_HORIZONTAL); 

//  		        UTFT_LCD_Write_COM(0x36); UTFT_LCD_Write_DATA2(0x00);

//                        lcddev.setxcmd=0x2A;
//			lcddev.setycmd=0x2B;
//			lcddev.wramcmd=0x2C;
//			lcddev.rramcmd=0x2E;
//			lcddev.dir = direction%4;
//	switch(lcddev.dir){		  
//		case 0:						 	 		
//			lcddev.width=LCD_W;
//			lcddev.height=LCD_H;		
  		        UTFT_LCD_Write_COM(0x36); UTFT_LCD_Write_DATA2((1<<3)|(1<<6));
//		break;
//		case 1:
//			lcddev.width=LCD_H;
//			lcddev.height=LCD_W;
//			LCD_WriteReg(0x36,(1<<3)|(1<<5));
//		        UTFT_LCD_Write_COM(0x36); UTFT_LCD_Write_DATA2((1<<3)|(1<<5)); //color ok
//		break;
//		case 2:						 	 		
//			lcddev.width=LCD_W;
//			lcddev.height=LCD_H;	
//			LCD_WriteReg(0x36,(1<<3)|(1<<7));
//  		        UTFT_LCD_Write_COM(0x36); UTFT_LCD_Write_DATA2((1<<3)|(1<<7));
//		break;
//		case 3:
//			lcddev.width=LCD_H;
//			lcddev.height=LCD_W;
//			LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<6)|(1<<5));
//  		        UTFT_LCD_Write_COM(0x36); UTFT_LCD_Write_DATA2((1<<3)|(1<<7)|(1<<6)|(1<<5));
//		break;	
//		default:break;
//	}	



	break;