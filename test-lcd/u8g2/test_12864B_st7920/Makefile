# name of your application
APPLICATION = test_LSD

# If no BOARD is found in the environment, use this default:
#BOARD ?= esp8266-esp-12x
BOARD ?= arduino-mega2560

PORT=/dev/ttyUSB0

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../../RIOT

USEMODULE += xtimer

USEPKG += u8g2

FEATURES_REQUIRED += periph_gpio periph_spi

# export parameters
#CFLAGS += -DTEST_OUTPUT=$(TEST_OUTPUT)

#CFLAGS += -DTEST_SPI=$(TEST_SPI)
#CFLAGS += -DTEST_I2C=$(TEST_I2C)

#CFLAGS += -DTEST_ADDR=$(TEST_ADDR)

#CFLAGS += -DTEST_PIN_CS=$(TEST_PIN_CS)
#CFLAGS += -DTEST_PIN_DC=$(TEST_PIN_DC)
#CFLAGS += -DTEST_PIN_RESET=$(TEST_PIN_RESET)

#CFLAGS += -DTEST_DISPLAY=$(TEST_DISPLAY)

include $(RIOTBASE)/Makefile.include
