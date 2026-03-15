# Use Ubuntu as the base operating system
# Ubuntu is chosen because ARM toolchains are well supported on it
FROM ubuntu:22.04

# Avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Update package list and install only required development tools
RUN apt-get update && apt-get install -y \
    # ARM GCC cross compiler for Cortex-M microcontrollers
    gcc-arm-none-eabi \
    \
    # Build tools required to compile projects
    make \
    cmake \
    build-essential \
    \
    # Debugging tool that can connect to ARM targets
    gdb-multiarch \
    \
    # OpenOCD allows flashing/debugging via ST-Link or JTAG
    openocd \
    \
    # ST-Link command line utilities
    stlink-tools \
    \
    # Git is useful to clone repositories
    git \
    \
    # Clean package cache to reduce image size
    && rm -rf /var/lib/apt/lists/*

# Set the working directory inside the container
# Your project will usually be mounted here
WORKDIR /workspace

# Start an interactive shell when the container runs
CMD ["/bin/bash"]