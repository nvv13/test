case ILI9486:
	UTFT_LCD_Write_COM(0x11);		// Sleep OUT
	delay(50);
 
	UTFT_LCD_Write_COM(0xF2);		// ?????
	UTFT_LCD_Write_DATA2(0x1C);
	UTFT_LCD_Write_DATA2(0xA3);
	UTFT_LCD_Write_DATA2(0x32);
	UTFT_LCD_Write_DATA2(0x02);
	UTFT_LCD_Write_DATA2(0xb2);
	UTFT_LCD_Write_DATA2(0x12);
	UTFT_LCD_Write_DATA2(0xFF);
	UTFT_LCD_Write_DATA2(0x12);
	UTFT_LCD_Write_DATA2(0x00);

	UTFT_LCD_Write_COM(0xF1);		// ?????
	UTFT_LCD_Write_DATA2(0x36);
	UTFT_LCD_Write_DATA2(0xA4);

	UTFT_LCD_Write_COM(0xF8);		// ?????
	UTFT_LCD_Write_DATA2(0x21);
	UTFT_LCD_Write_DATA2(0x04);

	UTFT_LCD_Write_COM(0xF9);		// ?????
	UTFT_LCD_Write_DATA2(0x00);
	UTFT_LCD_Write_DATA2(0x08);

	UTFT_LCD_Write_COM(0xC0);		// Power Control 1
	UTFT_LCD_Write_DATA2(0x0d);
	UTFT_LCD_Write_DATA2(0x0d);

	UTFT_LCD_Write_COM(0xC1);		// Power Control 2
	UTFT_LCD_Write_DATA2(0x43);
	UTFT_LCD_Write_DATA2(0x00);

	UTFT_LCD_Write_COM(0xC2);		// Power Control 3
	UTFT_LCD_Write_DATA2(0x00);

	UTFT_LCD_Write_COM(0xC5);		// VCOM Control
	UTFT_LCD_Write_DATA2(0x00);
	UTFT_LCD_Write_DATA2(0x48);

	UTFT_LCD_Write_COM(0xB6);		// Display Function Control
	UTFT_LCD_Write_DATA2(0x00);
	UTFT_LCD_Write_DATA2(0x22);		// 0x42 = Rotate display 180 deg.
	UTFT_LCD_Write_DATA2(0x3B);

	UTFT_LCD_Write_COM(0xE0);		// PGAMCTRL (Positive Gamma Control)
	UTFT_LCD_Write_DATA2(0x0f);
	UTFT_LCD_Write_DATA2(0x24);
	UTFT_LCD_Write_DATA2(0x1c);
	UTFT_LCD_Write_DATA2(0x0a);
	UTFT_LCD_Write_DATA2(0x0f);
	UTFT_LCD_Write_DATA2(0x08);
	UTFT_LCD_Write_DATA2(0x43);
	UTFT_LCD_Write_DATA2(0x88);
	UTFT_LCD_Write_DATA2(0x32);
	UTFT_LCD_Write_DATA2(0x0f);
	UTFT_LCD_Write_DATA2(0x10);
	UTFT_LCD_Write_DATA2(0x06);
	UTFT_LCD_Write_DATA2(0x0f);
	UTFT_LCD_Write_DATA2(0x07);
	UTFT_LCD_Write_DATA2(0x00);

	UTFT_LCD_Write_COM(0xE1);		// NGAMCTRL (Negative Gamma Control)
	UTFT_LCD_Write_DATA2(0x0F);
	UTFT_LCD_Write_DATA2(0x38);
	UTFT_LCD_Write_DATA2(0x30);
	UTFT_LCD_Write_DATA2(0x09);
	UTFT_LCD_Write_DATA2(0x0f);
	UTFT_LCD_Write_DATA2(0x0f);
	UTFT_LCD_Write_DATA2(0x4e);
	UTFT_LCD_Write_DATA2(0x77);
	UTFT_LCD_Write_DATA2(0x3c);
	UTFT_LCD_Write_DATA2(0x07);
	UTFT_LCD_Write_DATA2(0x10);
	UTFT_LCD_Write_DATA2(0x05);
	UTFT_LCD_Write_DATA2(0x23);
	UTFT_LCD_Write_DATA2(0x1b);
	UTFT_LCD_Write_DATA2(0x00); 

	UTFT_LCD_Write_COM(0x20);		// Display Inversion OFF
	UTFT_LCD_Write_DATA2(0x00);//C8 	 

	UTFT_LCD_Write_COM(0x36);		// Memory Access Control
	UTFT_LCD_Write_DATA2(0x0A);

	UTFT_LCD_Write_COM(0x3A);		// Interface Pixel Format
	UTFT_LCD_Write_DATA2(0x55); 

	UTFT_LCD_Write_COM(0x2A);		// Column Addess Set
	UTFT_LCD_Write_DATA2(0x00);
	UTFT_LCD_Write_DATA2(0x00);
	UTFT_LCD_Write_DATA2(0x01);
	UTFT_LCD_Write_DATA2(0xDF);

	UTFT_LCD_Write_COM(0x002B);		// Page Address Set
	UTFT_LCD_Write_DATA2(0x00);
	UTFT_LCD_Write_DATA2(0x00);
	UTFT_LCD_Write_DATA2(0x01);
	UTFT_LCD_Write_DATA2(0x3f);	 
	delay(50);
	UTFT_LCD_Write_COM(0x0029);		// Display ON
	UTFT_LCD_Write_COM(0x002C);		// Memory Write
	break;


