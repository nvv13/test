/* ILI9225 LCD Registers */
#define ILI9225_DRIVER_OUTPUT_CTRL      (0x01u)  // Driver Output Control
#define ILI9225_LCD_AC_DRIVING_CTRL     (0x02u)  // LCD AC Driving Control
#define ILI9225_ENTRY_MODE            	(0x03u)  // Entry Mode
#define ILI9225_DISP_CTRL1          	(0x07u)  // Display Control 1
#define ILI9225_BLANK_PERIOD_CTRL1      (0x08u)  // Blank Period Control
#define ILI9225_FRAME_CYCLE_CTRL        (0x0Bu)  // Frame Cycle Control
#define ILI9225_INTERFACE_CTRL          (0x0Cu)  // Interface Control
#define ILI9225_OSC_CTRL             	(0x0Fu)  // Osc Control
#define ILI9225_POWER_CTRL1            	(0x10u)  // Power Control 1
#define ILI9225_POWER_CTRL2           	(0x11u)  // Power Control 2
#define ILI9225_POWER_CTRL3            	(0x12u)  // Power Control 3
#define ILI9225_POWER_CTRL4            	(0x13u)  // Power Control 4
#define ILI9225_POWER_CTRL5            	(0x14u)  // Power Control 5
#define ILI9225_VCI_RECYCLING          	(0x15u)  // VCI Recycling
#define ILI9225_RAM_ADDR_SET1           (0x20u)  // Horizontal GRAM Address Set
#define ILI9225_RAM_ADDR_SET2           (0x21u)  // Vertical GRAM Address Set
#define ILI9225_GRAM_DATA_REG           (0x22u)  // GRAM Data Register
#define ILI9225_GATE_SCAN_CTRL          (0x30u)  // Gate Scan Control Register
#define ILI9225_VERTICAL_SCROLL_CTRL1   (0x31u)  // Vertical Scroll Control 1 Register
#define ILI9225_VERTICAL_SCROLL_CTRL2   (0x32u)  // Vertical Scroll Control 2 Register
#define ILI9225_VERTICAL_SCROLL_CTRL3   (0x33u)  // Vertical Scroll Control 3 Register
#define ILI9225_PARTIAL_DRIVING_POS1    (0x34u)  // Partial Driving Position 1 Register
#define ILI9225_PARTIAL_DRIVING_POS2    (0x35u)  // Partial Driving Position 2 Register
#define ILI9225_HORIZONTAL_WINDOW_ADDR1 (0x36u)  // Horizontal Address Start Position
#define ILI9225_HORIZONTAL_WINDOW_ADDR2	(0x37u)  // Horizontal Address End Position
#define ILI9225_VERTICAL_WINDOW_ADDR1   (0x38u)  // Vertical Address Start Position
#define ILI9225_VERTICAL_WINDOW_ADDR2   (0x39u)  // Vertical Address End Position
#define ILI9225_GAMMA_CTRL1            	(0x50u)  // Gamma Control 1
#define ILI9225_GAMMA_CTRL2             (0x51u)  // Gamma Control 2
#define ILI9225_GAMMA_CTRL3            	(0x52u)  // Gamma Control 3
#define ILI9225_GAMMA_CTRL4            	(0x53u)  // Gamma Control 4
#define ILI9225_GAMMA_CTRL5            	(0x54u)  // Gamma Control 5
#define ILI9225_GAMMA_CTRL6            	(0x55u)  // Gamma Control 6
#define ILI9225_GAMMA_CTRL7            	(0x56u)  // Gamma Control 7
#define ILI9225_GAMMA_CTRL8            	(0x57u)  // Gamma Control 8
#define ILI9225_GAMMA_CTRL9             (0x58u)  // Gamma Control 9
#define ILI9225_GAMMA_CTRL10            (0x59u)  // Gamma Control 10

