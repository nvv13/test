# name of your application
APPLICATION = task-09

# If no BOARD is found in the environment, use this default:
#BOARD ?= native
#BOARD ?= esp8266-esp-12x
BOARD ?= esp32-wroom-32

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(HOME)/github.com/OS/riot/RIOT-2024.07

BOARD_INSUFFICIENT_MEMORY := airfy-beacon chronos msb-430 msb-430h nrf51dongle \
                          nrf6310 nucleo-f103 nucleo-f334 pca10000 pca10005 spark-core \
                          stm32f0discovery telosb weio wsn430-v1_3b wsn430-v1_4 \
                          yunjia-nrf51822 z1 nucleo-f072


# Include packages that pull up and auto-init the link layer.
# NOTE: 6LoWPAN will be included if IEEE802.15.4 devices are present
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif

# Specify the mandatory networking modules for IPv6 and UDP
USEMODULE += gnrc_ipv6_router_default
USEMODULE += gnrc_udp

# Add a routing protocol
#USEMODULE += gnrc_rpl
#USEMODULE += auto_init_gnrc_rpl

# This application dumps received packets to STDIO using the pktdump module
#USEMODULE += gnrc_pktdump
# Additional networking modules that can be dropped if not needed
USEMODULE += gnrc_icmpv6_echo

# Add also the shell, some shell commands
USEMODULE += shell
USEMODULE += ps
USEMODULE += netstats_l2
USEMODULE += netstats_ipv6
#USEMODULE += netstats_rpl
USEMODULE += sntp

PROGRAMMER_QUIET=0

# Set a custom 802.15.4 channel if needed
#DEFAULT_CHANNEL ?= 26
#CFLAGS += -DDEFAULT_CHANNEL=$(DEFAULT_CHANNEL)

#CFLAGS += -DGNRC_RPL_LIFETIME_UNIT=1 -DGNRC_RPL_DEFAULT_LIFETIME=32
#CFLAGS += -DGNRC_RPL_REGULAR_DAO_INTERVAL=13
#CFLAGS += -DGNRC_RPL_DEFAULT_DIO_INTERVAL_DOUBLINGS=13

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DDEVELHELP

# Comment this out to join RPL DODAGs even if DIOs do not contain
# DODAG Configuration Options (see the doc for more info)
# CFLAGS += -DGNRC_RPL_DODAG_CONF_OPTIONAL_ON_JOIN

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
