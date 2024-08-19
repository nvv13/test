источник
https://community.milkv.io/t/framebuffer-st7789v/578

*******************************************************************************
A little supplement to FrameBuffer driver st7789v

FeOAr   1 Sep 2023


Milkv-duo's supplements on st7789v driver


*Please read this article first to understand the basic process and steps
    https://community.milkv.io/t/milk-v-duo-spi-st7789/131/10


1. Let’s take a look at the hardware pins first
PS: The pins in Figure 1 are labeled as GPIO A, B, and C, which are also the most commonly used;
 the GPIO in Figure 2 refers to the temporary network in the schematic diagram
https://imgforfeoar-1312132618.cos.ap-shanghai.myqcloud.com/Blog_update/202309032222355.png
https://imgforfeoar-1312132618.cos.ap-shanghai.myqcloud.com/Blog_update/202309031554332.png


QFN68	GPIO	Signal name	Pin name	Multiplexed Registers	Attribute Configuration Register	Common Name
38	9	SPI2_CS	SD1_D3	0x0300_108c	0x0502_7038		CS
43	6	SPI2_SCK	SD1_CLK		0x0300_10a0		0x0502_704c				CLK/SCL
41	8	SPI2_DI	SD1_D0	0x0300_1098	0x0502_7044		MISO
42	7	SPI2_DO	SD1_CMD	0x0300_109c	0x0502_7048		MOSI/SDA


https://imgforfeoar-1312132618.cos.ap-shanghai.myqcloud.com/Blog_update/202309031447840.png

2. See what the default looks like
2.1 Uboot
Read the multiplexing register

https://imgforfeoar-1312132618.cos.ap-shanghai.myqcloud.com/Blog_update/202309031506930.png

2.2 Linux
1 Read the multiplex register
https://imgforfeoar-1312132618.cos.ap-shanghai.myqcloud.com/Blog_update/202309031448554.png
2 Take a look at pin excel
https://imgforfeoar-1312132618.cos.ap-shanghai.myqcloud.com/Blog_update/202309031450116.png
3 Looking at the configuration code, there are two places

build/boards/cv180x/cv1800b_milkv_duo_sd/u-boot/cvi_board_init. c
u-boot-2021.10/board/cvitek/cv180x/board. c

Actually, it is called in a file, where cvi_board_init () is configured. The sentence above does not matter whether the comment is opened or not. (P.S. I don't quite understand why pin multiplexing is configured in uboot, isn't this a bare metal boot program, and the kernel operation does not depend on uboot)


3. Modification
First, let’s see where the parameters of PINMUX_CONFIG are defined

Linux_5.10/drivers/pinctrl/cvitek/cv180x_pinlist_swconfig.h

This is the function definition
https://imgforfeoar-1312132618.cos.ap-shanghai.myqcloud.com/Blog_update/202309032156321.png

The predefined macros are as follows:
https://imgforfeoar-1312132618.cos.ap-shanghai.myqcloud.com/Blog_update/202309031514132.png

4. The result is that two parameters are connected by double '_' to form a multiplexed parameter
Here I use the following pins

-----------------------------------------------------------------------------------------------
//build/boards/cv180x/cv1800b_milkv_duo_sd/u-boot/cvi_board_init. c
    // sd1
    PINMUX_CONFIG(SD1_D3, SPI2_CS_X);  //cs
    PINMUX_CONFIG(SD1_D2, PWR_GPIO_19);
    PINMUX_CONFIG(SD1_D1, PWR_GPIO_20);
    PINMUX_CONFIG(SD1_D0, PWR_GPIO_21);
    PINMUX_CONFIG(SD1_CMD, SPI2_SDO); //cmd
    PINMUX_CONFIG(SD1_CLK, SPI2_SCK);  //clk
    // PINMUX_CONFIG(SD1_D3, PWR_GPIO_18);
    // PINMUX_CONFIG(SD1_D2, PWR_GPIO_19);
    // PINMUX_CONFIG(SD1_D1, PWR_GPIO_20);
    // PINMUX_CONFIG(SD1_D0, PWR_GPIO_21);
    // PINMUX_CONFIG(SD1_CMD, PWR_GPIO_22);
    // PINMUX_CONFIG(SD1_CLK, PWR_GPIO_23);

    PINMUX_CONFIG(IIC0_SCL, XGPIOA_28);  //dc 28
    PINMUX_CONFIG(SPK_EN, XGPIOA_15);  //rst 15
-----------------------------------------------------------------------------------------------

5. Device tree modification
-----------------------------------------------------------------------------------------------
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
        rotate = <90>; //Rotate, if I don't rotate it will be vertical
        fps = <60>;
        rgb;
        buswidth = <8>;
        dc = <&porta 28 GPIO_ACTIVE_HIGH>; //Configure the pins for connecting the DC lines, corresponding to the pinmux configuration in the previous section, note that port a 14
        reset = <&porta 15 GPIO_ACTIVE_HIGH>; //Configure the pin connected to the reset line
        debug = <0x0>;
    };
};
-----------------------------------------------------------------------------------------------

Note that the 15th and 16th lines of dc and rst mean port a 28 pin and port a 15 pin. Corresponding to the XGPIOA_28 and XGPIOA_15 defined during multiplexing, the following steps are required to use other pins:

In pin excel (CV180xB_QFN68_PINOUT_V0.1_CN.xlsx), find the GPIO group in the function signal table tab and find an unimportant pin that seems to be unoccupied.

Modify according to the above logic
At this point, multiplexing is completed and the pin attributes remain default

Summary
Statement
I am not familiar with the Linux driver part, including the entire application layer. I only know the basics, so there will inevitably be errors and omissions. Please point them out and make progress together.

Suggestions
After configuring, compiling, packaging and starting, it is recommended to use the serial port to see if there is any fb-related log. If there is, it means that the modification to the kernel has taken effect. After entering the system, first check whether /dev/fb0 appears; then use lsmod to check; then use devmem and other commands to check the register values ​​of the multiplexed pins used, eg. devmem 0x0300108c to see if the value is required. See (2.2) for details. This register needs the value 0x3. If everything is normal, it is generally adjusted.
The configuration of milkv is strange. It is usually configured in the build under the root directory of the SDK, instead of the conventional kernel configuration and make menuconfig under the kernel. The configuration of this board is more like the script reading the custom configuration items after the external definition is completed during compilation, and there are hidden commands, which is too troublesome. It is better to configure it directly outside. I have to say that the brain circuit is clear and simple like the conventional SDK. Just go to whoever you want to configure. At most, it would be better to add a packaging script.
Q&A
Q1: The fb0 node does not appear under /dev
A: The gpio_request in fbtft-core.c is not modified properly

Q2: The fb0 node also exists, and fb_st7789v.ko is also seen in lsmod, but the screen does not respond
A: Check in the order in "Suggestions". If it doesn't work, change the dc or rst pins. I changed them because GPIOA15 was unusable.

Q3: Enter the command cat /dev/zero > /dev/fb0 and an error message appears: cat: write error: No space left on device
A: This error message does not affect the effect. The effect will still be displayed. If the screen is indifferent, it is because the driver is not configured properly, not because of the instruction problem.

