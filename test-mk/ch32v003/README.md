
Платка CH32V003 или её еще просто обзывают TWen32F003 на МК CH32V003F4P6
их несколько вариантов

мне попалась с кнопкой и двумя светодиодами - зелёным и красным,
   кнопка подсоединена к питанию и выводу PC0
   красный светиться когда загружаешь прошивку, к выводу SWD подсоединён
   зелёный - к земле и питанию (через резистор)


<p><img src="https://github.com/nvv13/test/blob/main/test-mk/ch32v003/doc/TWen32F003.jpg" alt="TWen32F003" title="CH32V003" /></p>


~~~
https://www.wch-ic.com/products/CH32V003.html

CH32V003F4P6 корпус TSSOP20
 QingKe 32-bit RISC-V2A processor, supporting 2 levels of interrupt nesting
 Maximum 48MHz system main frequency
 2KB SRAM, 16KB Flash
 Power supply voltage: 3.3/5V
 Multiple low-power modes: Sleep, Standby
 Power on/off reset, programmable voltage detector
 1 group of 1-channel general-purpose DMA controller
 1 group of op-amp comparator
 1 group of 10-bit ADC
 1×16-bit advanced-control timer, 1×16-bit general-purpose timer
 2 WDOG, 1×32-bit SysTick
 1 USART interface, 1 group of I2C interface, 1 group of SPI interface
 18 I/O ports, mapping an external interrupt
 64-bit chip unique ID
 1-wire serial debug interface (SDI)
~~~






~~~

напоминалка:

делал в fedora linux 39, в других возможно немного по другому надо


(1) установка пакетов для сборки riskv кода

# sudo dnf install pnpm

# sudo npm install --location=global xpm@latest

# xpm install @xpack-dev-tools/riscv-none-elf-gcc@latest --global --verbose

# export PATH=$HOME/.local/xPacks/@xpack-dev-tools/riscv-none-elf-gcc/14.2.0-2.1/.content/bin:$PATH



(2) утилита для прошивки через WCH-linkE (у меня версия WCH-linkE-R0-1v3.FP)

собираем из проекта 

https://github.com/cnlohr/ch32v003fun.git

minichlink

копируем оттуда файл
`99-minichlink.rules`
в
`/etc/udev/rules.d/`

$ sudo groupadd plugdev
$ sudo usermod -a -G plugdev cur_user
$ sudo reboot

(3)
 


~~~


PS:
 источники
https://github.com/openwch/ch32v003
https://github.com/cnlohr/ch32v003fun
https://github.com/wuxx/CH32V003-makefile-example
https://github.com/gregdavill/CH32V307-makefile-example


# Datasheet and Reference and Core Manual
### Download URL

- CH32V003RM.PDF-http://www.wch-ic.com/downloads/CH32V003RM_PDF.html
- CH32V003DS0.PDF-http://www.wch-ic.com/downloads/CH32V003DS0_PDF.html
- QingKeV2_Processor_Manual.PDF-http://www.wch-ic.com/downloads/QingKeV2_Processor_Manual_PDF.html

