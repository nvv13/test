SDCC ?= E:\com2on\Install\Free\Crosplatform\sdcc\sdcc\bin\sdcc
TAIL ?= C:\project.win\unixtools\tail
HEAD ?= C:\project.win\unixtools\head
SED ?= C:\project.win\unixtools\sed
STCCODESIZE ?= 2048
SDCCOPTS ?= --iram-size 128 --code-size $(STCCODESIZE) --xram-size 128 --data-loc 0x30 --disable-warning 126 --disable-warning 59
STCGAL ?= stcgal
STCGALOPTS ?= 
STCGALPORT ?= /dev/ttyUSB0
STCGALPROT ?= stc15
FLASHFILE ?= main.hex
SYSCLK ?= 35000
MKDIR_BUILD ?= cmd.exe /C IF NOT exist build ( mkdir build )

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
	@ $(TAIL) -n 5 build/main.mem | $(HEAD) -n 2
	@ $(TAIL) -n 1 build/main.mem
	cp build/$@.ihx $@.hex
	
eeprom:
	$(SED) -ne '/:..1/ { s/1/0/2; p }' main.hex > eeprom.hex


flash:
	$(STCGAL) -p $(STCGALPORT) -P $(STCGALPROT) -t $(SYSCLK) $(STCGALOPTS) $(FLASHFILE)

clean:
	rm -f *.ihx *.hex *.bin
	rm -rf build/*

fmt:
	clang-format --style=GNU --verbose -i src/*.c src/*.h