case TFT3_5SP_9488:

#define ILI9488_NOP           0x00
#define ILI9488_SWRESET       0x01
#define ILI9488_RDDID         0x04
#define ILI9488_RDDST         0x09

#define ILI9488_SLPIN         0x10
#define ILI9488_SLPOUT        0x11
#define ILI9488_PTLON         0x12
#define ILI9488_NORON         0x13

#define ILI9488_RDMODE        0x0A
#define ILI9488_RDMADCTL      0x0B
#define ILI9488_RDPIXFMT      0x0C
#define ILI9488_RDIMGFMT      0x0D
#define ILI9488_RDSELFDIAG    0x0F

#define ILI9488_INVOFF        0x20
#define ILI9488_INVON         0x21
#define ILI9488_GAMMASET      0x26
#define ILI9488_DISPOFF       0x28
#define ILI9488_DISPON        0x29

#define ILI9488_CASET         0x2A
#define ILI9488_PASET         0x2B
#define ILI9488_RAMWR         0x2C
#define ILI9488_RAMRD         0x2E

#define ILI9488_PTLAR         0x30
#define ILI9488_VSCRDEF       0x33
#define ILI9488_MADCTL        0x36
#define ILI9488_VSCRSADD      0x37
#define ILI9488_PIXFMT        0x3A
#define ILI9488_RAMWRCONT     0x3C
#define ILI9488_RAMRDCONT     0x3E

#define ILI9488_IMCTR         0xB0
#define ILI9488_FRMCTR1       0xB1
#define ILI9488_FRMCTR2       0xB2
#define ILI9488_FRMCTR3       0xB3
#define ILI9488_INVCTR        0xB4
#define ILI9488_DFUNCTR       0xB6

#define ILI9488_PWCTR1        0xC0
#define ILI9488_PWCTR2        0xC1
#define ILI9488_PWCTR3        0xC2
#define ILI9488_PWCTR4        0xC3
#define ILI9488_PWCTR5        0xC4
#define ILI9488_VMCTR1        0xC5
#define ILI9488_VMCTR2        0xC7

#define ILI9488_RDID1         0xDA
#define ILI9488_RDID2         0xDB
#define ILI9488_RDID3         0xDC
#define ILI9488_RDID4         0xDD

#define ILI9488_GMCTRP1       0xE0
#define ILI9488_GMCTRN1       0xE1
#define ILI9488_IMGFUNCT      0xE9

#define ILI9488_ADJCTR3       0xF7

