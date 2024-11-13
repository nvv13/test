TOP_DIR := .
TOOL_CHAIN_PATH := $(HOME)/.local/xPacks/@xpack-dev-tools/riscv-none-elf-gcc/14.2.0-2.1/.content/bin

TARGET_NAME ?= app
TARGET_ELF  ?= $(TARGET_NAME).elf
TARGET_HEX  ?= $(TARGET_NAME).hex
TARGET_BIN  ?= $(TARGET_NAME).bin

AS := $(TOOL_CHAIN_PATH)/riscv-none-elf-gcc
CC := $(TOOL_CHAIN_PATH)/riscv-none-elf-gcc
CXX := $(TOOL_CHAIN_PATH)/riscv-none-elf-g++
OBJCOPY := $(TOOL_CHAIN_PATH)/riscv-none-elf-objcopy

BUILD_DIR ?= ./build
SRC_DIRS ?= ./app ./vendor/Core ./vendor/Debug ./vendor/Peripheral ./vendor/Startup ./vendor/User

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.S)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

FLAGS ?= -march=rv32ec_zicsr -mabi=ilp32e -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g

ASFLAGS ?= $(FLAGS) -x assembler $(INC_FLAGS) -MMD -MP
CPPFLAGS ?=  $(FLAGS) $(INC_FLAGS) -g -MMD -MP
LDFLAGS ?= $(FLAGS) -T ./vendor/Ld/Link.ld -nostartfiles -Xlinker --gc-sections -Wl,--print-memory-usage -Wl,-Map,"$(BUILD_DIR)/CH32V003.map" --specs=nano.specs --specs=nosys.specs

all: $(BUILD_DIR)/$(TARGET_ELF)

$(BUILD_DIR)/$(TARGET_ELF): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)
	$(OBJCOPY) -Oihex   $@ $(BUILD_DIR)/$(TARGET_HEX)
	$(OBJCOPY) -Obinary $@ $(BUILD_DIR)/$(TARGET_BIN)
#	sz $(BUILD_DIR)/$(TARGET_BIN)

# assembly
$(BUILD_DIR)/%.S.o: %.S
	$(MKDIR_P) $(dir $@)
	$(CC) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p

flash:all
	$(TOP_DIR)/tools/minichlink/minichlink -w $(BUILD_DIR)/$(TARGET_BIN) 0x00000000
	$(TOP_DIR)/tools/minichlink/minichlink -b 

chipinfo:
	$(TOP_DIR)/tools/minichlink/minichlink -i

reboot:
	$(TOP_DIR)/tools/minichlink/minichlink -b

fmt:
	clang-format --style=GNU --verbose -i ./app/*.cpp ./app/*.h ./app/*.hpp


