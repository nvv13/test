# name of your application
APPLICATION = TestPWM

BOARD ?= arduino-mega2560

PORT=/dev/ttyUSB0


RIOTBASE ?= $(CURDIR)/../../../../RIOT

FEATURES_REQUIRED = periph_pwm

USEMODULE += xtimer
USEMODULE += shell

include $(RIOTBASE)/Makefile.include
