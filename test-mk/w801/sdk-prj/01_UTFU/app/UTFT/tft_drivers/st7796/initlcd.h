case MSP3526:


//*************3.5 ST7796S IPS**********//	
	LCD_WR_REG1(0x11);     

	delay(120);                //Delay 120ms

	LCD_WR_REG1(0x36);     // Memory Data Access Control MY,MX~~
	LCD_WR_DATA(0x48);   

	LCD_WR_REG1(0x3A);     
	LCD_WR_DATA(0x55);   //LCD_WR_DATA(0x66);

	LCD_WR_REG1(0xF0);     // Command Set Control
	LCD_WR_DATA(0xC3);   

	LCD_WR_REG1(0xF0);     
	LCD_WR_DATA(0x96);   

	LCD_WR_REG1(0xB4);     
	LCD_WR_DATA(0x01);   

	LCD_WR_REG1(0xB7);     
	LCD_WR_DATA(0xC6);   

	//LCD_WR_REG1(0xB9);     
	//LCD_WR_DATA(0x02);
	//LCD_WR_DATA(0xE0);

	LCD_WR_REG1(0xC0);     
	LCD_WR_DATA(0x80);   
	LCD_WR_DATA(0x45);   

	LCD_WR_REG1(0xC1);     
	LCD_WR_DATA(0x13);   //18  //00

	LCD_WR_REG1(0xC2);     
	LCD_WR_DATA(0xA7);   

	LCD_WR_REG1(0xC5);     
	LCD_WR_DATA(0x0A);   

	LCD_WR_REG1(0xE8);     
	LCD_WR_DATA(0x40);
	LCD_WR_DATA(0x8A);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x29);
	LCD_WR_DATA(0x19);
	LCD_WR_DATA(0xA5);
	LCD_WR_DATA(0x33);

	LCD_WR_REG1(0xE0);
	LCD_WR_DATA(0xD0);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x0F);
	LCD_WR_DATA(0x06);
	LCD_WR_DATA(0x06);
	LCD_WR_DATA(0x33);
	LCD_WR_DATA(0x30);
	LCD_WR_DATA(0x33);
	LCD_WR_DATA(0x47);
	LCD_WR_DATA(0x17);
	LCD_WR_DATA(0x13);
	LCD_WR_DATA(0x13);
	LCD_WR_DATA(0x2B);
	LCD_WR_DATA(0x31);

	LCD_WR_REG1(0xE1);
	LCD_WR_DATA(0xD0);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x0B);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x07);
	LCD_WR_DATA(0x2F);
	LCD_WR_DATA(0x33);
	LCD_WR_DATA(0x47);
	LCD_WR_DATA(0x38);
	LCD_WR_DATA(0x15);
	LCD_WR_DATA(0x16);
	LCD_WR_DATA(0x2C);
	LCD_WR_DATA(0x32);
	 

	LCD_WR_REG1(0xF0);     
	LCD_WR_DATA(0x3C);   

	LCD_WR_REG1(0xF0);     
	LCD_WR_DATA(0x69);   

	delay(120);                

	LCD_WR_REG1(0x21);     

	LCD_WR_REG1(0x29); 

//  LCD_direction(USE_HORIZONTAL); 

//  		        LCD_WR_REG1(0x36); LCD_WR_DATA(0x00);

//                        lcddev.setxcmd=0x2A;
//			lcddev.setycmd=0x2B;
//			lcddev.wramcmd=0x2C;
//			lcddev.rramcmd=0x2E;
//			lcddev.dir = direction%4;
//	switch(lcddev.dir){		  
//		case 0:						 	 		
//			lcddev.width=LCD_W;
//			lcddev.height=LCD_H;		
  		        LCD_WR_REG1(0x36); LCD_WR_DATA((1<<3)|(1<<6));
//		break;
//		case 1:
//			lcddev.width=LCD_H;
//			lcddev.height=LCD_W;
//			LCD_WriteReg(0x36,(1<<3)|(1<<5));
//		        LCD_WR_REG1(0x36); LCD_WR_DATA((1<<3)|(1<<5)); //color ok
//		break;
//		case 2:						 	 		
//			lcddev.width=LCD_W;
//			lcddev.height=LCD_H;	
//			LCD_WriteReg(0x36,(1<<3)|(1<<7));
//  		        LCD_WR_REG1(0x36); LCD_WR_DATA((1<<3)|(1<<7));
//		break;
//		case 3:
//			lcddev.width=LCD_H;
//			lcddev.height=LCD_W;
//			LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<6)|(1<<5));
//  		        LCD_WR_REG1(0x36); LCD_WR_DATA((1<<3)|(1<<7)|(1<<6)|(1<<5));
//		break;	
//		default:break;
//	}	



	break;