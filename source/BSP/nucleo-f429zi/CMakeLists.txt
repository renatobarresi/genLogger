##########################################################
#                Nucleo-F429zi CMakeLists.txt            #
##########################################################

##########################################################
#              Main variables                            #
##########################################################

set(bsp_nucleo_f429zi_component f429ziComponent)

set(stm32f4_hal_path ${dependencies_path}/stm32cubef4-src/Drivers/STM32F4xx_HAL_Driver)
set(stm32f4_cmsis_path ${dependencies_path}/stm32cubef4-src/Drivers/CMSIS)

##########################################################
#           Sources & include definitions                #
##########################################################

set(f429zi_init_includes
    inc
)

set(st_hal_includes
    ${stm32f4_cmsis_path}/Include
    ${stm32f4_cmsis_path}/Device/ST/STM32F4xx/Include
    ${stm32f4_hal_path}/Inc
    ${stm32f4_hal_path}/Inc/Legacy
)

set(st_hal_sources
    ${stm32f4_hal_path}/Src/stm32f4xx_hal.c
    ${stm32f4_hal_path}/Src/stm32f4xx_hal_cortex.c
    ${stm32f4_hal_path}/Src/stm32f4xx_hal_rcc.c
    ${stm32f4_hal_path}/Src/stm32f4xx_hal_gpio.c
)

set(f429zi_init_sources
    src/interrupts.c
    src/leds.c
    src/init.c
    src/syscalls.c
    src/sysmem.c
    src/system_stm32f4xx.c
    src/startup_stm32f429xx.s
)

##########################################################
#              Target definition                         #
##########################################################

add_library(${bsp_nucleo_f429zi_component} OBJECT 
    ${f429zi_init_sources}
    ${st_hal_sources}
)

target_include_directories(${bsp_nucleo_f429zi_component} SYSTEM PRIVATE
    ${st_hal_includes}
)

target_include_directories(${bsp_nucleo_f429zi_component} PUBLIC
    ${f429zi_init_includes}
)

target_compile_definitions(${bsp_nucleo_f429zi_component} PRIVATE
    -DSTM32F429xx
)

#############################
#     Compiler flags        #
#############################

set(CPU_PARAMETERS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard)

set(bsp_linker ${CMAKE_CURRENT_SOURCE_DIR}/link/STM32F429ZITX_FLASH.ld)

message(STATUS "bsp_linker: ${bsp_linker}")
message(STATUS "CPU_PARAMETERS: ${CPU_PARAMETERS}")
target_compile_options(${bsp_nucleo_f429zi_component} PRIVATE
    ${CPU_PARAMETERS}
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
    $<$<CONFIG:Debug>:-g3 -ggdb>
    $<$<CONFIG:Release>:-Og -g0>
)