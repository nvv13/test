TOP_DIR = ../..
sinclude $(TOP_DIR)/tools/w800/conf.mk

ifndef PDIR
GEN_LIBS = libVS1053$(LIB_EXT)
endif

#DEFINES +=

sinclude $(TOP_DIR)/tools/w800/rules.mk

PDIR := ../$(PDIR)
sinclude $(PDIR)Makefile

fmt2:
	clang-format --style=GNU --verbose -i $(MYAPPDIRS)*.c $(MYAPPDIRS)*.h
