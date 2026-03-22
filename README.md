# Bare-Metal Project

## 📌 Overview

This project demonstrates how to build a complete **bare-metal embedded system** for an microcontroller **from scratch**, without using any IDE.

The goal is to deeply understand:

* How a program is compiled and linked
* How a microcontroller boots and executes code
* How low-level drivers interact with hardware
* How toolchains and debuggers work internally

Everything in this project is written and configured manually, including:

* Startup code
* Linker script
* Peripheral drivers
* Build system (Makefile)

---

## 🎯 Objectives

* Understand the full embedded build process (C → Assembly → ELF → BIN)
* Learn how ARM Cortex-M startup works
* Control memory layout using a custom linker script
* Develop drivers without relying on vendor HAL libraries
* Build and debug firmware using command-line tools only

---

## 🛠️ Technologies & Tools

* **Language:** C
* **Architecture:** ARM Cortex-M3
* **Build System:** Makefile
* **Toolchain:** `arm-none-eabi-gcc`
* **Debugger:** `gdb` / `gdb-multiarch`
* **Flashing Tool:** `st-flash`
* **Debug Server:** OpenOCD
* **Environment:** Docker

---

## 🐳 Development Environment (Docker)

This project uses Docker to ensure a consistent and reproducible development environment.

### Build Docker image:

```bash
docker build -t baremetal_project .
```

### Run container:

```bash
docker run -it --rm \
    --privileged \
    -v ${PWD}:/workspace \
    -w /workspace \
    baremetal_project
```

### Using Dev Container for Build

You can also use a **VSCode Dev Container** to build and work on this project without installing toolchains locally.


## 📂 Project Structure

```
.
├── LICENSE
├── README.md
├── .clang-format                  # Format rule
├── drivers/
│   ├── Inc/                       # Driver header files (Drv_GPIO.h, Drv_I2C.h, Drv_SPI.h, Drv_UART.h, stm32f103c8t6.h)
│   └── Src/                       # Driver source files (Drv_GPIO.c, Drv_I2C.c, Drv_SPI.c, Drv_UART.c)
└── stm32f103/
    └── stm32-blink/
        ├── build/                 # Compiled output files (main.elf, main.bin)
        ├── linker/                # Linker script (stm32f103.ld)
        ├── Makefile               # Build system
        ├── src/                   # Application source code (main.c)
        └── startup/               # Startup code (startup.c, vector table)
```