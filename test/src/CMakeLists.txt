set(this exampleTests)

set(sources
    exampleTests.cpp
)

add_executable(${this} 
    ${sources}
)

target_link_libraries(${this} PUBLIC
    gtest_main
    tests
)

add_test(
    NAME ${this}
    COMMAND ${this}
) 