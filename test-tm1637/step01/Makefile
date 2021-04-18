# name of your application
APPLICATION = arduino_hello-world

# If no BOARD is found in the environment, use this default:
#BOARD ?= native
#BOARD ?= esp8266-esp-12x
#BOARD ?= arduino-leonardo

BOARD ?= arduino-uno
PORT ?= /dev/ttyUSB0

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../RIOT

USEMODULE += arduino

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1
PROGRAMMER_QUIET=0

include $(RIOTBASE)/Makefile.include

#INCLUDES += -Iapplication_include

INCLUDES += -I$(CURDIR)