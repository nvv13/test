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

Платка
https://wiki.luatos.com/_static/bom/Air103.html
 PA5 PA6 PB21 PB22 похоже ни куда не присоединены
 PB24 PB25 PB26 присоденены к LED на платке D3 D1 D2

Загадка
https://wiki.luatos.com/chips/air103/hardware.html#id5
 RF pin (?) 
	As a hidden feature, the RF pins of Air103 are: 14
	When designing PCB, this pin must be protected against ESD!!!
	Functionally supports BLE 4.2 and wifi 2.4G communication, but does not support wifi 5G

~~~

о luatos для чипа Air101/Air103 (+ еще W800/W801/W806)
 https://gitee.com/openLuat/luatos-soc-air101
 https://wiki.luatos.com/chips/air103/hardware.html

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