case ILI9225B:
    UTFT_LCD_Write_COM_DATA(ILI9225_POWER_CTRL1, 0x0000); // Set SAP,DSTB,STB
    UTFT_LCD_Write_COM_DATA(ILI9225_POWER_CTRL2, 0x0000); // Set APON,PON,AON,VCI1EN,VC
    UTFT_LCD_Write_COM_DATA(ILI9225_POWER_CTRL3, 0x0000); // Set BT,DC1,DC2,DC3
    UTFT_LCD_Write_COM_DATA(ILI9225_POWER_CTRL4, 0x0000); // Set GVDD
    UTFT_LCD_Write_COM_DATA(ILI9225_POWER_CTRL5, 0x0000); // Set VCOMH/VCOML voltage
    delay(40);
    // Power-on sequence
    UTFT_LCD_Write_COM_DATA(ILI9225_POWER_CTRL2, 0x0018); // Set APON,PON,AON,VCI1EN,VC
    UTFT_LCD_Write_COM_DATA(ILI9225_POWER_CTRL3, 0x6121); // Set BT,DC1,DC2,DC3
    UTFT_LCD_Write_COM_DATA(ILI9225_POWER_CTRL4, 0x006F); // Set GVDD   /*007F 0088 */
    UTFT_LCD_Write_COM_DATA(ILI9225_POWER_CTRL5, 0x495F); // Set VCOMH/VCOML voltage
    UTFT_LCD_Write_COM_DATA(ILI9225_POWER_CTRL1, 0x0800); // Set SAP,DSTB,STB
    delay(10);
    UTFT_LCD_Write_COM_DATA(ILI9225_POWER_CTRL2, 0x103B); // Set APON,PON,AON,VCI1EN,VC
    delay(50);
    UTFT_LCD_Write_COM_DATA(ILI9225_DRIVER_OUTPUT_CTRL, 0x011C); // set the display line number and display direction
    UTFT_LCD_Write_COM_DATA(ILI9225_LCD_AC_DRIVING_CTRL, 0x0100); // set 1 line inversion
    UTFT_LCD_Write_COM_DATA(ILI9225_ENTRY_MODE, 0x1030); // set GRAM write direction and BGR=1.
    UTFT_LCD_Write_COM_DATA(ILI9225_DISP_CTRL1, 0x0000); // Display off
    UTFT_LCD_Write_COM_DATA(ILI9225_BLANK_PERIOD_CTRL1, 0x0808); // set the back porch and front porch
    UTFT_LCD_Write_COM_DATA(ILI9225_FRAME_CYCLE_CTRL, 0x1100); // set the clocks number per line
    UTFT_LCD_Write_COM_DATA(ILI9225_INTERFACE_CTRL, 0x0000); // CPU interface
    UTFT_LCD_Write_COM_DATA(ILI9225_OSC_CTRL, 0x0D01); // Set Osc  /*0e01*/
    UTFT_LCD_Write_COM_DATA(ILI9225_VCI_RECYCLING, 0x0020); // Set VCI recycling
    UTFT_LCD_Write_COM_DATA(ILI9225_RAM_ADDR_SET1, 0x0000); // RAM Address
    UTFT_LCD_Write_COM_DATA(ILI9225_RAM_ADDR_SET2, 0x0000); // RAM Address
    // Set GRAM area
    UTFT_LCD_Write_COM_DATA(ILI9225_GATE_SCAN_CTRL, 0x0000);
    UTFT_LCD_Write_COM_DATA(ILI9225_VERTICAL_SCROLL_CTRL1, 0x00DB);
    UTFT_LCD_Write_COM_DATA(ILI9225_VERTICAL_SCROLL_CTRL2, 0x0000);
    UTFT_LCD_Write_COM_DATA(ILI9225_VERTICAL_SCROLL_CTRL3, 0x0000);
    UTFT_LCD_Write_COM_DATA(ILI9225_PARTIAL_DRIVING_POS1, 0x00DB);
    UTFT_LCD_Write_COM_DATA(ILI9225_PARTIAL_DRIVING_POS2, 0x0000);
    UTFT_LCD_Write_COM_DATA(ILI9225_HORIZONTAL_WINDOW_ADDR1, 0x00AF);
    UTFT_LCD_Write_COM_DATA(ILI9225_HORIZONTAL_WINDOW_ADDR2, 0x0000);
    UTFT_LCD_Write_COM_DATA(ILI9225_VERTICAL_WINDOW_ADDR1, 0x00DB);
    UTFT_LCD_Write_COM_DATA(ILI9225_VERTICAL_WINDOW_ADDR2, 0x0000);

    // Set GAMMA curve
    UTFT_LCD_Write_COM_DATA(ILI9225_GAMMA_CTRL1, 0x0000);
    UTFT_LCD_Write_COM_DATA(ILI9225_GAMMA_CTRL2, 0x0808);
    UTFT_LCD_Write_COM_DATA(ILI9225_GAMMA_CTRL3, 0x080A);
    UTFT_LCD_Write_COM_DATA(ILI9225_GAMMA_CTRL4, 0x000A);
    UTFT_LCD_Write_COM_DATA(ILI9225_GAMMA_CTRL5, 0x0A08);
    UTFT_LCD_Write_COM_DATA(ILI9225_GAMMA_CTRL6, 0x0808);
    UTFT_LCD_Write_COM_DATA(ILI9225_GAMMA_CTRL7, 0x0000);
    UTFT_LCD_Write_COM_DATA(ILI9225_GAMMA_CTRL8, 0x0A00);
    UTFT_LCD_Write_COM_DATA(ILI9225_GAMMA_CTRL9, 0x0710);
    UTFT_LCD_Write_COM_DATA(ILI9225_GAMMA_CTRL10, 0x0710);

    UTFT_LCD_Write_COM_DATA(ILI9225_DISP_CTRL1, 0x0012);
    delay(50);
    UTFT_LCD_Write_COM_DATA(ILI9225_DISP_CTRL1, 0x1017);

	break;
