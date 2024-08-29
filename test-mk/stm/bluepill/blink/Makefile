# name of your application
APPLICATION = Task03

# If no BOARD is found in the environment, use this default:
#BOARD ?= bluepill-stm32f030c8
BOARD ?= bluepill-stm32f103c8
#BOARD ?= bluepill-stm32f103cb
#BOARD ?= blackpill-stm32f103c8
#BOARD ?= weact-f411ce
#BOARD ?= weact-f401ce

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
USEMODULE += shell_cmds_default
USEMODULE += ps
FEATURES_REQUIRED = periph_gpio

include $(RIOTBASE)/Makefile.include
