air101

о чипе
 https://wiki.luatos.com/chips/air101/

~~~
Specific features

CPU   
  32-bit processor, maximum operating frequency 240MHz

Flash 
  2MByte, with on-chip file system

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
  Support 1 channel hardware I2C interface
  Rate: 100KHz-400KHz

I2C
  Only supports master
  LuatOS software supports multi-channel soft I2C, the speed is lower

ADC
  Support 2 channels of 16-bit ADC, the highest sampling rate is 1KHz

GPIO
  Support reading core temperature
  Supports up to 18 GPIOs
  Support 1 channel SDIO interface

SDIO
  Only supports master
  Only supports SD card reading and writing
  Support 5 PWM outputs
  Frequency range: 3Hz - 160KHz

PWM  Duty cycle maximum accuracy: 1/256
     The width of the counter inserted in the dead zone: 8bit

~~~




о luatos для чипа Air101/Air103
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

