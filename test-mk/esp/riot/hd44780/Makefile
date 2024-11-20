# name of your application
APPLICATION = test_LSD

# If no BOARD is found in the environment, use this default:
BOARD ?= esp8266-esp-12x

# This has to be the absolute path to the RIOT base directory:
#RIOTBASE ?= $(CURDIR)/../../../RIOT
RIOTBASE ?= $(HOME)/github.com/OS/riot/RIOT-2024.07

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DDEVELHELP

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Modules to include:
#USEMODULE += aip31068
USEMODULE += xtimer
USEMODULE += shell
#USEMODULE += pcf857x
#USEMODULE += hd44780

FEATURES_REQUIRED += periph_gpio
FEATURES_REQUIRED += periph_i2c

#include $(CURDIR)/include

include $(RIOTBASE)/Makefile.include 





