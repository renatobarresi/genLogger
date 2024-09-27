# STM32F429 cross-compiling

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CPU_NAME cortex-m4)
set(targetToolChainPath /home/renato/renato/toolchains/arm_bare_metal/arm-gnu-toolchain-13.2.rel1-x86_64-arm-none-eabi/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin)

set(CMAKE_C_COMPILER ${targetToolChainPath}/arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${targetToolChainPath}/arm-none-eabi-g++)
set(as ${targetToolChainPath}/arm-none-eabi-as)
set(CMAKE_AR ${targetToolChainPath}/arm-none-eabi-ar)
set(OBJCOPY ${targetToolChainPath}/arm-none-eabi-objcopy)
set(OBJDUMP ${targetToolChainPath}/arm-none-eabi-objdump)
set(SIZE ${targetToolChainPath}/arm-none-eabi-size)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Test compiles will use static libraries, so we won't need to define linker flags
# and scripts for linking to succeed
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Compiler flags


# Linker flags
