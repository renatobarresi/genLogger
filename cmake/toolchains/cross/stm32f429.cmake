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

##################################
#      Definition validations    #
##################################

# Check if the 'executable' target is defined
if (NOT TARGET ${executable})
    message(FATAL_ERROR "The executable target '${executable}' is not defined. Please ensure that the target is correctly created before this point.")
endif()

# Check if 'bsp_linker' (linkerPath) is provided
if (NOT DEFINED bsp_linker)
    message(FATAL_ERROR "The variable 'bsp_linker' (linker path) is not defined. Please provide a valid path to the linker script.")
endif()

#############################
#   General definitions     #
#############################
# Name of the operating system which CMake is to build (generic is for bare metal system).
set(CMAKE_SYSTEM_NAME Generic)
                                
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CPU_NAME cortex-m4)
set(targetToolChainPath /home/renato/renato/toolchains/arm_bare_metal/arm-gnu-toolchain-13.2.rel1-x86_64-arm-none-eabi/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin)
# This path comes from the cmakelists thats using this .cmake file, and should be provided by the user.
set(linkerPath ${bsp_linker}) 
set(cpu_parameters 
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

#############################
#   binutils definitions    #
#############################
set(CMAKE_C_COMPILER ${targetToolChainPath}/arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${targetToolChainPath}/arm-none-eabi-g++)
set(as ${targetToolChainPath}/arm-none-eabi-as)
set(CMAKE_AR ${targetToolChainPath}/arm-none-eabi-ar)
set(OBJCOPY ${targetToolChainPath}/arm-none-eabi-objcopy)
set(OBJDUMP ${targetToolChainPath}/arm-none-eabi-objdump)
set(SIZE ${targetToolChainPath}/arm-none-eabi-size)

#############################
#       CMake related       #
#############################
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Test compiles will use static libraries, so we won't need to define linker flags
# and scripts for linking to succeed
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

#############################
#     Compiler flags        #
#############################
target_compile_options(${executable} PRIVATE
    ${cpu_parameters}
    -Wall
    -Wextra
    -Werror
    -Wpedantic
    -Wshadow
    --specs=nano.specs
    -Wdouble-promotion
    -Wformat=2 -Wformat-truncation
    -Wundef
    -fno-common
    -Wno-unused-parameter
    -ffunction-sections
    -fdata-sections
    -fstack-usage
    $<$<COMPILE_LANGUAGE:CXX>:
        -Wconversion
        -Wno-volatile
        -Wold-style-cast
        -Wuseless-cast
        -Wsuggest-override
        -std=gnu++17
        -fno-exceptions
        -fno-rtti
        -fno-use-cxa-atexit>
    $<$<CONFIG:Debug>:-Og -g3 -ggdb>
    $<$<CONFIG:Release>:-Og -g0>
)

target_compile_definitions(${executable} PRIVATE
    -DSTM32F429xx # If using ST HAL
) 

#############################
#      Linker flags         #
#############################
target_link_options(${executable} PRIVATE
    -T${bsp_linker}
    --specs=nosys.specs
    ${cpu_parameters}
    -Wl,-Map=${executable}.map
    -Wl,--gc-sections 
    -static
    --specs=nano.specs
    -Wl,--start-group
    -lc
    -lm
    -lstdc++
    -lsupc++
    -Wl,--end-group
    -Wl,--print-memory-usage
)