APPLICATION = paho-mqtt-example

# If no BOARD is found in the environment, use this default:
BOARD ?= esp8266-esp-12x

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(HOME)/github.com/OS/riot/RIOT-2024.07

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

WIFI_SSID ?= "bred1"
WIFI_PASS ?= "9115676369"

ifneq (,$(DEFAULT_MQTT_CLIENT_ID))
  CFLAGS += -DDEFAULT_MQTT_CLIENT_ID=\"$(DEFAULT_MQTT_CLIENT_ID)\"
endif
ifneq (,$(DEFAULT_MQTT_USER))
  CFLAGS += -DDEFAULT_MQTT_USER=\"$(DEFAULT_MQTT_USER)\"
endif
ifneq (,$(DEFAULT_MQTT_PWD))
  CFLAGS += -DDEFAULT_MQTT_PWD=\"$(DEFAULT_MQTT_PWD)\"
endif

CFLAGS += -DBMX280_PARAM_I2C_ADDR=0x76
CFLAGS += -DDEVELHELP
#USEMODULE += bmp280_i2c
USEMODULE += bme280_i2c
#USEMODULE += periph_adc
USEMODULE += fmt
USEMODULE += xtimer
USEMODULE += printf_float
FEATURES_REQUIRED = periph_adc
FEATURES_REQUIRED += periph_gpio
#FEATURES_REQUIRED += periph_gpio_irq
FEATURES_REQUIRED += periph_i2c

USEMODULE += shell
USEMODULE += ps
USEMODULE += netdev_default
USEPKG += paho-mqtt

# paho-mqtt depends on TCP support, choose the stack you want
LWIP_IPV4 ?= 1

ifneq (0,$(LWIP_IPV4))
  USEMODULE += ipv4_addr
  USEMODULE += lwip_arp
  USEMODULE += lwip_ipv4
  USEMODULE += lwip_dhcp_auto
  CFLAGS += -DETHARP_SUPPORT_STATIC_ENTRIES=1
  LWIP_IPV6 ?= 0
else
  LWIP_IPV6 ?= 1
endif

ifneq (0,$(LWIP_IPV6))
  USEMODULE += ipv6_addr
  USEMODULE += lwip_ipv6_autoconfig
endif

USEMODULE += lwip_netdev
USEMODULE += lwip

USEMODULE += sock_async_event
USEMODULE += sock_ip
USEMODULE += sock_udp
USEMODULE += sock_tcp

####

include $(RIOTBASE)/Makefile.include

ifneq (,$(filter arch_esp,$(FEATURES_USED)))
  CFLAGS += -DESP_WIFI_SSID=\"$(WIFI_SSID)\"
  CFLAGS += -DESP_WIFI_PASS=\"$(WIFI_PASS)\"
endif
