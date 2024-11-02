##########################################################
#                ADS1115 CMakeLists.txt                  
##########################################################

#############################
#     Main variables        
#############################
set(executable ads1115Target)
set(bsp_path ${CMAKE_CURRENT_SOURCE_DIR}/../../source/BSP/nucleo-f429zi)
set(bsp_linker ${bsp_path}/link/STM32F429ZITX_FLASH.ld)

##########################################################
#           Sources & include definitions                
##########################################################
set(ads1115_sources
    src/main.cpp
    src/ads1115.cpp
    src/ads1115_comm_interface.cpp
)

set(ads1115_includes
    inc
)

##########################################################
#              Components definition                     
##########################################################
# TODO: define an appropiate output folder for the output files.
add_subdirectory(
    ${bsp_path}
    ${CMAKE_BINARY_DIR}/BSP/nucleo-f429zi
    EXCLUDE_FROM_ALL
)

##########################################################
#              Target definition                         
##########################################################
add_executable(${executable}
    ${ads1115_sources}
)

target_include_directories(${executable} PRIVATE
    ${ads1115_includes}
)

target_link_libraries(${executable} PRIVATE 
    f429ziComponent
)

#############################
#     Compiler flags        
#############################
set(CPU_PARAMETERS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard)

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
#      Linker flags         
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
#    Post build commands    
#############################
# Assuming 'executable' is the name of your target
set(output_file "${CMAKE_CURRENT_BINARY_DIR}/${executable}.objdump")

# Objdump
add_custom_command(TARGET ${executable} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Build complete. Generating objdump file."
    COMMAND ${OBJDUMP} -h ${CMAKE_CURRENT_BINARY_DIR}/${executable} > ${executable}_objdump.txt
)

# Objdump
add_custom_command(TARGET ${executable} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Generating dissasemble file."
    COMMAND ${OBJDUMP} -d ${CMAKE_CURRENT_BINARY_DIR}/${executable} > ${executable}_dissasemble.txt
)