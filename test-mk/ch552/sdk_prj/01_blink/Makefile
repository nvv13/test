TOP_DIR := .

TARGET_NAME ?= app
TARGET_INX  ?= $(TARGET_NAME).ihx
TARGET_HEX  ?= $(TARGET_NAME).hex
TARGET_BIN  ?= $(TARGET_NAME).bin

CC := sdcc
OBJCOPY := objcopy
MKDIR_P ?= mkdir -p


BUILD_DIR ?= ./build
SRC_DIRS ?= ./app ./vendor/inc ./vendor/src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.S)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.rel)
DEPS := $(OBJS:.rel=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

FREQ_SYS ?= 24000000
FLAGS ?= --xram-size 0x0400 --xram-loc 0x0100 --code-size 0x3800 -DFREQ_SYS=$(FREQ_SYS)
# -V -c -mmcs51 --model-small 
CFLAGS ?=  $(FLAGS) $(INC_FLAGS) -mmcs51 --model-small 


all: $(BUILD_DIR)/$(TARGET_INX)


$(BUILD_DIR)/$(TARGET_INX): $(OBJS)
	$(CC) $(OBJS) -o $@ $(FLAGS)
	$(OBJCOPY) -I ihex -O binary  $@ $(BUILD_DIR)/$(TARGET_BIN)


# c source
$(BUILD_DIR)/%.c.rel: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@




flash:
	python3 -m ch55xtool -r -f $(BUILD_DIR)/$(TARGET_BIN)

clean:
	rm -r $(BUILD_DIR)

fmt:
	clang-format --style=GNU --verbose -i app/*. app/*.h


