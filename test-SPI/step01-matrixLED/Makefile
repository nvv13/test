# name of your application
APPLICATION = test_spi_01

#BOARD ?= samr21-xpro
BOARD ?= esp8266-esp-12x

#include ../Makefile.tests_common

RIOTBASE ?= $(CURDIR)/../../RIOT

FEATURES_REQUIRED += periph_spi
FEATURES_OPTIONAL += periph_spi_reconfigure

USEMODULE += xtimer
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += schedstatistics

include $(RIOTBASE)/Makefile.include