#define ILI9488_MAD_RGB       0x08
#define ILI9488_MAD_BGR       0x00

#define ILI9488_MAD_VERTICAL  0x20
#define ILI9488_MAD_X_LEFT    0x00
#define ILI9488_MAD_X_RIGHT   0x40
#define ILI9488_MAD_Y_UP      0x80
#define ILI9488_MAD_Y_DOWN    0x00

//#if ILI9488_COLORMODE == 0
#define ILI9488_MAD_COLORMODE    ILI9488_MAD_RGB
//#else
//#define ILI9488_MAD_COLORMODE    ILI9488_MAD_BGR
//#endif
#define  ILI9488_ORIENTATION       0

#define LCD_ORIENTATION  ILI9488_ORIENTATION

	delay(105);
	UTFT_LCD_Write_COM(ILI9488_SWRESET);	
	delay(5);

	UTFT_LCD_Write_COM(0x11);		// Sleep OUT
	delay(50);

  // positive gamma control
  UTFT_LCD_Write_COM(ILI9488_GMCTRP1);
  UTFT_LCD_Write_DATA2(0x00);
  UTFT_LCD_Write_DATA2(0x03);
  UTFT_LCD_Write_DATA2(0x09);
  UTFT_LCD_Write_DATA2(0x08);
  UTFT_LCD_Write_DATA2(0x16);
  UTFT_LCD_Write_DATA2(0x0a);
  UTFT_LCD_Write_DATA2(0x3f);
  UTFT_LCD_Write_DATA2(0x78);
  UTFT_LCD_Write_DATA2(0x4c);
  UTFT_LCD_Write_DATA2(0x09);
  UTFT_LCD_Write_DATA2(0x0a);
  UTFT_LCD_Write_DATA2(0x08);
  UTFT_LCD_Write_DATA2(0x16);
  UTFT_LCD_Write_DATA2(0x1a);
  UTFT_LCD_Write_DATA2(0x0f);


  // negative gamma control
  UTFT_LCD_Write_COM(ILI9488_GMCTRN1);
  UTFT_LCD_Write_DATA2(0x00);
  UTFT_LCD_Write_DATA2(0x16);
  UTFT_LCD_Write_DATA2(0x19);
  UTFT_LCD_Write_DATA2(0x03);
  UTFT_LCD_Write_DATA2(0x0F);
  UTFT_LCD_Write_DATA2(0x05);
  UTFT_LCD_Write_DATA2(0x32);
  UTFT_LCD_Write_DATA2(0x45);
  UTFT_LCD_Write_DATA2(0x46);
  UTFT_LCD_Write_DATA2(0x04);
  UTFT_LCD_Write_DATA2(0x0E);
  UTFT_LCD_Write_DATA2(0x0D);
  UTFT_LCD_Write_DATA2(0x35);
  UTFT_LCD_Write_DATA2(0x37);
  UTFT_LCD_Write_DATA2(0x0F);

  // Power Control 1 (Vreg1out, Verg2out)
  UTFT_LCD_Write_COM(ILI9488_PWCTR1);
  UTFT_LCD_Write_DATA2(0x17);
  UTFT_LCD_Write_DATA2(0x15);
  delay(5);
  // Power Control 2 (VGH,VGL)
  UTFT_LCD_Write_COM(ILI9488_PWCTR2); UTFT_LCD_Write_DATA2(0x41);
  delay(5);
  // Power Control 3 (Vcom)
  UTFT_LCD_Write_COM(ILI9488_VMCTR1);
  UTFT_LCD_Write_DATA2(0x00);
  UTFT_LCD_Write_DATA2(0x12);
  UTFT_LCD_Write_DATA2(0x80);
  delay(5);

/* Interface mode
   - 0: SPI mode (the lcd does not work in 16bit/pixel mode in spi, so you have to write 24bit/pixel)
   - 1: paralell mode */
