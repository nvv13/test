# name of your application
APPLICATION = test_spi_01

#BOARD ?= samr21-xpro
BOARD ?= esp8266-esp-12x

#include ../Makefile.tests_common

RIOTBASE ?= $(HOME)/github.com/OS/riot/RIOT-2024.07

FEATURES_REQUIRED += periph_spi

USEMODULE += xtimer
USEMODULE += shell
USEMODULE += ds3231

# Optional feature to trigger suit update through gpio callback
FEATURES_OPTIONAL += periph_gpio_irq

#WIFI_SSID ?= "bred8"
#WIFI_PASS ?= "123123123"
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

USEMODULE += ps
#USEMODULE += netdev_default
#USEPKG += paho-mqtt

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

#USEMODULE += sntp

####


ifneq (,$(filter arch_esp,$(FEATURES_USED)))
  USEMODULE += esp_wifi
  CFLAGS += -DESP_WIFI_SSID=\"$(WIFI_SSID)\"
  CFLAGS += -DESP_WIFI_PASS=\"$(WIFI_PASS)\"
endif

ifeq ($(BOARD),native)
  USEMODULE += netdev_default
endif


include $(RIOTBASE)/Makefile.include

ifneq (,$(filter arch_esp,$(FEATURES_USED)))
  CFLAGS += -DESP_WIFI_SSID=\"$(WIFI_SSID)\"
  CFLAGS += -DESP_WIFI_PASS=\"$(WIFI_PASS)\"
endif


