message(STATUS "Compiling using gnu gcc")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_C_COMPILER gcc)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER g++)
set(as as)
set(CMAKE_AR ar)
set(OBJCOPY objcopy)
set(OBJDUMP objdump)
set(SIZE size)