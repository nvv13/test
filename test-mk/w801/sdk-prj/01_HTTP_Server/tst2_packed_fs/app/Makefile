TOP_DIR = ..
sinclude $(TOP_DIR)/tools/w800/conf.mk

ifndef PDIR
GEN_LIBS = libuser$(LIB_EXT)
MYAPPDIRS = .
endif

#DEFINES +=

sinclude $(TOP_DIR)/tools/w800/rules.mk

INCLUDES := $(INCLUDES) -I $(PDIR)include

PDIR := ../$(PDIR)
sinclude $(PDIR)Makefile


PACK = ../../../../W801_SDK-linux/tools/mongoose/pack             # Utility to pack files into a packed filesystem

FILES_TO_EMBED ?= $(wildcard web_root/*)

packfs:                    # Cleanup. Delete built program and all build artifacts
	$(PACK) $(FILES_TO_EMBED) > packed_fs.c

