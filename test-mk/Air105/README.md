air105

о чипе
 https://wiki.luatos.com/chips/air105/hardware.html
 https://developpaper.com/aizhou-air105-uses-keil-mdk-dap-link-to-burn-and-debug/
~~~
Specific features

ARM SecurCore ™ SC300 ™ core
 32-bit RISC Core(ARMv7-M)
 MPU memory protection unit
 Up to 204mhz main frequency (1 and 2 frequency division adjustable)
 FPU unit
1 controlled jtag-dp / sw-dp debugging port
640KB SRAM
1 QSPI controller supporting XIP
System control module (control all peripheral module clocks and system related configurations)
Secure encryption algorithm acceleration engine
 Symmetry algorithm: DES, TDES, AES-128 / 192 / 256
 Asymmetric algorithm: rsa-1024 / 2048, ECC
 Hash verification algorithm: SHA-1 / 224 / 256 / 384 / 512
2-way smartcard interface (supporting EMV level-1 protocol specification and iso7816-3 standard),
 in which sci0 integrates 7816 level conversion function and can be configured to output 3V and 1.8V
4-way UART interface (all support 4 lines)
3-way SPI interface (spi0 master-slave can be configured, and the other 2 ways only support Master)
 * 1-way high-speed SPI master interface SPI5
1-way IIC interface
1-way KBD (4×5 matrix keyboard)
8 32-bit timers (with PWM function, supporting single cycle output)
1-way LCDI interface, supporting 8080 and 6800 bus protocols
1 true random number generator
1 DMA controller (supporting 8-channel DMA transmission)
1 CRC module (supporting 16bit / 32bit and various common polynomial calculations)
Supports up to 87 gpios
Up to 8 static tampers or 4 groups of dynamic tampers (4 outputs, 4 inputs) are supported,
 and dynamic / static can be configured
1 set of internal sensors (supporting high and low voltage, high and low temperature, mesh,
              clock and voltage glitch detection)
1 key storage area (supporting fast hardware erasure)
1 USB (otg-fs)
Support USB2 0 and otg1 0a
Built in USB PHY module
Dedicated DMA channel and proprietary interrupt vector to speed up data communication
Integrated internal watchdog
1 10 bit DAC interface
One 7-channel 12bit ADC, with a maximum sampling rate of 857khz
 (0-channel fixed acquisition charge_vbat voltage,
  and the acquisition voltage range of other channels is 0 ~ 1.8V or 0 ~ 3.6V, which can be configured)
It supports magnetic stripe decoding function and standard cards such as ISO / ABA, aamva, IBM and JIS II
1 DCMI interface
The chip integrates an LDO with external output of 150mA driving capability
The chip integrates USB charging management module and supports a maximum charging current of 200mA
Chip integrated switching function
Support 27.12m output
~~~




о luatos для чипа Air105
 https://github.com/openLuat/LuatOS
 https://gitee.com/openLuat/luatos-soc-air105/tree/master/bsp/air105
 https://gitee.com/openLuat/luatos-soc-air105/tree/master

о компиляции luatos, если нужно, так то есть готовые образы прошивки для Air105
 https://wiki.luatos.com/develop/compile/Air105.html
  установка  xmake
   install xmake https://xmake.io/#/getting_started
  для Linux
   bash <(wget https://xmake.io/shget.text -O -)

готовые образы прошивки для Air105, ищем тут
 https://gitee.com/openLuat/LuatOS/releases
 
какие то демо скрипты lua для платки (с установленой LuatOS)
 https://github.com/monsterLang/LuatOS/tree/master/air105

про язык Lua
 https://ru.wikipedia.org/wiki/Lua
Learn Lua in 15 Minutes
 https://tylerneylon.com/a/learn-lua/

как прошить flash через COM порт
 https://wiki.luatos.com/boardGuide/flash.html


схема
 https://cdn.openluat-luatcommunity.openluat.com/attachment/20220110133139675_CORE-Air105-V1.1.pdf

подключение LCD
 https://chowdera.com/2022/128/202205090701216080.html
 https://chowdera.com/2022/02/202202191545002173.html

подключение клавиатуры 4х4
 https://icode.best/i/99341545380595

о платке
 https://wiki.luatos.com/chips/air105/board.html

