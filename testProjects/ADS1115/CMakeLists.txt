set(toolchain_cmake_path ../../cmake/toolchains/cross)
set(ads1115_header inc)
set(ads1115_src src)
set(bsp_path ${CMAKE_CURRENT_SOURCE_DIR}/../../source/BSP/nucleo-f429zi)
set(bsp_header ${bsp_path}/inc)
set(bsp_init_source ${bsp_path}/src)
set(bsp_linker ${bsp_path}/link/STM32F429ZITX_FLASH.ld)
set(executable ads1115_test)

add_executable(${executable} ${ads1115_src}/main.cpp ${ads1115_src}/ads1115.cpp ${bsp_init_source}/stm32f429zi_init.cpp)

target_include_directories(${executable} PRIVATE ${ads1115_header} ${bsp_header})

include(${toolchain_cmake_path}/stm32f429.cmake)