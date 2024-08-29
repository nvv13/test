# name of your application
APPLICATION = max7219clock

#BOARD ?= bluepill-stm32f030c8
BOARD ?= bluepill-stm32f103c8
#BOARD ?= bluepill-stm32f103cb
#BOARD ?= blackpill-stm32f103c8
#BOARD ?= weact-f411ce
#BOARD ?= weact-f401ce
#BOARD ?= seeedstudio-gd32


PORT=/dev/ttyUSB0

#include ../Makefile.tests_common

RIOTBASE ?= $(CURDIR)/../../../../../../RIOT

PROGRAMMER_QUIET = 0

FEATURES_REQUIRED += periph_spi
FEATURES_REQUIRED += periph_adc

USEMODULE += xtimer
USEMODULE += shell
USEMODULE += shell_cmds_default
USEMODULE += ds3231

# Optional feature to trigger suit update through gpio callback
FEATURES_OPTIONAL += periph_gpio_irq

include $(RIOTBASE)/Makefile.include

