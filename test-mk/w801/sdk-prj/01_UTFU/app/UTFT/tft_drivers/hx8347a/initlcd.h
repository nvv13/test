case HX8347A:
	UTFT_LCD_Write_COM_DATA(0x46,0x00A4);
	UTFT_LCD_Write_COM_DATA(0x47,0x0053);
	UTFT_LCD_Write_COM_DATA(0x48,0x0000);
	UTFT_LCD_Write_COM_DATA(0x49,0x0044);
	UTFT_LCD_Write_COM_DATA(0x4a,0x0004);
	UTFT_LCD_Write_COM_DATA(0x4b,0x0067);
	UTFT_LCD_Write_COM_DATA(0x4c,0x0033);
	UTFT_LCD_Write_COM_DATA(0x4d,0x0077);
	UTFT_LCD_Write_COM_DATA(0x4e,0x0012);
	UTFT_LCD_Write_COM_DATA(0x4f,0x004C);
	UTFT_LCD_Write_COM_DATA(0x50,0x0046);
	UTFT_LCD_Write_COM_DATA(0x51,0x0044);

	//240x320 window setting
	UTFT_LCD_Write_COM_DATA(0x02,0x0000); // Column address start2
	UTFT_LCD_Write_COM_DATA(0x03,0x0000); // Column address start1
	UTFT_LCD_Write_COM_DATA(0x04,0x0000); // Column address end2
	UTFT_LCD_Write_COM_DATA(0x05,0x00ef); // Column address end1
	UTFT_LCD_Write_COM_DATA(0x06,0x0000); // Row address start2
	UTFT_LCD_Write_COM_DATA(0x07,0x0000); // Row address start1
	UTFT_LCD_Write_COM_DATA(0x08,0x0001); // Row address end2
	UTFT_LCD_Write_COM_DATA(0x09,0x003f); // Row address end1

	// Display Setting
	UTFT_LCD_Write_COM_DATA(0x01,0x0006); // IDMON=0, INVON=1, NORON=1, PTLON=0
	UTFT_LCD_Write_COM_DATA(0x16,0x00C8); // MY=0, MX=0, MV=0, ML=1, BGR=0, TEON=0   0048
	UTFT_LCD_Write_COM_DATA(0x23,0x0095); // N_DC=1001 0101
	UTFT_LCD_Write_COM_DATA(0x24,0x0095); // PI_DC=1001 0101
	UTFT_LCD_Write_COM_DATA(0x25,0x00FF); // I_DC=1111 1111

	UTFT_LCD_Write_COM_DATA(0x27,0x0002); // N_BP=0000 0010
	UTFT_LCD_Write_COM_DATA(0x28,0x0002); // N_FP=0000 0010
	UTFT_LCD_Write_COM_DATA(0x29,0x0002); // PI_BP=0000 0010
	UTFT_LCD_Write_COM_DATA(0x2a,0x0002); // PI_FP=0000 0010
	UTFT_LCD_Write_COM_DATA(0x2C,0x0002); // I_BP=0000 0010
	UTFT_LCD_Write_COM_DATA(0x2d,0x0002); // I_FP=0000 0010

	UTFT_LCD_Write_COM_DATA(0x3a,0x0001); // N_RTN=0000, N_NW=001    0001
	UTFT_LCD_Write_COM_DATA(0x3b,0x0000); // P_RTN=0000, P_NW=001
	UTFT_LCD_Write_COM_DATA(0x3c,0x00f0); // I_RTN=1111, I_NW=000
	UTFT_LCD_Write_COM_DATA(0x3d,0x0000); // DIV=00
	delay(1);
	UTFT_LCD_Write_COM_DATA(0x35,0x0038); // EQS=38h
	UTFT_LCD_Write_COM_DATA(0x36,0x0078); // EQP=78h
	UTFT_LCD_Write_COM_DATA(0x3E,0x0038); // SON=38h
	UTFT_LCD_Write_COM_DATA(0x40,0x000F); // GDON=0Fh
	UTFT_LCD_Write_COM_DATA(0x41,0x00F0); // GDOFF

	// Power Supply Setting
	UTFT_LCD_Write_COM_DATA(0x19,0x0049); // CADJ=0100, CUADJ=100, OSD_EN=1 ,60Hz
	UTFT_LCD_Write_COM_DATA(0x93,0x000F); // RADJ=1111, 100%
	delay(1);
	UTFT_LCD_Write_COM_DATA(0x20,0x0040); // BT=0100
	UTFT_LCD_Write_COM_DATA(0x1D,0x0007); // VC1=111   0007
	UTFT_LCD_Write_COM_DATA(0x1E,0x0000); // VC3=000
	UTFT_LCD_Write_COM_DATA(0x1F,0x0004); // VRH=0011

	//VCOM SETTING
	UTFT_LCD_Write_COM_DATA(0x44,0x004D); // VCM=101 0000  4D
	UTFT_LCD_Write_COM_DATA(0x45,0x000E); // VDV=1 0001   0011
	delay(1);
	UTFT_LCD_Write_COM_DATA(0x1C,0x0004); // AP=100
	delay(2);

	UTFT_LCD_Write_COM_DATA(0x1B,0x0018); // GASENB=0, PON=0, DK=1, XDK=0, VUTFT_LCD_TRI=0, STB=0
	delay(1);
	UTFT_LCD_Write_COM_DATA(0x1B,0x0010); // GASENB=0, PON=1, DK=0, XDK=0, VUTFT_LCD_TRI=0, STB=0
	delay(1);
	UTFT_LCD_Write_COM_DATA(0x43,0x0080); //set VCOMG=1
	delay(2);

	// Display ON Setting
	UTFT_LCD_Write_COM_DATA(0x90,0x007F); // SAP=0111 1111
	UTFT_LCD_Write_COM_DATA(0x26,0x0004); //GON=0, DTE=0, D=01
	delay(1);
	UTFT_LCD_Write_COM_DATA(0x26,0x0024); //GON=1, DTE=0, D=01
	UTFT_LCD_Write_COM_DATA(0x26,0x002C); //GON=1, DTE=0, D=11
	delay(1);
	UTFT_LCD_Write_COM_DATA(0x26,0x003C); //GON=1, DTE=1, D=11

	// INTERNAL REGISTER SETTING
	UTFT_LCD_Write_COM_DATA(0x57,0x0002); // TEST_Mode=1: into TEST mode
	UTFT_LCD_Write_COM_DATA(0x95,0x0001); // SET DISPLAY CLOCK AND PUMPING CLOCK TO SYNCHRONIZE
	UTFT_LCD_Write_COM_DATA(0x57,0x0000); // TEST_Mode=0: exit TEST mode
	//UTFT_LCD_Write_COM_DATA(0x21,0x0000);
	UTFT_LCD_Write_COM(0x22);   
	break;
