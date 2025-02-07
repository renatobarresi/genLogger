##########################################################
#                 Tests CMakeLists.txt                  
##########################################################

set(this tests)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(headers
    test.hpp
)

set(sources
    test.cpp
)

add_library(${this} STATIC ${sources} ${headers})

add_subdirectory(src)