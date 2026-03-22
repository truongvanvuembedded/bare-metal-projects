CC=arm-none-eabi-gcc
CFLAGS = -mcpu=cortex-m3 -mthumb -O0 -g -ffreestanding
LDFLAGS = -T linker/stm32f103.ld -nostdlib -nostartfiles

SRC = src/main.c \
      startup/startup.c \
      ../lib/drivers/Src/Drv_GPIO.c \
      ../lib/drivers/Src/Drv_I2C.c \
      ../lib/drivers/Src/Drv_SPI.c \
      ../lib/drivers/Src/Drv_UART.c
	  
OUT = build/main
INCLUDES=-I../lib/drivers/Inc

# Debug tools
GDB = gdb-multiarch
OPENOCD = openocd

all: clean
	mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC) $(LDFLAGS) -o $(OUT).elf
	arm-none-eabi-objcopy -O binary $(OUT).elf $(OUT).bin

flash: all
	st-flash write build/main.bin 0x8000000

clean:
	rm -rf build

# ================= DEBUG =================

# 1. Run OpenOCD server
debug-server:
	$(OPENOCD) -f interface/stlink.cfg -f target/stm32f1x.cfg

# 2. Run GDB client
debug:
	$(GDB) $(OUT).elf \
	-ex "target remote localhost:3333" \
	-ex "monitor reset init" \
	-ex "break main" \
	-ex "continue"

# 3. One command run all (build + openocd + gdb)
debug-all:
	@echo "Building project..."
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC) $(LDFLAGS) -o $(OUT).elf
	arm-none-eabi-objcopy -O binary $(OUT).elf $(OUT).bin
	@echo "Starting OpenOCD..."
	$(OPENOCD) -f interface/stlink.cfg -f target/stm32f1x.cfg & \
	sleep 2 && \
	echo "Starting GDB..." && \
	$(GDB) $(OUT).elf \
	-ex "target remote localhost:3333" \
	-ex "monitor reset init" \
	-ex "break main" \
	-ex "continue"

# ================= format =================
.PHONY: format-project
format-project:
	find . \( -name "*.c" -o -name "*.h" \) -print0 | \
	xargs -0 clang-format -i -style=file

.PHONY: format-lib
format-lib:
	find ../lib/ \( -name "*.c" -o -name "*.h" \) -print0 | \
	xargs -0 clang-format -i -style=file