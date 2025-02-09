################################################
#             testSuite CMakeLists.txt                   
################################################

set(this testFilesystem)

set(includes
    ${CMAKE_CURRENT_SOURCE_DIR}/../../source/platform/inc
)

set(sources
    testSuiteFS.cpp
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