#define  ILI9488_INTERFACE        0

  #if ILI9488_INTERFACE == 0
  UTFT_LCD_Write_COM(ILI9488_PIXFMT); UTFT_LCD_Write_DATA2(0x66); // Interface Pixel Format (24 bit)

  //#if LCD_SPI_MODE != 2
  // LCD_IO_WriteCmd8(0xFB); LCD_IO_WriteData8(0x80);
  UTFT_LCD_Write_COM(ILI9488_IMCTR); UTFT_LCD_Write_DATA2(0x80); // Interface Mode Control (SDO NOT USE)
  //#else
  //UTFT_LCD_Write_COM(ILI9488_IMCTR); UTFT_LCD_Write_DATA2(0x00); // Interface Mode Control (SDO USE)
  //#endif
  #elif ILI9488_INTERFACE == 1
    UTFT_LCD_Write_COM(ILI9488_PIXFMT); UTFT_LCD_Write_DATA2(0x55); // Interface Pixel Format (16 bit)
  #endif

  UTFT_LCD_Write_COM(ILI9488_FRMCTR1); UTFT_LCD_Write_DATA2(0xA0); // Frame rate (60Hz)
  UTFT_LCD_Write_COM(ILI9488_INVCTR); UTFT_LCD_Write_DATA2(0x02); // Display Inversion Control (2-dot)
  //UTFT_LCD_Write_COM(ILI9488_INVON);
  //UTFT_LCD_Write_COM(ILI9488_INVOFF);
  UTFT_LCD_Write_COM(ILI9488_DFUNCTR); UTFT_LCD_Write_DATA2(0x02);UTFT_LCD_Write_DATA2(0x02); // Display Function Control RGB/MCU Interface Control
  UTFT_LCD_Write_COM(ILI9488_IMGFUNCT); UTFT_LCD_Write_DATA2(0x00); // Set Image Functio (Disable 24 bit data)
  UTFT_LCD_Write_COM(ILI9488_ADJCTR3); UTFT_LCD_Write_DATA2(0xA9);UTFT_LCD_Write_DATA2(0x51);UTFT_LCD_Write_DATA2(0x2C);UTFT_LCD_Write_DATA2(0x82); // Adjust Control (D7 stream, loose)
  delay(5);
  UTFT_LCD_Write_COM(ILI9488_SLPOUT);      // Exit Sleep
  delay(120);
  UTFT_LCD_Write_COM(ILI9488_DISPON);      // Display on
  delay(5);

#define ILI9488_MAD_DATA_RIGHT_THEN_DOWN   ILI9488_MAD_COLORMODE | ILI9488_MAD_X_RIGHT | ILI9488_MAD_Y_DOWN

  UTFT_LCD_Write_COM(ILI9488_MADCTL); UTFT_LCD_Write_DATA2(ILI9488_MAD_DATA_RIGHT_THEN_DOWN);

	break;



