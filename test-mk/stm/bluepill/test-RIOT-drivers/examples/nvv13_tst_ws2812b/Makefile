#arduino-udo pin = D3       --- not support  ---
#CFLAGS="-DWS2812B_PARAM_DATA_PIN=(GPIO_PIN(3,3))"


#bluepill pin = B9
CFLAGS+="-DWS2812B_PARAM_DATA_PIN=(GPIO_PIN(1,9))"

#NUM_LED
CFLAGS+="-DWS2812B_PARAM_LED_NUMOF=(20)"


# name of your application
APPLICATION = test_ws2818b

# If no BOARD is found in the environment, use this default:
BOARD ?= bluepill

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

USEMODULE += ws2812b
USEMODULE += color
USEMODULE += xtimer

include $(RIOTBASE)/Makefile.include

