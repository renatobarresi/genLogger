################################################
#             testSuite CMakeLists.txt                   
################################################

set(this generalTests)
set(sourceDirectory ${CMAKE_CURRENT_SOURCE_DIR}/../../source)

set(includes
    ${sourceDirectory}/platform/inc/
    ${sourceDirectory}/virtualDevices/inc/
    ${sourceDirectory}/app/configurationSubsystem/inc/
    ${sourceDirectory}/drivers/rtc/inc/
    ${sourceDirectory}/app/loggerMetadata/
    ${sourceDirectory}/app/main/inc/
    ${sourceDirectory}/app/utilities/inc/
)

set(sources
    testSuiteFS.cpp
    ${sourceDirectory}/app/configurationSubsystem/src/terminal_component.cpp
    ${sourceDirectory}/drivers/rtc/src/host_rtc.cpp
    ${sourceDirectory}/app/loggerMetadata/loggerMetadata.cpp
    ${sourceDirectory}/app/configurationSubsystem/src/internalStorage_component.cpp
    ${sourceDirectory}/app/configurationSubsystem/src/config_manager.cpp
    ${sourceDirectory}/app/main/src/errorHandler.cpp
    ${sourceDirectory}/app/utilities/src/utilities.cpp 
)

add_executable(${this} 
    ${sources}
)

target_include_directories(${this} PRIVATE
    ${includes}
)

target_link_libraries(${this} PUBLIC
    gtest_main
    tests
)

add_test(
    NAME ${this}
    COMMAND ${this}
) 