case MHS3528:

      if(_SPI_16bit)
        {// SPI 16bit
        LCD_WR_REG(0xb0); // Interface Mode Control
        UTFT_LCD_Write_DATA (0x00 ,0x00);

        LCD_WR_REG(0x11); // Sleep out, also SW reset
        delay(250);

        LCD_WR_REG(0x3A); // Interface Pixel Format, 16 bits / pixel
        UTFT_LCD_Write_DATA (0x00 ,0x55);

        LCD_WR_REG(0xC2); // Power Control 3 (For Normal Mode)
        UTFT_LCD_Write_DATA (0x00 ,0x44);

        LCD_WR_REG(0xC5); // VCOM Control
        UTFT_LCD_Write_DATA (0x00 ,0x00);
        UTFT_LCD_Write_DATA (0x00 ,0x00);
        UTFT_LCD_Write_DATA (0x00 ,0x00);
        UTFT_LCD_Write_DATA (0x00 ,0x00);

        LCD_WR_REG(0xE0); // PGAMCTRL(Positive Gamma Control)
        UTFT_LCD_Write_DATA (0x00 ,0x0F);
        UTFT_LCD_Write_DATA (0x00 ,0x1F);
        UTFT_LCD_Write_DATA (0x00 ,0x1C);
        UTFT_LCD_Write_DATA (0x00 ,0x0C);
        UTFT_LCD_Write_DATA (0x00 ,0x0F);
        UTFT_LCD_Write_DATA (0x00 ,0x08);
        UTFT_LCD_Write_DATA (0x00 ,0x48);
        UTFT_LCD_Write_DATA (0x00 ,0x98);
        UTFT_LCD_Write_DATA (0x00 ,0x37);
        UTFT_LCD_Write_DATA (0x00 ,0x0A);
        UTFT_LCD_Write_DATA (0x00 ,0x13);
        UTFT_LCD_Write_DATA (0x00 ,0x04);
        UTFT_LCD_Write_DATA (0x00 ,0x11);
        UTFT_LCD_Write_DATA (0x00 ,0x0D);
        UTFT_LCD_Write_DATA (0x00 ,0x00);

        LCD_WR_REG(0xE1); // NGAMCTRL (Negative Gamma Correction)
        UTFT_LCD_Write_DATA (0x00 ,0x0F);
        UTFT_LCD_Write_DATA (0x00 ,0x32);
        UTFT_LCD_Write_DATA (0x00 ,0x2E);
        UTFT_LCD_Write_DATA (0x00 ,0x0B);
        UTFT_LCD_Write_DATA (0x00 ,0x0D);
        UTFT_LCD_Write_DATA (0x00 ,0x05);
        UTFT_LCD_Write_DATA (0x00 ,0x47);
        UTFT_LCD_Write_DATA (0x00 ,0x75);
        UTFT_LCD_Write_DATA (0x00 ,0x37);
        UTFT_LCD_Write_DATA (0x00 ,0x06);
        UTFT_LCD_Write_DATA (0x00 ,0x10);
        UTFT_LCD_Write_DATA (0x00 ,0x03);
        UTFT_LCD_Write_DATA (0x00 ,0x24);
        UTFT_LCD_Write_DATA (0x00 ,0x20);
        UTFT_LCD_Write_DATA (0x00 ,0x00);


        LCD_WR_REG(0xE2); // Digital Gamma Control 1
        UTFT_LCD_Write_DATA (0x00 ,0x0F);
        UTFT_LCD_Write_DATA (0x00 ,0x32);
        UTFT_LCD_Write_DATA (0x00 ,0x2E);
        UTFT_LCD_Write_DATA (0x00 ,0x0B);
        UTFT_LCD_Write_DATA (0x00 ,0x0D);
        UTFT_LCD_Write_DATA (0x00 ,0x05);
        UTFT_LCD_Write_DATA (0x00 ,0x47);
        UTFT_LCD_Write_DATA (0x00 ,0x75);
        UTFT_LCD_Write_DATA (0x00 ,0x37);
        UTFT_LCD_Write_DATA (0x00 ,0x06);
        UTFT_LCD_Write_DATA (0x00 ,0x10);
        UTFT_LCD_Write_DATA (0x00 ,0x03);
        UTFT_LCD_Write_DATA (0x00 ,0x24);
        UTFT_LCD_Write_DATA (0x00 ,0x20);
        UTFT_LCD_Write_DATA (0x00 ,0x00);


        LCD_WR_REG(0x20); // Display Inversion OFF   RPi LCD (A)
//      LCD_WR_REG(0x21); // Display Inversion ON    RPi LCD (B)

        LCD_WR_REG(0x36); // Memory Access Control  RGB = 0x48
        UTFT_LCD_Write_DATA (0x00 ,0x48);
                          // Memory Access Control, BGR = 0x28

        LCD_WR_REG(0x29); // Display ON
      }
      else
      { // SPI 8bit

        UTFT_LCD_Write_COM(0xb0); // Interface Mode Control
        UTFT_LCD_Write_DATA2(0x00);

        UTFT_LCD_Write_COM(0x11); // Sleep out, also SW reset
        delay(250);

        UTFT_LCD_Write_COM(0x3A); // Interface Pixel Format, 16 bits / pixel
        UTFT_LCD_Write_DATA2(0x55);

        UTFT_LCD_Write_COM(0xC2); // Power Control 3 (For Normal Mode)
        UTFT_LCD_Write_DATA2(0x44);

        UTFT_LCD_Write_COM(0xC5); // VCOM Control
        UTFT_LCD_Write_DATA2(0x00);
        UTFT_LCD_Write_DATA2(0x00);
        UTFT_LCD_Write_DATA2(0x00);
        UTFT_LCD_Write_DATA2(0x00);

        UTFT_LCD_Write_COM(0xE0); // PGAMCTRL(Positive Gamma Control)
        UTFT_LCD_Write_DATA2(0x0F);
        UTFT_LCD_Write_DATA2(0x1F);
        UTFT_LCD_Write_DATA2(0x1C);
        UTFT_LCD_Write_DATA2(0x0C);
        UTFT_LCD_Write_DATA2(0x0F);
        UTFT_LCD_Write_DATA2(0x08);
        UTFT_LCD_Write_DATA2(0x48);
        UTFT_LCD_Write_DATA2(0x98);
        UTFT_LCD_Write_DATA2(0x37);
        UTFT_LCD_Write_DATA2(0x0A);
        UTFT_LCD_Write_DATA2(0x13);
        UTFT_LCD_Write_DATA2(0x04);
        UTFT_LCD_Write_DATA2(0x11);
        UTFT_LCD_Write_DATA2(0x0D);
        UTFT_LCD_Write_DATA2(0x00);

        UTFT_LCD_Write_COM(0xE1); // NGAMCTRL (Negative Gamma Correction)
        UTFT_LCD_Write_DATA2(0x0F);
        UTFT_LCD_Write_DATA2(0x32);
        UTFT_LCD_Write_DATA2(0x2E);
        UTFT_LCD_Write_DATA2(0x0B);
        UTFT_LCD_Write_DATA2(0x0D);
        UTFT_LCD_Write_DATA2(0x05);
        UTFT_LCD_Write_DATA2(0x47);
        UTFT_LCD_Write_DATA2(0x75);
        UTFT_LCD_Write_DATA2(0x37);
        UTFT_LCD_Write_DATA2(0x06);
        UTFT_LCD_Write_DATA2(0x10);
        UTFT_LCD_Write_DATA2(0x03);
        UTFT_LCD_Write_DATA2(0x24);
        UTFT_LCD_Write_DATA2(0x20);
        UTFT_LCD_Write_DATA2(0x00);


        UTFT_LCD_Write_COM(0xE2); // Digital Gamma Control 1
        UTFT_LCD_Write_DATA2(0x0F);
        UTFT_LCD_Write_DATA2(0x32);
        UTFT_LCD_Write_DATA2(0x2E);
        UTFT_LCD_Write_DATA2(0x0B);
        UTFT_LCD_Write_DATA2(0x0D);
        UTFT_LCD_Write_DATA2(0x05);
        UTFT_LCD_Write_DATA2(0x47);
        UTFT_LCD_Write_DATA2(0x75);
        UTFT_LCD_Write_DATA2(0x37);
        UTFT_LCD_Write_DATA2(0x06);
        UTFT_LCD_Write_DATA2(0x10);
        UTFT_LCD_Write_DATA2(0x03);
        UTFT_LCD_Write_DATA2(0x24);
        UTFT_LCD_Write_DATA2(0x20);
        UTFT_LCD_Write_DATA2(0x00);


        UTFT_LCD_Write_COM(0x20); // Display Inversion OFF   RPi LCD (A)
//      UTFT_LCD_Write_COM(0x21); // Display Inversion ON    RPi LCD (B)

        UTFT_LCD_Write_COM(0x36); // Memory Access Control  RGB = 0x48
        UTFT_LCD_Write_DATA2(0x48);
                          // Memory Access Control, BGR = 0x28

        UTFT_LCD_Write_COM(0x29); // Display ON
      }
        delay(250);

	break;


