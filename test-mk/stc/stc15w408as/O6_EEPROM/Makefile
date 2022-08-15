SDCC ?= sdcc
STCCODESIZE ?= 8192
SDCCOPTS ?= --iram-size 256 --code-size $(STCCODESIZE) --xram-size 256 --data-loc 0x30 --disable-warning 126 --disable-warning 59
STCGAL ?= stcgal
STCGALOPTS ?= 
STCGALPORT ?= /dev/ttyUSB0
STCGALPROT ?= stc15
FLASHFILE ?= main.hex
SYSCLK ?= 35000
MKDIR_BUILD ?= mkdir -p build

#33177 
#11059

SRC = #src/adc.c src/ds1302.c

OBJ=$(patsubst src%.c,build%.rel, $(SRC))

all: main

build/%.rel: src/%.c src/%.h
	mkdir -p $(dir $@)
	$(SDCC) $(SDCCOPTS) -o $@ -c $<

main: $(OBJ)
	$(MKDIR_BUILD)
	$(SDCC) -o build/ src/$@.c $(SDCCOPTS) $^
	@ tail -n 5 build/main.mem | head -n 2
	@ tail -n 1 build/main.mem
	cp build/$@.ihx $@.hex
	
eeprom:
	sed -ne '/:..1/ { s/1/0/2; p }' main.hex > eeprom.hex

flash:
	$(STCGAL) -p $(STCGALPORT) -P $(STCGALPROT) -t $(SYSCLK) $(STCGALOPTS) $(FLASHFILE)

clean:
	rm -f *.ihx *.hex *.bin
	rm -rf build/*

