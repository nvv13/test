
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
static void digitalWrite(byte PIN,int LEVEL){
 if(PIN==NO_GPIO_PIN)return;
 tls_gpio_write ((enum tls_io_name)PIN, LEVEL ? 1 : 0 );
 };

//		P_RS	= portOutputRegister(digitalPinToPort(RS));
//		B_RS	= digitalPinToBitMask(RS);

static u8 sost_RS=255;

static void sbi_RS(void)
{
if(sost_RS==1)return;
sbi(P_RS, B_RS);
}
static void cbi_RS(void)
{
if(sost_RS==0)return;
cbi(P_RS, B_RS);
}

static u8 sost_SDA=255;

static void sbi_SDA(void)
{
if(sost_SDA==1)return;
sbi(P_SDA, B_SDA);
}
static void cbi_SDA(void)
{
if(sost_SDA==0)return;
cbi(P_SDA, B_SDA);
}


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

        if(PIN==B_RS)sost_RS=0;
        if(PIN==B_SDA)sost_SDA=0;

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

        if(PIN==B_RS)sost_RS=1;
        if(PIN==B_SDA)sost_SDA=1;

        tls_reg_write32(HR_GPIO_DATA_EN + offset, reg_en); // reg_en return

	tls_os_release_critical(cpu_sr); // enable Interrupt

}

//#define pulse_high(reg, bitmask) sbi(reg, bitmask); cbi(reg, bitmask);
//#define pulse_low(reg, bitmask) cbi(reg, bitmask); sbi(reg, bitmask);

static void pulse_low_SCL(void)
{

        if(B_SCL==NO_GPIO_PIN)return;

	u32 cpu_sr = 0;
        u32 reg;
	u32	reg_en;
        u8  pin;
        u16 offset;

        if (B_SCL >= WM_IO_PB_00) // w801 chip, only two GPIO port, PA (GPIOA - 16 bit) and PB (GPIOB - 32 bit), max power 12ma
        {
          pin    = B_SCL - WM_IO_PB_00;
          offset = TLS_IO_AB_OFFSET;
        }
        else
        {
          pin    = B_SCL;
          offset = 0;
        }

	
	cpu_sr = tls_os_set_critical();  // disable Interrupt !!!
	
	reg_en = tls_reg_read32(HR_GPIO_DATA_EN + offset);
	tls_reg_write32(HR_GPIO_DATA_EN + offset, reg_en | (1 << pin)); // enabled control reg from need pin

	reg = tls_reg_read32(HR_GPIO_DATA + offset); // load all pins from port

        tls_reg_write32(HR_GPIO_DATA + offset, reg & (~(1 << pin)));/* write low from pin */

        tls_reg_write32(HR_GPIO_DATA + offset, reg | (1 << pin));/* write Hi from pin */

        tls_reg_write32(HR_GPIO_DATA_EN + offset, reg_en); // reg_en return

	tls_os_release_critical(cpu_sr); // enable Interrupt


}

/*
static void pulse_high(uint32_t* PORT_REG, u8 PIN)
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

        tls_reg_write32(HR_GPIO_DATA + offset, reg | (1 << pin));// write Hi from pin 

        tls_reg_write32(HR_GPIO_DATA + offset, reg & (~(1 << pin)));// write low from pin 

        tls_reg_write32(HR_GPIO_DATA_EN + offset, reg_en); // reg_en return

	tls_os_release_critical(cpu_sr); // enable Interrupt

}
*/

/*
static void pulse_low(uint32_t* PORT_REG, u8 PIN)
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

        tls_reg_write32(HR_GPIO_DATA + offset, reg | (1 << pin));// write Hi from pin 

        tls_reg_write32(HR_GPIO_DATA + offset, reg & (~(1 << pin)));// write low from pin 

        tls_reg_write32(HR_GPIO_DATA_EN + offset, reg_en); // reg_en return

	tls_os_release_critical(cpu_sr); // enable Interrupt

}

*/



