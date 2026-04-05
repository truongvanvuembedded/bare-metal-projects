#================================================================================
# STM32F103 Bare Metal Project Makefile
# This Makefile handles compilation, linking, flashing, and debugging for
# the STM32F103 microcontroller bare metal development.
#================================================================================


#================================================================================
# 1. COMPILER AND TOOLS CONFIGURATION
#================================================================================
# ARM EABI GCC compiler for arm-none-eabi architecture
CC = arm-none-eabi-gcc

# Object copy utility - used to convert ELF to binary format
CP = arm-none-eabi-objcopy

# GDB debugger for ARM (multiarch version supports multiple architectures)
GDB = gdb-multiarch

# OpenOCD - Open On-Chip Debugger for STM32 devices
OPENOCD = openocd


#================================================================================
# 2. PROJECT CONFIGURATION
#================================================================================
# Output directory where build artifacts will be stored
OUT_DIR = build

# Application name (can be overridden: make APP=blink)
APP ?= main

# Build output file paths
OUT = $(OUT_DIR)/$(APP)


#================================================================================
# 3. COMPILER FLAGS
#================================================================================
# ARM Cortex-M3 specific flags
#   -mcpu=cortex-m3  : Target CPU is Cortex-M3
#   -mthumb          : Use Thumb instruction set (16-bit)
#   -O0              : No optimization (faster compilation, useful for debugging)
#   -g               : Include debugging symbols
#   -ffreestanding   : Don't assume standard library functions are available
CFLAGS = -mcpu=cortex-m3 -mthumb -O0 -g -ffreestanding


#================================================================================
# 4. LINKER FLAGS
#================================================================================
# Linker script that defines memory layout (flash and RAM sections)
LINKER_SCRIPT = src/linker/stm32f103.ld

# Linker flags:
#   -T              : Specify linker script
#   -nostdlib       : Don't link standard library
#   -nostartfiles   : Don't use standard startup files
LDFLAGS = -T $(LINKER_SCRIPT) -nostdlib -nostartfiles


#================================================================================
# 5. INCLUDE PATHS
#================================================================================
# Include directory containing driver header files
INCLUDES = -I src/lib/drivers/Inc


#================================================================================
# 6. SOURCE FILES
#================================================================================
# Main application source file
TARGET_SRC = src/$(APP).c

# Library source files (startup code and device drivers)
LIB_SRC = src/startup/startup.c \
          src/lib/drivers/Src/Drv_GPIO.c \
          src/lib/drivers/Src/Drv_I2C.c \
          src/lib/drivers/Src/Drv_SPI.c \
          src/lib/drivers/Src/Drv_UART.c

# All source files combined (application + libraries)
SRC = $(TARGET_SRC) $(LIB_SRC)

# Flash memory address where code will be programmed (0x8000000 = start of Flash)
FLASH_ADDR = 0x8000000


#================================================================================
# 7. BUILD TARGETS - MAIN COMPILATION
#================================================================================

# Default target: clean build
all: clean
	@mkdir -p $(OUT_DIR)
	@echo "Building application: $(APP)..."
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC) $(LDFLAGS) -o $(OUT).elf
	$(CP) -O binary $(OUT).elf $(OUT).bin
	@echo "Build finished: $(OUT).bin"

# Clean: Remove all build artifacts
clean:
	@echo "Cleaning build directory..."
	@rm -rf $(OUT_DIR)
	@echo "Clean complete."


#================================================================================
# 8. FLASH PROGRAMMING
#================================================================================

# Flash: Build and program the binary to STM32F103 device
flash: all
	@echo "Flashing to STM32F103 at address $(FLASH_ADDR)..."
	st-flash write $(OUT).bin $(FLASH_ADDR)
	@echo "Flash complete."


#================================================================================
# 9. DEBUGGING TARGETS
#================================================================================

# debug-server: Start OpenOCD server (listens on port 3333 for GDB connections)
debug-server:
	@echo "Starting OpenOCD server..."
	@echo "GDB will connect to: localhost:3333"
	$(OPENOCD) -f interface/stlink.cfg -f target/stm32f1x.cfg

# debug: Start GDB client (requires OpenOCD server running separately)
#   Steps: 
#     1. Terminal 1: make debug-server
#     2. Terminal 2: make debug
debug:
	@echo "Starting GDB debugger..."
	$(GDB) $(OUT).elf \
	  -ex "target remote localhost:3333" \
	  -ex "monitor reset init" \
	  -ex "break main" \
	  -ex "continue"

# debug-all: Build + Start OpenOCD + Start GDB (all in one command)
debug-all:
	@echo "Building project..."
	@mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC) $(LDFLAGS) -o $(OUT).elf
	$(CP) -O binary $(OUT).elf $(OUT).bin
	@echo "Build finished."
	@echo "Starting OpenOCD server..."
	$(OPENOCD) -f interface/stlink.cfg -f target/stm32f1x.cfg & \
	  sleep 2 && \
	  echo "Starting GDB debugger..." && \
	  $(GDB) $(OUT).elf \
	    -ex "target remote localhost:3333" \
	    -ex "monitor reset init" \
	    -ex "break main" \
	    -ex "continue"


#================================================================================
# 10. CODE FORMATTING (using clang-format)
#================================================================================

# format-project: Format all C/H files in the current project
format-project:
	@echo "Formatting project files..."
	@find . \( -name "*.c" -o -name "*.h" \) -print0 | \
	  xargs -0 clang-format -i -style=file
	@echo "Formatting complete."

# format-lib: Format all C/H files in the library directory
format-lib:
	@echo "Formatting library files..."
	@find ../lib/ \( -name "*.c" -o -name "*.h" \) -print0 | \
	  xargs -0 clang-format -i -style=file
	@echo "Formatting complete."


#================================================================================
# 11. PHONY TARGETS (targets that don't represent actual files)
#================================================================================
.PHONY: all clean flash debug-server debug debug-all format-project format-lib