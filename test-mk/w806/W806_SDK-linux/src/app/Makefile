TOP_DIR = ../..
sinclude $(TOP_DIR)/tools/w800/conf.mk

ifndef PDIR
GEN_LIBS = libapp$(LIB_EXT)
ifeq ($(USE_NIMBLE),1)
COMPONENTS_libapp = cjson/libcjson$(LIB_EXT) 	\
					polarssl/libpolarssl$(LIB_EXT) 	\
					mbedtls/libmbedtls$(LIB_EXT) \
					fatfs/libfatfs$(LIB_EXT)
else
COMPONENTS_libapp = cjson/libcjson$(LIB_EXT) 	\
					polarssl/libpolarssl$(LIB_EXT) 	\
					mbedtls/libmbedtls$(LIB_EXT) \
					fatfs/libfatfs$(LIB_EXT)
endif
endif

#DEFINES +=

sinclude $(TOP_DIR)/tools/w800/rules.mk

INCLUDES := $(INCLUDES) -I $(PDIR)include

PDIR := ../$(PDIR)
sinclude $(PDIR)Makefile