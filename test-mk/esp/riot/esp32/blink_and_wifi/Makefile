APPLICATION = sntp-test02

# If no BOARD is found in the environment, use this default:
BOARD ?= esp32-wroom-32
#BOARD ?= esp32-wemos-lolin-d32-pro

#view msg prog tools
PROGRAMMER_QUIET=0
PORT=/dev/ttyUSB0
#PORT=/dev/ttyACM0
#PYTERMFLAGS=-tg


#BOARD ?= arduino-uno
#PORT ?= /dev/ttyUSB0

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(HOME)/github.com/OS/riot/RIOT-2024.07



#mesage prog tool
PROGRAMMER_QUIET=0

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

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

USEMODULE += shell
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

#USEMODULE += pppoe_support

USEMODULE += sock_async_event
USEMODULE += sock_ip
USEMODULE += sock_udp
USEMODULE += sock_tcp
USEMODULE += sock_dns

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


INCLUDES += -I$(CURDIR)

#нужно чтоб сдесь было - иначе не увидит название точки wifi ....
ifneq (,$(filter arch_esp,$(FEATURES_USED)))
  CFLAGS += -DESP_WIFI_SSID=\"$(WIFI_SSID)\"
  CFLAGS += -DESP_WIFI_PASS=\"$(WIFI_PASS)\"
endif
