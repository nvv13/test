# pinout https://microtechnics.ru/arduino-mega-2560-r3-raspinovka-i-princzipialnaya-shema/
MCU=atmega2560
F_CPU=16000000UL  # 
LFUSE=0xff # 0xFF - default fuse, see https://www.engbedded.com/fusecalc/
HFUSE=0xd8 # SPIEN, BOOTSZ1, BOOTSZ0, BOOTRST
EFUSE=0xfd # BODLEVEL1
CXX=avr-g++
OBJCOPY=avr-objcopy
SIZE=avr-size
AVRDUDE=avrdude
CFLAGS=-std=c99 -Wall -g -Os -mmcu=${MCU} -DF_CPU=${F_CPU} -I. -I../User
CXX_FLAGS=-Wall -Os -std=gnu++11 -mmcu=${MCU} -DF_CPU=${F_CPU} -I. -I../User -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums

TARGET=main
SRCS=main.cpp ../User/CUsart.cpp

ATMEGA=m2560
PROTOCOL=wiring
DEVICE=/dev/ttyUSB0
BAUD=115200

	
all:
	${CXX} ${CXX_FLAGS} -o ${TARGET}.bin ${SRCS}
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.bin ${TARGET}.hex	
	${SIZE} -C --mcu=${MCU} ${TARGET}.bin
	
flash:
	${AVRDUDE} -p $(ATMEGA) -c $(PROTOCOL) -P $(DEVICE) -b $(BAUD) -v -U flash:w:${TARGET}.hex:i -D

writefuse:
	${AVRDUDE} -p $(ATMEGA) -c $(PROTOCOL) -P $(DEVICE) -b $(BAUD) -v -U hfuse:w:${HFUSE}:m -U lfuse:w:${LFUSE}:m -U efuse:w:${EFUSE}:m

readfuse:
	${AVRDUDE} -p $(ATMEGA) -c $(PROTOCOL) -P $(DEVICE) -b $(BAUD) -v -U lfuse:r:low_fuse_val.hex:h -U hfuse:r:high_fuse_val.hex:h -U efuse:r:ext_fuse_val.hex:h

clean:
	rm -f *.bin *.c~ *.o *.d *.elf *.hex

fmt:
	clang-format --style=GNU --verbose -i ./*.cpp ./*.h
