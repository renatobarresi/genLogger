##########################################################
#                BLINK CMakeLists.txt                    
##########################################################

#############################
#     Main variables        
#############################
set(executable nuttx_blink)
#set(bsp_path ${CMAKE_CURRENT_SOURCE_DIR}/../../source/BSP/nucleo-f429zi)

##########################################################
#           Sources & include definitions                
##########################################################
set(${executable}_sources
    src/main.cpp
    src/helloWorld.cpp
)

set(${executable}_includes
    inc
)

##########################################################
#              Components definition                     
##########################################################
set(nuttx_path ${CMAKE_SOURCE_DIR}/nuttx)

set(nuttx_includes
    ${nuttx_path}/include
    ${nuttx_path}/include/libcxx
    ${nuttx_path}/arch/chip
)

link_directories(${executable} 
    ${nuttx_path}/libs
)

set(bsp_linker ${nuttx_path}/scripts/ld.script)
##########################################################
#              Target definition                         
##########################################################
add_executable(${executable}
    ${${executable}_sources}
)

target_include_directories(${executable} PRIVATE
    ${${executable}_includes}    
)

target_link_libraries(${executable} PRIVATE 
    #--start-group
    sched
    drivers
    boards
    c
    mm
    arch
    xx
    apps
    fs
    binfmt
    board
    gcc
    supc++
    #--end-group
)

#############################
#     Compiler flags        
#############################
set(CPU_PARAMETERS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard)

target_compile_definitions(${executable} PRIVATE
    -DCONFIG_WCHAR_BUILTIN
)

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
        -fno-strict-aliasing
        -fno-exceptions
        -fno-rtti
        -fcheck-new
        -pedantic
        -nostdinc++
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