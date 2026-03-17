CC=arm-none-eabi-gcc
CFLAGS = -mcpu=cortex-m3 -mthumb -O0 -g -ffreestanding
LDFLAGS = -T linker/stm32f103.ld -nostdlib -nostartfiles

SRC = src/main.c \
      startup/startup.c \
      drivers/Src/Drv_GPIO.c \
      drivers/Src/Drv_I2C.c \
      drivers/Src/Drv_SPI.c \
      drivers/Src/Drv_UART.c
	  
OUT = build/main
INCLUDES=-Idrivers/Inc

all:
	mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC) $(LDFLAGS) -o $(OUT).elf
	arm-none-eabi-objcopy -O binary $(OUT).elf $(OUT).bin

flash:
	st-flash write build/main.bin 0x8000000

clean:
	rm -rf build