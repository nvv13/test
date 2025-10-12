# pinout https://microtechnics.ru/arduino-uno-r3-obzor-raspinovka-princzipialnaya-shema/
MCU=atmega328
F_CPU=16000000UL  # 
CC=avr-gcc
OBJCOPY=avr-objcopy
SIZE=avr-size
AVRDUDE=avrdude
CFLAGS=-std=c99 -Wall -g -Os -mmcu=${MCU} -DF_CPU=${F_CPU} -I.
TARGET=main
SRCS=main.c 

ATMEGA=atmega328p
#PROTOCOL=wiring
PROTOCOL=arduino
DEVICE=/dev/ttyUSB0
BAUD=115200

	
all:
	${CC} ${CFLAGS} -o ${TARGET}.bin ${SRCS}
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.bin ${TARGET}.hex	
	${SIZE} -C --mcu=${MCU} ${TARGET}.bin
	
flash:
	${AVRDUDE} -p $(ATMEGA) -c $(PROTOCOL) -P $(DEVICE) -b $(BAUD) -v -U flash:w:${TARGET}.hex:i -D


clean:
	rm -f *.bin *.c~ *.o *.elf *.hex

fmt:
	clang-format --style=GNU --verbose -i ./*.c ./*.h
