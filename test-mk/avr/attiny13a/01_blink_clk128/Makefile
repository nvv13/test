MCU=attiny13
F_CPU=128000 # 128kHz 
HFUSE=0xFF   # 0xFF 
LFUSE=0x7B   # 0x7B Int. RC Osc. 128 kHz; see https://www.engbedded.com/fusecalc/
CC=avr-gcc
OBJCOPY=avr-objcopy
SIZE=avr-size
AVRDUDE=avrdude
CFLAGS=-std=c99 -Wall -g -Os -mmcu=${MCU} -DF_CPU=${F_CPU} -I.
TARGET=main
SRCS=main.c 
BITCLOCK=125kHz
	
all:
	${CC} ${CFLAGS} -o ${TARGET}.bin ${SRCS}
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.bin ${TARGET}.hex	
	${SIZE} -C --mcu=${MCU} ${TARGET}.bin
	
flash:
	${AVRDUDE} -p ${MCU} -c usbasp -B ${BITCLOCK} -U flash:w:${TARGET}.hex:i -F -P usb

writefuse:
	${AVRDUDE} -p ${MCU} -c usbasp -B 125kHz -U hfuse:w:${HFUSE}:m -U lfuse:w:${LFUSE}:m

readfuse:
	${AVRDUDE} -p ${MCU} -c usbasp -B 125kHz -U lfuse:r:low_fuse_val.hex:h -U hfuse:r:high_fuse_val.hex:h

clean:
	rm -f *.bin *.c~ *.o *.elf *.hex

