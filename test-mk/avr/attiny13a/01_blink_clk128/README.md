https://blog.podkalicki.com/how-to-make-attiny13-running-at-128-khz-clock-source/
https://blog.podkalicki.com/how-to-update-avr-usbasp-firmware-to-latest-version/


 $ avr-gcc -std=c99 -Wall -g -Os -mmcu=attiny13 -DF_CPU=128000 -I. -o main.o main.c
 $ avr-objcopy -j .text -j .data -O ihex main.o main.hex
 $ avrdude -p attiny13 -c usbasp -B 1024 -U flash:w:main.hex:i -F -P usb

AVR® Fuse Calculator
  https://www.engbedded.com/fusecalc/

Setting Fuse Bits
 $ avrdude -p attiny13 -c usbasp -U hfuse:w:0xFF:m -U lfuse:w:0x7B:m



















sudo dnf install avr-gcc avr-libc avrdude
https://www.microchip.com/en-us/product/ATtiny13A#

ATtiny13A
The Microchip picoPower® 8-bit AVR® RISC-based microcontroller features
 1 KB of ISP Flash,
 64B EEPROM,
 64B SRAM,
 32B register file,
 and 4-channel 10-bit A/D converter.
 The device achieves up to 20 MIPS throughput at 20 MHz 
 at 1.8-5.5V operation.

By executing powerful instructions in a single clock cycle, 
 the ATtiny13A achieves throughputs approaching one MIPS per MHz 
 allowing you to optimize power consumption versus processing speed.



             Pinout of ATtiny13A 8-PDIP/SOIJ/SOIC
*********************************************************************
(PCINT5/RESET/ADC0/dW) PB5 1      8 VCC
    (PCINT3/CLKI/ADC3) PB3 2      7 PB2 (SCK/ADC1/T0/PCINT2)
         (PCINT4/ADC2) PB4 3      6 PB1 (MISO/AIN1/OC0B/INT0/PCINT1)
                       GND 4      5 PB0 (MOSI/AIN0/OC0A/PCINT0)
*********************************************************************







