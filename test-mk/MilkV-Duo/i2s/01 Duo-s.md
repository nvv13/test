Milk-V Duo S - How to use audio?
[источник](https://community.milkv.io/t/milk-v-duo-s-how-to-use-audio/1731)



emeb
May 2024
Got a Duo S today. 
Installed the Duo S SD V1.1.0 image from the github site and booted it up. 
The on-chip codec drivers are already in the kernel
 but ALSA utils & libs are not included in the buildroot configuration. 
I copied the utils, libs and shared data over from another system
 I built earlier and it all worked fine from the dedicated audio I/O pins on the J4 header.
                                                           ( AOUT_L , AOUT_R ) 


emeb
May 2024
Actually, in the first reply I overestimated the effort required -
 I didn’t have to rebuild the kernel to enable the audio drivers -
 the audio ADC and DAC were already in the kernel and merely needed the proper ALSA userspace utilities to feed them.
 I grabbed some existing alsa stuff (cmd line progs, libs & shared data) from a buildroot I’d done earlier,
 copied them into the right places in the SD card filesystem and things “just worked”.
 If we had a nice package system that would have been super easy to bring up but as it stood
 I just unzipped some files into the proper directories.




подробнее об выводах есть
в экселе SG2000-PINOUT.xlsx
по ссылке [SG2000-PINOUT.xlsx](https://github.com/milkv-duo/duo-files/blob/main/duo-s/hardware/SG2000-PINOUT.xlsx?raw=true)

~~~



PAD_AUD_AOUTL	"AUDIO GPIO (1.8V)" FMUX_GPIO_REG_IOCTRL_PAD_AUD_AOUTL

0x0300_11C4"	0x3	"IO PAD_AUD_AOUTL function select : 
    3 : XGPIOC[25] (default)
    4 : IIS1_LRCK 
    5 : IIS2_LRCK 
    Others : Reserved"



PAD_AUD_AOUTR	"AUDIO GPIO (1.8V)" FMUX_GPIO_REG_IOCTRL_PAD_AUD_AOUTR

0x0300_11C8"	0x3	"IO PAD_AUD_AOUTR function select : 
    3 : XGPIOC[24] (default)
    4 : IIS1_DI 
    5 : IIS2_DO 
    6 : IIS1_DO 
    Others : Reserved"



~~~

![photo](../jpg/duos-pinout.jpg)

