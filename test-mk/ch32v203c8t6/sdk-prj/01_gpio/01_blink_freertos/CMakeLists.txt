# Set the system name of CMake to Generic
set(CMAKE_SYSTEM_NAME Generic)

# Set the system version of CMake to 1
set(CMAKE_SYSTEM_VERSION 1)

# Set the target type that CMake tries to compile to a static library
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

# Set the minimum version requirement of CMake to 3.20
cmake_minimum_required(VERSION 3.19)

# Set the toolchain path
set(TOOLPATH "/home/nvv/.local/xPacks/@xpack-dev-tools/riscv-none-elf-gcc/14.2.0-2.1/.content/bin/riscv-none-elf-")

# If the current system is Windows
if (WIN32)
    # Output message: The current system is Windows
    MESSAGE(STATUS "Now is windows!")

    # Set the C compiler path
    set(CMAKE_C_COMPILER ${TOOLPATH}gcc.exe)

    # Set the C++ compiler path
    set(CMAKE_CXX_COMPILER ${TOOLPATH}g++.exe)

    # Set the assembly compiler path
    set(CMAKE_ASM_COMPILER ${TOOLPATH}gcc.exe)

    # Set the AR tool path
    set(CMAKE_AR ${TOOLPATH}ar.exe)

    # Set the OBJCOPY tool path
    set(CMAKE_OBJCOPY ${TOOLPATH}objcopy.exe)

    # Set the OBJDUMP tool path
    set(CMAKE_OBJDUMP ${TOOLPATH}objdump.exe)

    # Set the SIZE tool path
    set(SIZE ${TOOLPATH}size.exe)

# If the current system is UNIX or UNIX-like system
elseif (UNIX)
    # Output message: The current system is UNIX or UNIX-like system
    MESSAGE(STATUS "Now is UNIX-like OS!")

    # Set C compiler path
    set(CMAKE_C_COMPILER ${TOOLPATH}gcc)

    # Set C++ compiler path
    set(CMAKE_CXX_COMPILER ${TOOLPATH}g++)

    # Set assembly compiler path
    set(CMAKE_ASM_COMPILER ${TOOLPATH}gcc)

    # Set AR tool path
    set(CMAKE_AR ${TOOLPATH}ar)

    # Set OBJCOPY tool path
    set(CMAKE_OBJCOPY ${TOOLPATH}objcopy)

    # Set OBJDUMP tool path
    set(CMAKE_OBJDUMP ${TOOLPATH}objdump)

    # Set SIZE tool path
    set(SIZE ${TOOLPATH}size)

# If the current system is not Windows, UNIX or UNIX-like system
else ()
    # Output message: Unsupported system
    MESSAGE(STATUS "Unsupported system!")
endif ()

# Set the project name and language used
project(ch32v203-FreeRTOS C CXX ASM)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)

# Set the C standard to C99
set(CMAKE_C_STANDARD 99)

# Add compile options
add_compile_options(
        -march=rv32imac_zicsr  # Set the target architecture to RISC-V, with RV32I basic integer instruction set, M standard multiplication and division extensions, A standard atomic extensions, C standard compression extensions
        -mabi=ilp32     # Set the application binary interface (ABI) to ilp32, that is, integers, long integers and pointers are all 32 bits
        -mcmodel=medany  # Set the code model to medany, suitable for any size of code and data
        -msmall-data-limit=8  # Set the size limit of the small data area to 8 bytes
        -mno-save-restore  # Disable optimization of saving and restoring registers before and after function calls
)

add_compile_options(
        -fmessage-length=0  # Set the maximum length of diagnostic messages to 0, that is, no maximum length
        -fsigned-char  # Set char type to signed type
        -ffunction-sections  # Put each function in its own section for linker optimization
        -fdata-sections  # Put data in its own section for linker optimization
        -fno-common  # Do not allow multiple files to share the same global variable
)


# Compile level
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast -flto)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    message(STATUS "Debug mode")
    add_compile_options(-Og -g)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
endif ()

# Compile information level
add_compile_options(-Wall -Wextra -Wpedantic -Wstack-usage=2048)


# Header file path
include_directories(
        app
        vendor/User
        vendor/Core
        FreeRTOS/include
        FreeRTOS/portable/GCC/RISC-V
        FreeRTOS/portable/GCC/RISC-V/chip_specific_extensions/RV32I_PFIC_no_extensions
        vendor/Debug
        vendor/Peripheral/inc
)

# Macro definition
# add_definitions(-DDEBUG=1)

# Source code file
file(GLOB_RECURSE SOURCES
        "app/*.c"
        "vendor/User/*.c"
        "vendor/Core/core_riscv.c"
        "vendor/Debug/debug.c"
        "FreeRTOS/*.c"
        "FreeRTOS/portable/GCC/RISC-V/*.c"
        "FreeRTOS/portable/MemMang/heap_4.c"
        "FreeRTOS/portable/GCC/RISC-V/portASM.S"
        "vendor/Peripheral/src/*.c"
        "vendor/Startup/startup_ch32v20x_D6.S"
)
list(REMOVE_ITEM SOURCES ${CMAKE_SOURCE_DIR}/FreeRTOS/portable/Common/mpu_wrappers.c)


# Set the path of the linker script
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/FreeRTOS/Ld/Link.ld)

# Add link options
add_link_options(
    -march=rv32imac_zicsr  # 设置目标架构为RISC-V，具有RV32I基础整数指令集，M标准乘法和除法扩展，A标准原子扩展，C标准压缩扩展
    -mabi=ilp32  # 设置应用二进制接口（ABI）为ilp32，即整数、长整数和指针都是32位
    -nostartfiles  # 不使用标准系统启动文件
    -Xlinker --gc-sections  # 通过链接器选项--gc-sections，删除未使用的代码和数据块
    -Wl,--print-memory-usage  # 通过链接器选项--print-memory-usage，打印内存使用情况
    -Wl,-Map,${PROJECT_NAME}.map  # 通过链接器选项-Map，生成map文件，显示符号的分布情况
    --specs=nano.specs  # 使用nano.specs规格文件，这会链接更小的C库
    --specs=nosys.specs  # 使用nosys.specs规格文件，这会链接一个不包含系统调用的C库
)

# Add link script options
add_link_options(-T ${LINKER_SCRIPT})

# Compile executable files
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

# Link static libraries
# target_link_libraries(${PROJECT_NAME}.elf printfloat)

# Set output file variables
set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)
set(LST_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.lst)

# Add a custom command to generate output files after building a target
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE} # 生成 .hex 文件
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE} # 生成 .bin 文件
    COMMAND ${CMAKE_OBJDUMP} --all-headers --demangle --disassemble $<TARGET_FILE:${PROJECT_NAME}.elf> > ${LST_FILE} # 生成 .lst 文件
    COMMENT "Building ${HEX_FILE}\nBuilding ${BIN_FILE}" # 在生成输出文件后显示消息
)
