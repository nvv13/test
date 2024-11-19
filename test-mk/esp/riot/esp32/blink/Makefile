# name of your application
APPLICATION = Task03

# If no BOARD is found in the environment, use this default:
#BOARD ?= esp32-wroom-32
#BOARD ?= esp32-wemos-lolin-d32-pro
BOARD ?= esp32s2-wemos-mini

#view msg prog tools
PROGRAMMER_QUIET=0
#PORT=/dev/ttyACM0
PORT=/dev/ttyUSB0
#PORT=/dev/ttyCH343USB0
#PYTERMFLAGS=-tg

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= /home/nvv/github.com/OS/riot/RIOT-2024.07

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
