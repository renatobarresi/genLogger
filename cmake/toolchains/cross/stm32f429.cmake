#############################################################
#                  STM32F429 Cross Compiler                 #
#############################################################
# STM32F429 cross-compiling                                 # 
# This file is used for cross compilation                   #
# targeted to a STM32F4 microcontroller.                    #
# You can enable cross-compilation by setting the built-in  # 
# CMAKE_TOOLCHAIN_FILE variable to the desired toolchain    # 
# script when you are configuring the build output directory#
#############################################################

# CMake includes the toolchain file multiple times when configuring the build,
# which causes errors for some flags (e.g., --specs=nano.specs).
# We prevent this with an include guard.
if(ARM_CORTEX_M4_TOOLCHAIN_INCLUDED)
	return()
endif()

set(ARM_CORTEX_M4_TOOLCHAIN_INCLUDED true)

#message(STATUS "CMAKE_CROSSCOMPILING: ${CMAKE_CROSSCOMPILING}")
message(STATUS "Cross compiling for a CORTEX M4")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

#############################
#   General definitions     #
#############################
# Name of the operating system which CMake is to build (generic is for bare metal system).
set(CMAKE_SYSTEM_NAME Generic)                                
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CPU_NAME cortex-m4)
#set(stm32f4_toolchain_path replace_with_your_toolchain_path)

#############################
#   binutils definitions    #
#############################
set(CMAKE_C_COMPILER ${stm32f4_toolchain_path}/arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${stm32f4_toolchain_path}/arm-none-eabi-g++)
set(as ${stm32f4_toolchain_path}/arm-none-eabi-as)
set(CMAKE_AR ${stm32f4_toolchain_path}/arm-none-eabi-ar)
set(OBJCOPY ${stm32f4_toolchain_path}/arm-none-eabi-objcopy)
set(OBJDUMP ${stm32f4_toolchain_path}/arm-none-eabi-objdump)
set(SIZE ${stm32f4_toolchain_path}/arm-none-eabi-size)

#############################
#       CMake related       #
#############################
# Test compiles will use static libraries, so we won't need to define linker flags
# and scripts for linking to succeed
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)