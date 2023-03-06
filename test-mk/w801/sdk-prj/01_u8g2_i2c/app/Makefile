TOP_DIR = ..
sinclude $(TOP_DIR)/tools/w800/conf.mk

ifndef PDIR
GEN_LIBS = libuser$(LIB_EXT)
MYAPPDIRS = .
endif

#DEFINES +=

sinclude $(TOP_DIR)/tools/w800/rules.mk

CFLAGS += -DU8X8_WITH_USER_PTR

INCLUDES := $(INCLUDES) -I $(PDIR)include

PDIR := ../$(PDIR)
sinclude $(PDIR)Makefile

