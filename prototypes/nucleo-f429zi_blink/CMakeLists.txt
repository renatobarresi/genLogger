##########################################################
#                BLINK CMakeLists.txt                    #
##########################################################

#############################
#     Main variables        #
#############################
set(executable blinkTarget)
set(bsp_path ${CMAKE_CURRENT_SOURCE_DIR}/../../source/BSP/nucleo-f429zi)
set(bsp_linker ${bsp_path}/link/STM32F429ZITX_FLASH.ld)

##########################################################
#           Sources & include definitions                #
##########################################################
set(blink_sources
    src/main.cpp
    src/blinkInterface.cpp
)

set(blink_includes
    inc
)

##########################################################
#              Components definition                     #
##########################################################
# TODO: define an appropiate output folder for the output files.
add_subdirectory(
    ${bsp_path}
    ${CMAKE_BINARY_DIR}/BSP/nucleo-f429zi
    EXCLUDE_FROM_ALL
)

##########################################################
#              Target definition                         #
##########################################################
add_executable(${executable}
    ${blink_sources}
)

target_include_directories(${executable} PRIVATE
    ${blink_includes}    
)

target_link_libraries(${executable} PRIVATE 
    f429ziComponent
)

#############################
#     Compiler flags        #
#############################
set(CPU_PARAMETERS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard)

message(STATUS "bsp_linker: ${bsp_linker}")
message(STATUS "CPU_PARAMETERS: ${CPU_PARAMETERS}")
target_compile_options(${executable} PRIVATE
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

#############################
#      Linker flags         #
#############################
target_link_options(${executable} PRIVATE
    -T${bsp_linker}
    --specs=nosys.specs
    ${CPU_PARAMETERS}
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
    #-Wl,--verbose
)

#############################
#    Post build commands    #
#############################
# Post-build message
set(Objdump_path /home/renato/renato/toolchains/arm_bare_metal/arm-gnu-toolchain-13.2.rel1-x86_64-arm-none-eabi/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin)

# Assuming 'executable' is the name of your target
set(output_file "${CMAKE_CURRENT_BINARY_DIR}/${executable}.objdump")

# Objdump
add_custom_command(TARGET ${executable} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Build complete. Generating objdump file."
    COMMAND ${Objdump_path}/arm-none-eabi-objdump -h ${CMAKE_CURRENT_BINARY_DIR}/${executable} > ${executable}_objdump.txt
)

# Objdump
add_custom_command(TARGET ${executable} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Generating dissasemble file."
    COMMAND ${Objdump_path}/arm-none-eabi-objdump -d ${CMAKE_CURRENT_BINARY_DIR}/${executable} > ${executable}_dissasemble.txt
)