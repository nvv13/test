# name of your application
APPLICATION = Task03

# If no BOARD is found in the environment, use this default:
BOARD ?= atmega328p
ATMEGA328P_CLOCK    ?= 16000000UL
AVRDUDE_PROGRAMMER = usbasp

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../../RIOT

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DDEVELHELP

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Modules to include:
USEMODULE += xtimer
#USEMODULE += shell          - not from Atmega
#USEMODULE += shell_commands - not from Atmega
#USEMODULE += ps             - not from Atmega
#USEMODULE += ds3231
FEATURES_REQUIRED = periph_gpio
#FEATURES_REQUIRED += periph_spi
#FEATURES_OPTIONAL += periph_gpio_irq

include $(RIOTBASE)/Makefile.include
