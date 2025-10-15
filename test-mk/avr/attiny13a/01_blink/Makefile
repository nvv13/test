MCU=attiny13
F_CPU=1200000  # 9600000 / 8 = 1200000
HFUSE=0xFF # 0xFF - default fuse, see https://www.engbedded.com/fusecalc/
LFUSE=0x6A # 0x6A - default fuse, Int. RC Osc. 9.6 MHz + Divide clock by 8 internally;
CC=avr-gcc
OBJCOPY=avr-objcopy
SIZE=avr-size
AVRDUDE=avrdude
CFLAGS=-std=c99 -Wall -g -Os -mmcu=${MCU} -DF_CPU=${F_CPU} -I.
TARGET=main
SRCS=main.c 
	
all:
	${CC} ${CFLAGS} -o ${TARGET}.bin ${SRCS}
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.bin ${TARGET}.hex	
	${SIZE} -C --mcu=${MCU} ${TARGET}.bin
	
flash:
	${AVRDUDE} -p ${MCU} -c usbasp -B 125kHz -U flash:w:${TARGET}.hex:i -F -P usb

writefuse:
	${AVRDUDE} -p ${MCU} -c usbasp -B 125kHz -U hfuse:w:${HFUSE}:m -U lfuse:w:${LFUSE}:m

readfuse:
	${AVRDUDE} -p ${MCU} -c usbasp -B 125kHz -U lfuse:r:low_fuse_val.hex:h -U hfuse:r:high_fuse_val.hex:h

clean:
	rm -f *.bin *.c~ *.o *.elf *.hex
