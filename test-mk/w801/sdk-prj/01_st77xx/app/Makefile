TOP_DIR = ..
sinclude $(TOP_DIR)/tools/w800/conf.mk

ifndef PDIR
GEN_LIBS = libuser$(LIB_EXT)
MYAPPDIRS = .
endif

#DEFINES +=

sinclude $(TOP_DIR)/tools/w800/rules.mk

INCLUDES := $(INCLUDES) -I $(PDIR)include
INCLUDES += -I ./st77xx

PDIR := ../$(PDIR)
sinclude $(PDIR)Makefile

