

#define LOW	0
#define OUTPUT	1


static void pinMode(byte PIN,byte b_mode)
 {
 if(PIN==NO_GPIO_PIN)return;
 tls_gpio_cfg ((enum tls_io_name)PIN, WM_GPIO_DIR_OUTPUT,WM_GPIO_ATTR_FLOATING);
 };
//volatile 
static uint32_t* portOutputRegister(u8 PIN)
{
        if (PIN >= WM_IO_PB_00) 
        {
        return (uint32_t*) TLS_IO_AB_OFFSET;
        }
        else
        {
         return (uint32_t*) 0;
        }
};
static u8 digitalPinToPort(byte PIN){return PIN;};
static u8 digitalPinToBitMask(byte PIN){return PIN;};
static void digitalWrite(byte PIN,int LEVEL){tls_gpio_write ((enum tls_io_name)PIN, LEVEL ? 1 : 0 );};

//		P_RS	= portOutputRegister(digitalPinToPort(RS));
//		B_RS	= digitalPinToBitMask(RS);

static void cbi(uint32_t* PORT_REG, u8 PIN)
{
        if(PIN==NO_GPIO_PIN)return;

	u32 cpu_sr = 0;
        u32 reg;
	u32	reg_en;
        u8  pin;
        u16 offset;

        if (PIN >= WM_IO_PB_00) // w801 chip, only two GPIO port, PA (GPIOA - 16 bit) and PB (GPIOB - 32 bit), max power 12ma
        {
          pin    = PIN - WM_IO_PB_00;
          offset = TLS_IO_AB_OFFSET;
        }
        else
        {
          pin    = PIN;
          offset = 0;
        }

	
	cpu_sr = tls_os_set_critical();  // disable Interrupt !!!
	
	reg_en = tls_reg_read32(HR_GPIO_DATA_EN + offset);
	tls_reg_write32(HR_GPIO_DATA_EN + offset, reg_en | (1 << pin)); // enabled control reg from need pin

	reg = tls_reg_read32(HR_GPIO_DATA + offset); // load all pins from port

        tls_reg_write32(HR_GPIO_DATA + offset, reg & (~(1 << pin)));/* write low from pin */

        tls_reg_write32(HR_GPIO_DATA_EN + offset, reg_en); // reg_en return

	tls_os_release_critical(cpu_sr); // enable Interrupt

}

static void sbi(uint32_t* PORT_REG, u8 PIN)
{
        if(PIN==NO_GPIO_PIN)return;

	u32 cpu_sr = 0;
        u32 reg;
	u32	reg_en;
        u8  pin;
        u16 offset;

        if (PIN >= WM_IO_PB_00) // w801 chip, only two GPIO port, PA (GPIOA - 16 bit) and PB (GPIOB - 32 bit), max power 12ma
        {
          pin    = PIN - WM_IO_PB_00;
          offset = TLS_IO_AB_OFFSET;
        }
        else
        {
          pin    = PIN;
          offset = 0;
        }

	
	cpu_sr = tls_os_set_critical();  // disable Interrupt !!!
	
	reg_en = tls_reg_read32(HR_GPIO_DATA_EN + offset);
	tls_reg_write32(HR_GPIO_DATA_EN + offset, reg_en | (1 << pin)); // enabled control reg from need pin

	reg = tls_reg_read32(HR_GPIO_DATA + offset); // load all pins from port

        tls_reg_write32(HR_GPIO_DATA + offset, reg | (1 << pin));/* write Hi from pin */

        tls_reg_write32(HR_GPIO_DATA_EN + offset, reg_en); // reg_en return

	tls_os_release_critical(cpu_sr); // enable Interrupt

}

/*
static void pulse_high(uint32_t* PORT_REG, u8 PIN)
{
	u32 cpu_sr = 0;
        u32 reg;
	u32	reg_en;
        u8  pin;
        u16 offset;

        if (PIN >= WM_IO_PB_00) // w801 chip, only two GPIO port, PA (GPIOA - 16 bit) and PB (GPIOB - 32 bit), max power 12ma
        {
          pin    = PIN - WM_IO_PB_00;
          offset = TLS_IO_AB_OFFSET;
        }
        else
        {
          pin    = PIN;
          offset = 0;
        }

	
	cpu_sr = tls_os_set_critical();  // disable Interrupt !!!
	
	reg_en = tls_reg_read32(HR_GPIO_DATA_EN + offset);
	tls_reg_write32(HR_GPIO_DATA_EN + offset, reg_en | (1 << pin)); // enabled control reg from need pin

	reg = tls_reg_read32(HR_GPIO_DATA + offset); // load all pins from port

        tls_reg_write32(HR_GPIO_DATA + offset, reg | (1 << pin));// write Hi from pin 

        tls_reg_write32(HR_GPIO_DATA + offset, reg & (~(1 << pin)));// write low from pin 

        tls_reg_write32(HR_GPIO_DATA_EN + offset, reg_en); // reg_en return

	tls_os_release_critical(cpu_sr); // enable Interrupt

}
*/

/*
static void pulse_low(uint32_t* PORT_REG, u8 PIN)
{
	u32 cpu_sr = 0;
        u32 reg;
	u32	reg_en;
        u8  pin;
        u16 offset;

        if (PIN >= WM_IO_PB_00) // w801 chip, only two GPIO port, PA (GPIOA - 16 bit) and PB (GPIOB - 32 bit), max power 12ma
        {
          pin    = PIN - WM_IO_PB_00;
          offset = TLS_IO_AB_OFFSET;
        }
        else
        {
          pin    = PIN;
          offset = 0;
        }

	
	cpu_sr = tls_os_set_critical();  // disable Interrupt !!!
	
	reg_en = tls_reg_read32(HR_GPIO_DATA_EN + offset);
	tls_reg_write32(HR_GPIO_DATA_EN + offset, reg_en | (1 << pin)); // enabled control reg from need pin

	reg = tls_reg_read32(HR_GPIO_DATA + offset); // load all pins from port

        tls_reg_write32(HR_GPIO_DATA + offset, reg | (1 << pin));// write Hi from pin 

        tls_reg_write32(HR_GPIO_DATA + offset, reg & (~(1 << pin)));// write low from pin 

        tls_reg_write32(HR_GPIO_DATA_EN + offset, reg_en); // reg_en return

	tls_os_release_critical(cpu_sr); // enable Interrupt

}

*/