TOP_DIR := .
sinclude $(TOP_DIR)/tools/w800/conf.mk

ifndef PDIR # {
GEN_IMAGES= $(TARGET).elf
GEN_BINS = $(TARGET).bin
SUBDIRS = \
    $(TOP_DIR)/app \
    $(TOP_DIR)/demo
endif # } PDIR

ifndef PDIR # {
ifeq ($(USE_LIB), 0)
SUBDIRS += \
	$(TOP_DIR)/platform/arch        \
	$(TOP_DIR)/platform/common      \
	$(TOP_DIR)/platform/drivers     \
	$(TOP_DIR)/platform/sys         \
	$(TOP_DIR)/src/os               \
	$(TOP_DIR)/src/app
endif
endif

COMPONENTS_$(TARGET) =	\
    $(TOP_DIR)/app/libuser$(LIB_EXT) \
    $(TOP_DIR)/demo/libdemo$(LIB_EXT)

ifeq ($(USE_LIB), 0)
COMPONENTS_$(TARGET) += \
    $(TOP_DIR)/platform/boot/libwmarch$(LIB_EXT)        \
    $(TOP_DIR)/platform/common/libwmcommon$(LIB_EXT)    \
    $(TOP_DIR)/platform/drivers/libdrivers$(LIB_EXT)    \
    $(TOP_DIR)/platform/sys/libwmsys$(LIB_EXT)          \
    $(TOP_DIR)/src/os/libos$(LIB_EXT)
endif


LINKLIB = 	\
	$(TOP_DIR)/lib/$(CONFIG_ARCH_TYPE)/libdsp$(LIB_EXT)	

	


ifeq ($(USE_LIB), 1)
LINKLIB += \
	$(TOP_DIR)/lib/$(CONFIG_ARCH_TYPE)/libwmarch$(LIB_EXT)      \
	$(TOP_DIR)/lib/$(CONFIG_ARCH_TYPE)/libwmcommon$(LIB_EXT)    \
	$(TOP_DIR)/lib/$(CONFIG_ARCH_TYPE)/libdrivers$(LIB_EXT)     \
	$(TOP_DIR)/lib/$(CONFIG_ARCH_TYPE)/libos$(LIB_EXT)          \
	$(TOP_DIR)/lib/$(CONFIG_ARCH_TYPE)/libwmsys$(LIB_EXT)
endif

LINKFLAGS_$(TARGET) =  \
	$(LINKLIB)

CONFIGURATION_DEFINES =	

DEFINES +=				\
	$(CONFIGURATION_DEFINES)

DDEFINES +=				\
	$(CONFIGURATION_DEFINES)

INCLUDES := $(INCLUDES) -I$(PDIR)include
INCLUDES += -I ./

sinclude $(TOP_DIR)/tools/$(CONFIG_ARCH_TYPE)/rules.mk

.PHONY: FORCE
FORCE:
