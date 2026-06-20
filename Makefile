#Toolchain
CC      := i686-elf-gcc
ASM     := nasm
LD      := ld
OBJCOPY := objcopy

#FLAGS
#Note that -IScripts/headers is added which tells the compiler to look for files in -IScripts/headers
#This way I can directly import header files by just typing their name instead of writing the full path
CFLAGS  := -ffreestanding -O3 -Wall -Wextra -fno-stack-protector -g -IScripts/headers
LDFLAGS := -m elf_i386 -T linker.ld
ASMFLAGS = -f elf32 $(foreach dir, $(INC_DIRS), -i $(dir)/) #use = instead := because (search google)

# Directories
SRC_DIR   := Scripts/src
BUILD_DIR := build
BIN_DIR   := bin
INC_DIRS := $(shell find $(SRC_DIR) -name "*.inc" -exec dirname {} \; | sort -u)

# Targets
BOOT_BIN   := $(BIN_DIR)/boot.bin
KERNEL_ELF := $(BUILD_DIR)/kernel.elf
KERNEL_BIN := $(BIN_DIR)/kernel.bin
OS_IMAGE   := $(BIN_DIR)/os.img

# Gather sources
C_SOURCES   := $(shell find $(SRC_DIR) -name '*.c')
ASM_SOURCES := $(shell find $(SRC_DIR) -name '*.asm' ! -name 'boot.asm')
OBJECTS     := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES)) \
               $(patsubst $(SRC_DIR)/%.asm, $(BUILD_DIR)/%.asm.o, $(ASM_SOURCES))

# Force rebuild if headers change
HEADERS := $(wildcard Scripts/headers/*.h)

.PHONY: all clean run

all: $(OS_IMAGE)

# Rule: Combine boot.bin and kernel.bin
$(OS_IMAGE): $(BOOT_BIN) $(KERNEL_BIN)
	@mkdir -p $(BIN_DIR)
	cat $(BOOT_BIN) $(KERNEL_BIN) > $(OS_IMAGE)

$(BOOT_BIN): $(SRC_DIR)/boot.asm
	@mkdir -p $(BIN_DIR)
	$(ASM) -f bin $< -o $@

# Rule: Convert ELF to Binary
$(KERNEL_BIN): $(KERNEL_ELF)
	@mkdir -p $(BIN_DIR)
	$(OBJCOPY) -O binary $< $@

# Rule: Link ELF
$(KERNEL_ELF): $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS)

# Pattern: C files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Pattern: ASM files
$(BUILD_DIR)/%.asm.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS) $< -o $@
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

run: $(OS_IMAGE)
	qemu-system-i386 -drive format=raw,file=$(OS_IMAGE)

debug:
	   @echo "INC_DIRS is: $(INC_DIRS)"
	   @echo "ASMFLAGS is: $(ASMFLAGS)"