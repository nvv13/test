# name of your application
APPLICATION = test_spi_01

BOARD ?= bluepill

#include ../Makefile.tests_common

RIOTBASE ?= $(CURDIR)/../../../../RIOT

PROGRAMMER_QUIET = 0

FEATURES_REQUIRED += periph_spi

USEMODULE += xtimer
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ds3231

# Optional feature to trigger suit update through gpio callback
FEATURES_OPTIONAL += periph_gpio_irq

include $(RIOTBASE)/Makefile.include

