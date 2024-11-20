# name of your application
APPLICATION = test_spi_01

#BOARD ?= samr21-xpro
BOARD ?= esp8266-esp-12x

#include ../Makefile.tests_common

RIOTBASE ?= $(HOME)/github.com/OS/riot/RIOT-2024.07

FEATURES_REQUIRED += periph_spi

USEMODULE += xtimer
USEMODULE += shell
USEMODULE += ds3231

# Optional feature to trigger suit update through gpio callback
FEATURES_OPTIONAL += periph_gpio_irq

include $(RIOTBASE)/Makefile.include

