cmake_minimum_required(VERSION 3.12)

set(PICO_PLATFORM rp2350-riscv)

set(PICO_TOOLCHAIN_PATH /opt/riscv32/riscv32-embecosm-centos7-gcc13.2.0/bin)

include(pico_sdk_import.cmake)

project(sdk-prj C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


pico_sdk_init()

add_subdirectory(01_usb_serial)

