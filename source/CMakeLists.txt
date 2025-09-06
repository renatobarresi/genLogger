##########################################################
#                  APP CMakeLists.txt                    #
##########################################################
# TODO: define an appropiate output folder for the output files.

#############################
#     Main variables        #
#############################
set(executable app)
set(microcontroller_path ${CMAKE_CURRENT_SOURCE_DIR}/microcontroller/stm32f429zi)
set(bsp_linker ${microcontroller_path}/link/STM32F429ZITX_FLASH.ld)

##########################################################
#           Sources & include definitions                #
##########################################################
################
#    Sources
################
set(app_sources
    app/main/src/main.cpp
    app/main/src/errorHandler.cpp
    app/configurationSubsystem/src/config_manager.cpp
    app/configurationSubsystem/src/terminal_component.cpp
    app/configurationSubsystem/src/internalStorage_component.cpp
    app/loggerMetadata/loggerMetadata.cpp
    app/utilities/src/utilities.cpp
    app/loggerSubsystem/src/logger_manager.cpp
    app/networkSubsystem/src/networkManager.cpp
    app/networkSubsystem/src/httpClient.cpp
)

set(platform_sources
    platform/src/serialHandler.cpp
)

set(driver_sources)

set(dependencies_sources)

set(virtualDevice_sources
    virtualDevices/src/virtualCounter.cpp
)

set(middleware_sources
    middleware/mongoose/mongoose.c
)

# Special target specific sources
if (target_type STREQUAL "target")
    LIST(APPEND platform_sources
        platform/src/littleFSInterface.cpp
        platform/src/mongooseUtilities.c
    )

    LIST(APPEND driver_sources
        drivers/W25Qx/src/W25Qx_module.cpp
        drivers/rtc/src/stm32F429_RTC.cpp
        drivers/SDCard/sd_spi.c
    )

    LIST(APPEND dependencies_sources
        ${dependencies_path}/little-fs-src/lfs_util.c
        ${dependencies_path}/little-fs-src/lfs.c
    )

    LIST(APPEND middleware_sources
        middleware/FatFS/diskio.c
        middleware/FatFS/ff_gen_drv.c
        middleware/FatFS/ff.c
        middleware/FatFS/interface/fatfs.c
        middleware/FatFS/interface/user_diskio.c
        middleware/FatFS/option/cc949.c
    )
endif()

# Special target specific host
if (target_type STREQUAL "host")
    LIST(APPEND driver_sources
        drivers/rtc/src/host_rtc.cpp
        app/measurementSubsystem/sensors/sensorSimulator/sensorSimulatorConsumer.cpp
    )
endif()

################
#    Includes
################
set(app_includes
    app/main/inc/
    app/configurationSubsystem/inc/
    app/loggerMetadata/
    app/utilities/inc/
    app/measurementSubsystem/inc/
    app/loggerSubsystem/inc/
    app/measurementSubsystem/sensors/sensorSimulator/
    app/measurementSubsystem/sensors/inc/
    app/networkSubsystem/inc/
)

set(platform_includes
    platform/inc/
)

set(driver_includes
    # Real time clock  
    drivers/rtc/inc/
    # W25Qx driver
    drivers/W25Qx/inc/
    drivers/SDCard/
)

set(dependencies_includes)

set(virtualDevices_includes
    virtualDevices/inc/
)

set(middleware_includes
    middleware/mongoose/
)

# Special target specific includes
if (target_type STREQUAL "target")
    list(APPEND driver_includes
        # W25Qx driver
        # drivers/W25Qx/inc/
        drivers/SDCard/
    )
    list(APPEND dependencies_includes
        ${dependencies_path}/little-fs-src/
    )

    list(APPEND middleware_includes
        middleware/FatFS/
        middleware/mongoose/config/
    )
endif()

if (target_type STREQUAL "host")

endif()

##########################################################
#              Components definition                     #
##########################################################

if (target_type STREQUAL "target")
    add_subdirectory(
        ${microcontroller_path}
        ./microcontroller/stm32f429zi
        EXCLUDE_FROM_ALL
    )
endif()

##########################################################
#              Target definition                         #
##########################################################
################
#    Sources
################
add_executable(${executable} 
    ${app_sources}
    ${platform_sources}
    ${driver_sources}
    ${dependencies_sources}
    ${virtualDevice_sources}
    ${middleware_sources}
)

################
#    Includes
################
target_include_directories(${executable} PRIVATE
    ${app_includes}
    ${platform_includes}
    ${virtualDevices_includes}
    ${driver_includes}
    ${dependencies_includes}
    ${middleware_includes}
)

################
#   Libraries
################
if (target_type STREQUAL "target")
    target_link_libraries(${executable} PRIVATE 
        f429ziComponent
    )
endif()

if (target_type STREQUAL "host")

endif()

#############################
#     Compiler flags        #
#############################
if (target_type STREQUAL "target")

    set(CPU_PARAMETERS
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
    )

    target_compile_definitions(${executable} PRIVATE
        -DTARGET_MICRO
    )

    target_compile_options(${executable} PRIVATE
        ${CPU_PARAMETERS}
        -Wall
        -Wextra
        #-Werror
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
        -Wno-missing-field-initializers
        $<$<COMPILE_LANGUAGE:CXX>:
            -std=c++20
            -Wconversion
            -Wno-volatile
            -Wold-style-cast
            -Wuseless-cast
            -Wsuggest-override
            -fno-exceptions
            -fno-rtti
            -fno-use-cxa-atexit      # Avoid usage of cxa_atexit
            -fno-threadsafe-statics> # no extra code for thread safe variables initialization
        $<$<CONFIG:Debug>:-g3 -ggdb>
        $<$<CONFIG:Release>:-Og -g0>
    )
endif()

if (target_type STREQUAL "host")
    # Add compile options for debugging
    target_compile_options(${executable} PRIVATE
        -g     # Include debug symbols
        -O0    # Disable optimizations for easier debugging
        -Wall  # Enable all warnings
        -Wextra # Enable extra warnings
        -Wpedantic # Enforce strict standards compliance
    )
endif()

#############################
#      Linker flags         #
#############################
if (target_type STREQUAL "target")

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
endif()


if (target_type STREQUAL "host")
    # Add linker options for debugging
    target_link_options(${executable} PRIVATE
        -g     # Pass debug symbol flag to the linker
    )
endif()

#############################
#    Post build commands    
#############################
if (target_type STREQUAL "target")

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
endif()