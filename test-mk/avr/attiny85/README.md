просто из даташита



Atmel 8-bit AVR Microcontroller with 2/4/8K
Bytes In-System Programmable Flash

[ATtiny25/V / ATtiny45/V / ATtiny85/V](https://static.chipdip.ru/lib/696/DOC011696762.pdf)

[datasheet](https://static.chipdip.ru/lib/070/DOC027070938.pdf)

[tech datasheet](https://static.chipdip.ru/lib/542/DOC003542649.pdf)


~~~
Features
• High Performance, Low Power AVR® 8-Bit Microcontroller
• Advanced RISC Architecture
– 120 Powerful Instructions – Most Single Clock Cycle Execution
– 32 x 8 General Purpose Working Registers
– Fully Static Operation
• Non-volatile Program and Data Memories
– 2/4/8K Bytes of In-System Programmable Program Memory Flash
• Endurance: 10,000 Write/Erase Cycles
– 128/256/512 Bytes In-System Programmable EEPROM
• Endurance: 100,000 Write/Erase Cycles
– 128/256/512 Bytes Internal SRAM
– Programming Lock for Self-Programming Flash Program and EEPROM Data Security
• Peripheral Features
– 8-bit Timer/Counter with Prescaler and Two PWM Channels
– 8-bit High Speed Timer/Counter with Separate Prescaler
• 2 High Frequency PWM Outputs with Separate Output Compare Registers
• Programmable Dead Time Generator
– USI – Universal Serial Interface with Start Condition Detector
– 10-bit ADC
• 4 Single Ended Channels
• 2 Differential ADC Channel Pairs with Programmable Gain (1x, 20x)
• Temperature Measurement
– Programmable Watchdog Timer with Separate On-chip Oscillator
– On-chip Analog Comparator
• Special Microcontroller Features
– debugWIRE On-chip Debug System
– In-System Programmable via SPI Port
– External and Internal Interrupt Sources
– Low Power Idle, ADC Noise Reduction, and Power-down Modes
– Enhanced Power-on Reset Circuit
– Programmable Brown-out Detection Circuit
– Internal Calibrated Oscillator
• I/O and Packages
– Six Programmable I/O Lines
– 8-pin PDIP, 8-pin SOIC, 20-pad QFN/MLF, and 8-pin TSSOP (only ATtiny45/V)
• Operating Voltage
– 1.8 - 5.5V for ATtiny25V/45V/85V
– 2.7 - 5.5V for ATtiny25/45/85
• Speed Grade
– ATtiny25V/45V/85V: 0 – 4 MHz @ 1.8 - 5.5V, 0 - 10 MHz @ 2.7 - 5.5V
– ATtiny25/45/85: 0 – 10 MHz @ 2.7 - 5.5V, 0 - 20 MHz @ 4.5 - 5.5V
• Industrial Temperature Range
• Low Power Consumption
– Active Mode:
• 1 MHz, 1.8V: 300 µA
– Power-down Mode:
• 0.1 µA at 1.8V
~~~

~~~
             Pinout of ATtiny85 8-PDIP/SOIJ/SOIC
***************************************************************************************
       (PCINT5/RESET/ADC0/dW) PB5 1      8 VCC
(PCINT3/XTAL1/CLKI/OC1B/ADC3) PB3 2      7 PB2 (SCK/USCK/SCL/ADC1/T0/INT0/PCINT2)
(PCINT4/XTAL2/CLKO/OC1B/ADC2) PB4 3      6 PB1 (MISO/DO/AIN1/OC0B/OC0B/PCINT1)
                       	      GND 4      5 PB0 (MOSI/DI/SDA/AIN0/OC0A/OC1A/AREF/PCINT0)
***************************************************************************************

~~~

