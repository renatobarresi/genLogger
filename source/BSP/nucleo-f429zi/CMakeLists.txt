##########################################################
#                Nucleo-F429zi CMakeLists.txt            
##########################################################
# This CMake file sets up the build configuration for the 
# Nucleo-F429zi target, specifying source files, include 
# directories, compiler options, and linker settings.
##########################################################

##########################################################
#                    Main variables                            
##########################################################

# Component name for the Nucleo-F429zi board support package (BSP)
set(bsp_nucleo_f429zi_component f429ziComponent)

# Paths to the STM32CubeF4 libraries for HAL and CMSIS drivers
set(stm32f4_hal_path ${dependencies_path}/stm32cubef4-src/Drivers/STM32F4xx_HAL_Driver)
set(stm32f4_cmsis_path ${dependencies_path}/stm32cubef4-src/Drivers/CMSIS)

##########################################################
#           Sources & include definitions                
##########################################################

# Paths to the include directories for the project initialization files
set(f429zi_init_includes
    inc
)

# Paths to the include directories for STM32Cube HAL and CMSIS libraries
set(st_hal_includes
    ${stm32f4_cmsis_path}/Include
    ${stm32f4_cmsis_path}/Device/ST/STM32F4xx/Include
    ${stm32f4_hal_path}/Inc
    ${stm32f4_hal_path}/Inc/Legacy
)

# List of source files from STM32 HAL libraries used in the project
set(st_hal_sources
    ${stm32f4_hal_path}/Src/stm32f4xx_hal.c
    ${stm32f4_hal_path}/Src/stm32f4xx_hal_cortex.c
    ${stm32f4_hal_path}/Src/stm32f4xx_hal_rcc.c
    ${stm32f4_hal_path}/Src/stm32f4xx_hal_gpio.c
    ${stm32f4_hal_path}/Src/stm32f4xx_hal_dma.c
    ${stm32f4_hal_path}/Src/stm32f4xx_hal_uart.c
    ${stm32f4_hal_path}/Src/stm32f4xx_hal_i2c.c
    ${stm32f4_hal_path}/Src/stm32f4xx_hal_i2c_ex.c
)

# List of source files for initializing and setting up the Nucleo-F429zi board
set(f429zi_init_sources
    src/interrupts.c
    src/leds.c
    src/init.c
    src/syscalls.c
    src/sysmem.c
    src/system_stm32f4xx.c
    src/startup_stm32f429xx.s
    src/i2c_drv.c
    src/uart.c
)

##########################################################
#              Target definition                         
##########################################################

# Define the library target for the Nucleo-F429zi BSP component as an OBJECT library
add_library(${bsp_nucleo_f429zi_component} OBJECT 
    ${f429zi_init_sources}
    ${st_hal_sources}
)

# Add the STM32 HAL include directories as system paths for this target
target_include_directories(${bsp_nucleo_f429zi_component} SYSTEM PRIVATE
    ${st_hal_includes}
)

# Add the project-specific include directories as public include paths
target_include_directories(${bsp_nucleo_f429zi_component} PUBLIC
    ${f429zi_init_includes}
)

# Define preprocessor macros for the STM32F429xx microcontroller
target_compile_definitions(${bsp_nucleo_f429zi_component} PRIVATE
    -DSTM32F429xx
)

#############################
#     Compiler flags        
#############################

# CPU and FPU parameters for the ARM Cortex-M4 microcontroller
set(CPU_PARAMETERS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard)

# Linker script for the STM32F429ZI microcontroller's flash memory
set(bsp_linker ${CMAKE_CURRENT_SOURCE_DIR}/link/STM32F429ZITX_FLASH.ld)

# Define compile options for the target, including optimization, warnings, and debugging
target_compile_options(${bsp_nucleo_f429zi_component} PRIVATE
    ${CPU_PARAMETERS}
    -Wall                       # Enable most compiler warnings
    -Wextra                     # Enable extra warnings
    -Werror                     # Treat warnings as errors
    -Wpedantic                  # Enforce strict ANSI compliance
    -Wshadow                    # Warn about shadowed variables
    --specs=nano.specs          # Use smaller C library for embedded systems
    -Wdouble-promotion          # Warn on implicit double to float conversions
    -Wformat=2 -Wformat-truncation  # Warn about format string issues
    -Wundef                     # Warn on undefined macros
    -fno-common                 # Prevent multiple definition issues
    -Wno-unused-parameter       # Suppress unused parameter warnings
    -ffunction-sections         # Place each function in its own section
    -fdata-sections             # Place each data item in its own section
    -fstack-usage               # Generate stack usage information
    $<$<COMPILE_LANGUAGE:CXX>:
        -Wconversion             # Warn on data conversions in C++
        -Wno-volatile            # Suppress volatile warnings in C++
        -Wold-style-cast         # Warn on old-style C casts in C++
        -Wuseless-cast           # Warn on unnecessary casts
        -Wsuggest-override       # Suggest overriding virtual functions
        -std=gnu++17             # Set C++ standard to GNU++17
        -fno-exceptions          # Disable exceptions in C++
        -fno-rtti                # Disable runtime type information in C++
        -fno-use-cxa-atexit>     # Avoid usage of cxa_atexit
    $<$<CONFIG:Debug>:-g3 -ggdb> # Full debug symbols for Debug configuration
    $<$<CONFIG:Release>:-Og -g0> # Optimize for size and no debugging in Release configuration
)
