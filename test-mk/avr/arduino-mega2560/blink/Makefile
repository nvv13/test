# name of your application
APPLICATION = Task03

# If no BOARD is found in the environment, use this default:
BOARD ?= arduino-mega2560

PORT=/dev/ttyUSB0

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../../../../RIOT

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DDEVELHELP

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Modules to include:
USEMODULE += xtimer
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps
FEATURES_REQUIRED = periph_gpio

include $(RIOTBASE)/Makefile.include
