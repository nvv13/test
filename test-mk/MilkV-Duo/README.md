


milkv-duo and milkv-duo 256m front
![photo](jpg/milkv-duo%20front.jpg)

milkv-duo and milkv-duo 256m back
![photo](jpg/milkv-duo%20back.jpg)




источник https://milkv.io/ru/docs/duo/overview


*******************************************************************************
https://milkv.io/ru/docs/duo/getting-started/duo

	Milk-V Duo

SoC	CVITEK CV1800B
	1GHz and 700MHz RISC-V C906 processors
	Integrated CVITEK TPU for smart detection.
	SIP DRAM 64MB
	Supports H.264/H.265 video encoding, up to 2880x1620@20fps.
	Compatible with high-definition CMOS sensors.
	Programmable frequency output for sensor clock.
	Comprehensive ISP features for image optimization.
	Partial OpenCV library support with CV hardware acceleration.
	16-bit audio codec with built-in mic input and output functions.
	Flexible network configurations with 1 Ethernet PHY.

RISC-V CPU	C906@1Ghz (Vector+FPU)  +  C906@700MHz (FPU)
        *насколько я понял, это 2 разных CPU но общей шине,
         1Ghz - с раширениями комманд RISC-V для запуска Linux
         700MHz - используют для RTOS, набор комманд RISC-V - Linux не поддерживает ("из коробки")

MCU	8051@8KB SRAM
        *Подсистема 8051 расположена в модуле, который независимо питается от RTC. 
	Подсистема сконфигурирована с 8051, контроллером I2C/UART/SPI NOR/SD, таймером/WDT,
	управлением прерываниями и mailbox IP (взаимодействие между CPU).
	Системное программное обеспечение может использовать 8051 для управления
	пробуждением системы, когда она находится в спящем режиме.



TPU	0.5TOPS@INT8
         *can be used to accelerate image classification, object detection,
         face detection and recognition, segmentation, LSTM, and more.

Storage	1x слот для MicroSD
        1x паяльная площадка SD NAND

Memory	SIP DRAM 64MB DDR2

USB	1х порт USB type-C for power and data
        1x паяльная площадка USB2

CSI	1 x 16P FPC connector (MIPI CSI 2-lane)


Sensor Support	4M @ 25fps

Ethernet	10/100Mbps ethernet with PHY
         *есть 5-контактная площадка PHY для припайки коннектора "трансформатор и разъем RJ45"

GPIO	up to 26 Pins available for general purpose I/O（GPIO）
        *Интерфейсы ввода/вывода:
	26x GPIO, если на них не сконфигурировано:
	 3x I2C
         5x UART
         1x SPI
	 2x АЦП
         7x PWM
	 1x SDIO
         1x JTAG

Power	5V/1A

OS Support	Buildroot, RTOS

Size	21mm*51mm
*******************************************************************************

*******************************************************************************
https://milkv.io/ru/docs/duo/getting-started/duo256m

	Milk-V  Duo 256M

SoC	SG2002
	1GHz and 700MHz RISC-V C906 processors
	Optional T-Head C906@1GHz or Cortex-A53@1GHz
	Integrated 1TOPS@INT8 TPU for smart detection.
	SIP DRAM 256MB
	Supports H.264/H.265 video encoding, up to 5M@30fps.
	Compatible with high-definition CMOS sensors.
	Support Multiple storage devices via SPI-NOR, SPI-NAND, eMMC5.0, 2 x SDIO3.0 interfaces
	Comprehensive ISP features for image optimization.
	Partial OpenCV library support with CV hardware acceleration.
	16-bit audio codec with built-in mic input and output functions.
	2L MIPI DSI 5M@30fps
	4L or 2L+2L MIPI CSI 5M@30fps
	Flexible network configurations with 1 Ethernet PHY.
	QFN88

RISC-V CPU  C906@1Ghz (Vector+FPU)  +  C906@700MHz (FPU)
        *насколько я понял, это 2 разных CPU но общей шине,
         1Ghz - с раширениями комманд RISC-V для запуска Linux
         700MHz - используют для RTOS, набор комманд RISC-V - Linux не поддерживает ("из коробки")

Arm CPU	1 x Cortex-A53@1GHz (neon+FPU)
	*насколько я понял, при старте MK - проверяеться ножка boot, и исходя из этого
         стартует либо C906@1Ghz либо A53@1GHz - т.е. одновременно они не работают.

MCU	8051@8KB SRAM  (25Mz - 300Mz)
        *Подсистема 8051 расположена в модуле, который независимо питается от RTC. 
	Подсистема сконфигурирована с 8051, контроллером I2C/UART/SPI NOR/SD, таймером/WDT,
	управлением прерываниями и mailbox IP (взаимодействие между CPU).
	Системное программное обеспечение может использовать 8051 для управления
	пробуждением системы, когда она находится в спящем режиме.

TPU	1TOPS@INT8
         *can be used to accelerate image classification, object detection,
         face detection and recognition, segmentation, LSTM, and more.

Storage	1x слот для MicroSD
        1x паяльная площадка SD NAND

Memory	SIP DRAM 256MB DDR2

USB	1х порт USB type-C for power and data
        1x паяльная площадка USB2

CSI	1 x 16P FPC connector (MIPI CSI 2-lane)

Sensor Support	5M @ 30fps

Ethernet	100Mbps ethernet with PHY
         *есть 5-контактная площадка PHY для припайки коннектора "трансформатор и разъем RJ45"

Audio	Via GPIO Pads

GPIO	up to 26 Pins available for general purpose I/O（GPIO）
        *Интерфейсы ввода/вывода:
	26x GPIO, если на них не сконфигурировано:
	 3x I2C
         5x UART
         1x SPI
	 2x АЦП
         7x PWM
	 1x SDIO
         1x JTAG

Power	5V/1A

OS Support	Buildroot, RTOS

Size	21mm*51mm
*******************************************************************************






