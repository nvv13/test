TOP_DIR = ..
sinclude $(TOP_DIR)/tools/w800/conf.mk

ifndef PDIR
GEN_LIBS = libuser$(LIB_EXT)
endif

#DEFINES +=

sinclude $(TOP_DIR)/tools/w800/rules.mk

CFLAGS += -DU8X8_WITH_USER_PTR
CFLAGS += -DMODULE_PERIPH_SPI

INCLUDES := $(INCLUDES) -I $(PDIR)include
INCLUDES += -I ./u8g2

PDIR := ../$(PDIR)
sinclude $(PDIR)Makefile

