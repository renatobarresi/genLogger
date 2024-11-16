message(STATUS "Compiling using clang")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_C_COMPILER    clang)
set(CMAKE_CXX_COMPILER  clang++)
set(AR                  llvm-ar)