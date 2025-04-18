источник

https://community.milkv.io/t/milk-v-duo-spi-st7789/131
(что то похожее есть в https://github.com/milkv-duo/duo-lvgl-fb-demo.git)

firstman 8 Jun 2023

I wanted to wait for the master to post a tutorial on how to light up the screen,
 but I found that there was no relevant information.
 I had no choice but to do it myself

First of all, I would like to state that I am a novice.
 The following tutorial is based on my own research.
 Since it is my first time to use a Linux development board,
 I spent a lot of time trying to succeed.
 If there are any misleading or wrong parts in the following tutorial, please be merciful.

****************************************************************

----------------------------------------st7789 driver----------------------------------------------------------------------
1. Modify the st7789 driver
The official SDK is the Linux 5.10 kernel. You can use the driver in fbtft and directly modify the st7789v driver to use it
The following are the directory files that need to be modified:

linux_5.10/drivers/staging/fbtft/fb_st7789v.c

(1) Modify init_display
 (the screen initialization method can usually be found in the official example of the screen.
  Just follow the STM32 code. For st7789, just use the following code. For other SPI screens,
  refer to other materials for modification)

---------------------------------fb_st7789v.c---------------------
static int init_display(struct fbtft_par *par)
{
    par->fbtftops.reset(par);
    mdelay(50);
    write_reg(par,0x36,0x00);
    write_reg(par,0x3A,0x05);
    write_reg(par,0xB2,0x0C,0x0C,0x00,0x33,0x33);
    write_reg(par,0xB7,0x35);
    write_reg(par,0xBB,0x19);
    write_reg(par,0xC0,0x2C);
    write_reg(par,0xC2,0x01);
    write_reg(par,0xC3,0x12);
    write_reg(par,0xC4,0x20);
    write_reg(par,0xC6,0x0F);
    write_reg(par,0xD0,0xA4,0xA1);
    write_reg(par,0xE0,0xD0,0x04,0x0D,0x11,0x13,0x2B,0x3F,0x54,0x4C,0x18,0x0D,0x0B,0x1F,0x23);
    write_reg(par,0xE1,0xD0,0x04,0x0C,0x11,0x13,0x2C,0x3F,0x44,0x51,0x2F,0x1F,0x1F,0x20,0x23);
    write_reg(par,0x21);
    write_reg(par,0x11);
    mdelay(50);
    write_reg(par,0x29);
    mdelay(200);
    return 0;
}
---------------------------------fb_st7789v.c---------------------

(2)Modify fbtft_display
 (if your screen height and width parameters are inconsistent, please modify)
---------------------------------fb_st7789v.c---------------------
static struct fbtft_display display = {
	.regwidth = 8,
	.width = 240, //
	.height = 320, //
	.gamma_num = 2,
	.gamma_len = 14,
	.gamma = HSD20_IPS_GAMMA, // You can use the default
	.fbtftops = {
		.init_display = init_display,
		.set_var = set_var,
		.set_gamma = set_gamma,
		.blank = blank,
	},
};
---------------------------------fb_st7789v.c---------------------

linux_5.10/drivers/staging/fbtft/fbtft-core.c

(1)Add include

---------------------------------fbtft-core.c---------------------
#include <linux/gpio.h> //add
#include <linux/of_gpio.h> //add
---------------------------------fbtft-core.c---------------------

(2)Modify fbtft_request_one_gpio

---------------------------------fbtft-core.c---------------------
static int fbtft_request_one_gpio(struct fbtft_par *par,
                  const char *name, int index,
                  struct gpio_desc **gpiop)
{
    struct device *dev = par->info->device;
    struct device_node *node = dev->of_node;
    int gpio, flags, ret = 0;
    enum of_gpio_flags of_flags;

    if (of_find_property(node, name, NULL)) {
        gpio = of_get_named_gpio_flags(node, name, index, &of_flags);
        if (gpio == -ENOENT)
            return 0;
        if (gpio == -EPROBE_DEFER)
            return gpio;
        if (gpio < 0) {
            dev_err(dev,
                "failed to get '%s' from DT\n", name);
            return gpio;
        }

         //active low translates to initially low 
        flags = (of_flags & OF_GPIO_ACTIVE_LOW) ? GPIOF_OUT_INIT_LOW :
                            GPIOF_OUT_INIT_HIGH;
        ret = devm_gpio_request_one(dev, gpio, flags,
                        dev->driver->name);
        if (ret) {
            dev_err(dev,
                "gpio_request_one('%s'=%d) failed with %d\n",
                name, gpio, ret);
            return ret;
        }

        *gpiop = gpio_to_desc(gpio);
        fbtft_par_dbg(DEBUG_REQUEST_GPIOS, par, "%s: '%s' = GPIO%d\n",
                            __func__, name, gpio);
    }

    return ret;
}
---------------------------------fbtft-core.c---------------------

(3)Modify fbtft_reset (I am not sure if this step is necessary)

---------------------------------fbtft-core.c---------------------
static void fbtft_reset(struct fbtft_par *par)
{
	if (!par->gpio.reset)
		return;
	fbtft_par_dbg(DEBUG_RESET, par, "%s()\n", __func__);
	gpiod_set_value_cansleep(par->gpio.reset, 1);
	msleep(10);
	gpiod_set_value_cansleep(par->gpio.reset, 0);
	msleep(200);
	gpiod_set_value_cansleep(par->gpio.reset, 1);
	msleep(10);
}
---------------------------------fbtft-core.c---------------------

The above modification of the st7789 driver part is completed.
  Of course, some people may ask whether the quoted parameters
 in fbtft_request_gpios do not need to add -gpios.
 My answer is, no, there is no problem matching in the device tree.

----------------------------------------------pinmux----------------------------------------------------------------
2. It is very important to modify pinmux
  All the pin configurations of Duo are here.
 For the specific pin definition functions and default pin configurations, please refer to:
   CV181x/CV180x HDK Development Document Summary (Hardware Development Document) 
    https://developer.sophgo.com/thread/472.html

   The CV180xB_QFN68_PINOUT_V0.1_CN.xlsx file in it
   Corresponding to the pin position on Duo:
    https://milkv.io/files/duo/duo-schematic-v1.1.pdf


Next we start to modify pinmux

u-boot-2021.10/board/cvitek/cv180x/board.c

---------------------------------board.c---------------------
pinmux_config(PINMUX_SPI2); //SDK comments this out by default, uncomment it.
PINMUX_CONFIG(SPINOR_CS_X, XGPIOA_24); //Added, the default pin function here is not GPIO, modify it
PINMUX_CONFIG(SPINOR_MISO, XGPIOA_23); //Add, same here
cvi_board_init();
---------------------------------board.c---------------------


The above completes the modification of pinmux. In fact, in addition to modifying board.c, you can also modify

build/boards/cv180x/cv1800b_sophpi_duo_sd/u-boot/cvi_board_init.c

I won’t write the specific method, everyone is so smart that they will definitely
figure it out. By the way, I will change the dc and reset pins that are going to
 connect to SPI to GPIOA24 and GPIOA23, refer to the red frame pins in the figure:
 https://community.milkv.io/uploads/default/original/1X/69d41de6236905e1328c943d34f90a6a8ca3313f.jpeg

--------------------------------------------------------------------------------------------------------------
3. Modify the device tree
This is also very important. There is a pitfall here. Let's start!

build/boards/cv180x/cv1800b_sophpi_duo_sd/dts_riscv/cv1800b_sophpi_duo_sd.dts

You need to add a piece of code to this file

------------------------------------cv1800b_sophpi_duo_sd.dts----------------------------------------------------------------
&spi2 {
	status = "okay";
	/delete-node/ spidev@0; //Two options, either write this or change the reg in st7789
	st7789v: st7789v@0{
		compatible = "sitronix,st7789v"; //Driver Name
		reg = <0>;
		status = "okay";
		spi-max-frequency = <48000000>;
		spi-cpol;
		spi-cpha;
		rotate = <0>; //Rotate, if I don't rotate it will be vertical
		fps = <60>;
		rgb;
		buswidth = <8>;
		dc = <&porta 24 GPIO_ACTIVE_HIGH>; //Configure the pins connected to the DC line, corresponding to the pinmux configuration in the previous section
		reset = <&porta 23 GPIO_ACTIVE_HIGH>; //Configure the pin connected to the reset line
		debug = <0x0>;
	};
};
----------------------------------------cv1800b_sophpi_duo_sd.dts------------------------------------------------------------


build/boards/default/dts/cv180x/cv180x_base.dtsi

Modify spi2: spi2@041A0000. This is very important. The pit is here.
 If I don’t add it, I can’t drive the screen no matter what I do.

--------------------------------cv180x_base.dtsi------------------------------------------------------------------------------
spi2:spi2@041A0000 {
		compatible = "snps,dw-apb-ssi";
		reg = <0x0 0x041A0000 0x0 0x10000>;
		clocks = <&clk CV180X_CLK_SPI>;
		#address-cells = <1>;
		#size-cells = <0>;
		bias-pull-up; //Add this place to pull up
	};
----------------------------------cv180x_base.dtsi----------------------------------------------------------------------------
The device tree is now configured. Be careful not to step into the pit

------------------------------------------kernel SPI--------------------------------------------------------------------

4. Add kernel SPI configuration and st7789 driver
There are two ways to modify it.

(1)After starting the compilation environment, enter menuconfig_kernel
 to enter the kernel graphics configuration to set it.
 I was confused by this, so I won’t use it here. But there is a small tip.
 Press the "/" key to search for methods in the interface. It is very useful.
 The output console configuration later depends on it.

(2)Add it directly to .config or cvitek_cv1800b_sophpi_duo_sd_defconfig
 (refer to the forum mollysama for SPI kernel configuration)

linux_5.10/build/cv1800b_sophpi_duo_sd/.config
or
build/boards/cv180x/cv1800b_sophpi_duo_sd/linux/cvitek_cv1800b_sophpi_duo_sd_defconfig

--------------------------------------------------------------------------------------------------------------
CONFIG_SPI=y
CONFIG_SPI_MASTER=y
CONFIG_SPI_DESIGNWARE=y
CONFIG_SPI_DW_MMIO=y
CONFIG_SPI_SPIDEV=y
CONFIG_FB=y
CONFIG_FB_TFT=y
CONFIG_FB_TFT_ST7789V=y
--------------------------------------------------------------------------------------------------------------


-------------------------test-------------------------------------------------------------------------------------
5. Compile, connect, and test
(1) I won’t explain much about compilation, just check out other big guys’ posts

--------------------------------------------------------------------------------------------------------------
source build/cvisetup.sh
defconfig cv1800b_sophpi_duo_sd
build_all
pack_sd_image
--------------------------------------------------------------------------------------------------------------

(2) Let me explain the connection.
 The previous pinmux configuration uses the hardware SPI method.
 For the defined pins, please refer to the pdf file placed when modifying the pinmux above.
 The following is my connection configuration

--------------------------------------------------------------------------------------------------------------
Duo       |       st7789
GND       |         GND
3.3v(out) |         VCC
GPIO23    |         SCL
GPIO22    |         SDA
GPIOA23   |         RES
GPIOA24   |         DC
GPIO18    |         CS
--------------------------------------------------------------------------------------------------------------

(3) Connect the cables, burn the SD card, plug it into the duo, plug in the USB to power it up, and execute the following code test

--------------------------------------------------------------------------------------------------------------
cat /dev/urandom > /dev/fb0  //Display screen
cat /dev/zero > /dev/fb0  //Clear the screen, that is, black screen
--------------------------------------------------------------------------------------------------------------
If you have performed the above operations correctly,
  and the first line can display the screen normally during the test,
 it means that the SPI driver st7789 is 100% successful.
 However, you will find that the first line of screen pixels displayed by
  echo xxxx > /dev/fb0 is a few pixels.
 This is not a problem. In fact, it is normal for you to write a program to fill the pixels on the screen.


-----------------------------------need to console?---------------------------------------------------------------------------

6. The screen output is displayed normally, outputs kernel loading information and displays the console
kernel.config settings
--------------------------------------------------------------------------------------------------------------
CONFIG_TTY=y
CONFIG_VT=y
CONFIG_CONSOLE_TRANSLATIONS=y
CONFIG_VT_CONSOLE=y
CONFIG_HW_CONSOLE=y
CONFIG_VT_HW_CONSOLE_BINDING=y
CONFIG_FB=y
CONFIG_FB_CMDLINE=y
CONFIG_FB_NOTIFY=y
CONFIG_FONT_SUPPORT=y
CONFIG_FONTS=y
CONFIG_FONT_8x16=y
CONFIG_VGA_CONSOLE=y
CONFIG_DUMMY_CONSOLE=y
CONFIG_DUMMY_CONSOLE_COLUMNS=80
CONFIG_DUMMY_CONSOLE_ROWS=25
CONFIG_FRAMEBUFFER_CONSOLE=y
CONFIG_FRAMEBUFFER_CONSOLE_DETECT_PRIMARY=y
--------------------------------------------------------------------------------------------------------------

Related kernel options can be found in the kernel configuration graphical interface.
 If you want to look up the detailed explanation of various configurations,
 you can find them in the Kconfig files in each directory.

Finally, you need to achieve the effect shown in the figure below.
 After the screen driver is loaded, the kernel loading information is output.
 You need to add the startup parameter console=tty0 in uboot.
In fact, you can also add it in the /u-boot-2021.10/include/configs/cv180x-asic.h file before compiling.
 As for the setenv bootargs saveenv method, you have to enable the corresponding command in uboot.

over!

https://community.milkv.io/uploads/default/original/1X/e415baa3d425bc0df83894560bda7204365f2255.jpeg

--------------------------------------------------------------------------------------------------------------

PS: see coments https://community.milkv.io/t/milk-v-duo-spi-st7789/131

