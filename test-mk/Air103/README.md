air103

о чипе
 https://wiki.luatos.com/chips/air103/

~~~
Specific features

CPU   
  32-bit processor, maximum operating frequency 240MHz
Kernel 
  xt804

Flash 
  1MByte, with on-chip file system 

Total Ram 
  288k
Lua available memory 
  176-232KB, default 176KB

serial port
  Support 6-channel UART, one of which is fixed as download and log port
  Baud rate range: 1200bps - 2Mbps
  Data bits: 5-8bit
  Stop bit: 1bit/2bit
  Parity: Configurable
  RTS/CTS flow control: software control only

SPI
  Support 1 SPI interface
  Only supports master

I2C
  Support 1 channel hardware I2C interface
  Rate: 100KHz-400KHz
  Only supports master
  LuatOS software supports multi-channel soft I2C, the speed is lower

ADC
  Support 2 channels of 16-bit ADC, the highest sampling rate is 1KHz
  Support reading core temperature

GPIO
  Supports up to 44 GPIOs

SDIO
  Support 1 channel SDIO interface
  Only supports master
  Only supports SD card reading and writing

PWM  
  Support 5 PWM outputs
  Frequency range: 3Hz - 160KHz
  Duty cycle maximum accuracy: 1/256
  The width of the counter inserted in the dead zone: 8bit

~~~


~~~
GPIO    Pin Mapping Table Air103 Chip
number	name	Default functions and extended functions
0	PA_00	BOOT
1	PA_01	GPIO_01/ADC_1/I2C_SCL
2	PA_02	GPIO_02/ADC_3/PWM_30
3	PA_03	GPIO_03/ADC_2/PWM_31
4	PA_04	GPIO_04/ADC_0/I2C_SDA
5	PA_05	GPIO_05
6	PA_06	GPIO_06
7	PA_07	GPIO_07/ PWM_04
8	PA_08	GPIO_08/UART4_TX
9	PA_09	GPIO_09/UART4_RX
10	PA_10	GPIO_10/ PWM_10
11	PA_11	GPIO_11/PWM_11
12	PA_12	GPIO_12/ UART5_TX/ PWM_12
13	PA_13	GPIO_13/ UART5_RX/ PWM_13
14	PA_14	GPIO_14/ PWM_14
15	PA_15	GPIO_15/PSRAM_CLK
16	PB_00	GPIO_16/PWM_00/UART3_TX
17	PB_01	GPIO_17/PWM_01/UART3_RX
18	PB_02	GPIO_18/UART2_TX/PSRAM_D0/SPI0_CLK/PWM_02
19	PB_03	GPIO_19/UART2_RX/PSRAM_D1/SPI0_MISO/PWM_03
20	PB_04	GPIO_20/PSRAM_D2/SPI0_CS
21	PB_05	GPIO_21/PSRAM_D3/SPI0_MOSI
22	PB_06	GPIO_22/UART1_TX/SDIO_CLK
23	PB_07	GPIO_23/UART1_RX/SDIO_CMD
24	PB_08	GPIO_24/SDIO_D0
25	PB_09	GPIO_25/SDIO_D1
26	PB_10	GPIO_26/SDIO_D2
27	PB_11	GPIO_27/SDIO_D3
28	PB_12	GPIO_28/PWM_20
29	PB_13	GPIO_29/PWM_21
30	PB_14	GPIO_30/SPI1_CS/PWM_22
31	PB_15	GPIO_31/SPI1_CLK/PWM_23
32	PB_16	GPIO_32/SPI1_MISO/PWM_24
33	PB_17	GPIO_33/SPI1_MOSI
34	PB_18	GPIO_34
35	PB_19	UART0_TX
36	PB_20	UART0_RX
37	PB_21	GPIO_37
38	PB_22	GPIO_38
40	PB_24	GPIO_40/ PWM_32
41	PB_25	GPIO_41/ PWM_33
42	PB_26	GPIO_42/PWM_34
43	PB_27	GPIO_43/PSRAM_CS


BOOT When starting up, only the and are configured UART0_TX/RX,
other digital pins are GPIO pins, and the status is input high impedance.

ADC number (LuatOS)	Function
0		Module ADC0-PA1
1		Module ADC1-PA4
2		Module ADC2-PA3
3		Module ADC3-PA2
adc.CH_CPU	CPU temperature
adc.CH_VBAT	internal voltage




https://wiki.luatos.com/chips/air103/hardware.html#id5

RF pin (?) 
 As a hidden feature, the RF pins of Air103 are: 14
 When designing PCB, this pin must be protected against ESD!!!
 Functionally supports BLE 4.2 and wifi 2.4G communication, but does not support wifi 5G


https://wiki.luatos.com/_static/bom/Air103.html

~~~

о luatos для чипа Air101/Air103 (+ еще W800/W801/W806)
 https://gitee.com/openLuat/luatos-soc-air101

о компиляции luatos, если нужно, так то есть готовые образы прошивки для Air101/Air103
 https://wiki.luatos.com/develop/compile/Air101.html
  установка  xmake
   install xmake https://xmake.io/#/getting_started
  для Linux
   bash <(wget https://xmake.io/shget.text -O -)


готовые образы прошивки для Air101/Air103, ищем тут
 https://gitee.com/openLuat/LuatOS/releases
 
какие то демо скрипты lua для платки (с установленой LuatOS)
 https://github.com/monsterLang/LuatOS/tree/master/air101


про язык Lua
 https://ru.wikipedia.org/wiki/Lua
Learn Lua in 15 Minutes
 https://tylerneylon.com/a/learn-lua/


как прошить flash через COM порт
 https://wiki.luatos.com/boardGuide/flash.html


о платке
 https://wiki.luatos.com/chips/air103/board.html

