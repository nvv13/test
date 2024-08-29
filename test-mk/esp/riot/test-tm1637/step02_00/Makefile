APPLICATION = tm1637-test02

# If no BOARD is found in the environment, use this default:
#BOARD ?= esp32-wroom-32
#BOARD ?= esp8266-esp-12x

BOARD ?= arduino-uno
PORT ?= /dev/ttyUSB0

#BOARD ?= atmega328p
#ATMEGA328P_CLOCK    ?= 16000000UL
#AVRDUDE_PROGRAMMER = usbasp

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../RIOT
#RIOTBASE ?= $(CURDIR)/../../../../RIOT

CFLAGS += -DDEVELHELP

# Features required
#FEATURES_REQUIRED += cpp # basic C++ support
#FEATURES_REQUIRED += libstdcpp # libstdc++ support (for #include <cstdio>)

# If you want to add some extra flags when compile c++ files, add these flags
# to CXXEXFLAGS variable
CXXEXFLAGS +=


#mesage prog tool
PROGRAMMER_QUIET=0

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1


USEMODULE += shell
#USEMODULE += shell_commands
USEMODULE += ps

USEMODULE += xtimer
FEATURES_REQUIRED = periph_gpio
FEATURES_REQUIRED += cpp

DISABLE_MODULE += core_msg

include $(RIOTBASE)/Makefile.include

INCLUDES += -I$(CURDIR)

