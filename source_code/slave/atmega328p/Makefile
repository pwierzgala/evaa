ARCH=avr
MCU=atmega328p
LD_MMCU=avr5
PLATFORM=arduino
F_CPU=16000000

DEV=/dev/ttyUSB0
WIN_DEV=//./com8
BAUDRATE?=115200
WIN_BAUDRATE=115200

BUILD_DIR=build
TARGET=main

BUILD_SUB_DIRS= \
action_manager

C_SOURCES_DIR=src
C_SOURCES= \
$(wildcard $(C_SOURCES_DIR)/*.c) \
$(wildcard $(C_SOURCES_DIR)/$(BUILD_SUB_DIRS)/*.c)

C_INCLUDES_DIR=inc
C_INCLUDES= \
-I$(C_INCLUDES_DIR) \
-I$(C_INCLUDES_DIR)/$(BUILD_SUB_DIRS)

OBJ_SOURCES= \
$(patsubst %.c,%.o,$(subst $(C_SOURCES_DIR)/,,$(C_SOURCES)))

$(info $(OBJ_SOURCES))
PREFIX=avr-

CC=$(PREFIX)gcc
CP=$(PREFIX)objcopy
SZ=$(PREFIX)size

CFLAGS=-g -Os -std=c11 -Wall -Wextra -pedantic -mmcu=$(MCU) -DF_CPU=$(F_CPU)
LDFLAGS=-g -mmcu=$(MCU)

all: make_build_dir compile link make_hex show_size

compile: $(patsubst %,$(BUILD_DIR)/%,$(OBJ_SOURCES))
	@$(CC) $(CFLAGS) $(C_INCLUDES) -c $(TARGET).c -o $(BUILD_DIR)/$(TARGET).o

$(BUILD_DIR)/%.o: $(C_SOURCES_DIR)/%.c
	@$(CC) $(CFLAGS) $(C_INCLUDES) -c $< -o $@

link:
	@$(CC) $(LDFLAGS) -o $(BUILD_DIR)/$(TARGET).elf \
	$(addprefix $(BUILD_DIR)/,$(OBJ_SOURCES)) $(BUILD_DIR)/$(TARGET).o 

make_hex:
	@$(CP) -j .text -j .bss -j .rodata -j .data -O ihex \
	$(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex

show_size:
	$(SZ) --format=$(ARCH) --mcu=$(MCU)  $(BUILD_DIR)/$(TARGET).elf

flash:
	@sudo avrdude -c $(PLATFORM) -p $(MCU) -b $(BAUDRATE) -P $(DEV) -vv \
	-U flash:w:$(BUILD_DIR)/$(TARGET).hex

win_flash:
	@avrdude -c $(PLATFORM) -p $(MCU) -b $(WIN_BAUDRATE) -P $(WIN_DEV) -vv \
	-U flash:w:$(BUILD_DIR)/$(TARGET).hex

make_build_dir:
	@mkdir -p $(BUILD_DIR)/$(BUILD_SUB_DIRS)

clean:
	@rm -rf $(BUILD_DIR)
