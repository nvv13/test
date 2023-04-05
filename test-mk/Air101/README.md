air101

о чипе
 https://wiki.luatos.com/chips/air101/

~~~
Specific features

CPU   
  32-bit processor, maximum operating frequency 240MHz

Flash 
  2MByte, with on-chip file system (*air103 1MByte)

Lua available memory 
  176-232KB, default 176KB

serial port
  Support 5-channel UART, one of which is fixed as download and log port
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
  Supports up to 18 GPIOs

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
	Pin Mapping Table 
GPIO number	name	Default and extended functions
0		PA0	BOOT
1		PA1	I2C_SCL/ADC0
4		PA4	I2C_SDA/ADC1
7		PA7	GPIO/PWM4
16		PB0	GPIO/PWM0/UART3_TX
17		PB1	GPIO/PWM1/UART3_RX
18		PB2	SPI_SCK/PWM2/UART2_TX
19		PB3	SPI_MISO/PWM3/UART2_RX
20		PB4	SPI_CS/UART4_TX
21		PB5	SPI_MOSI/UART4_RX
22		PB6	UART1_TX
23		PB7	UART1_RX
24		PB8	GPIO
25		PB9	GPIO
26		PB10	GPIO
27		PB11	GPIO
35		PB19	UART0_TX
36		PB20	UART0_RX
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


схема
 https://cdn.openluat-luatcommunity.openluat.com/attachment/20210910182802476_Air101-Board_v1.0.pdf


о платке
 https://wiki.luatos.com/chips/air101/board.html

