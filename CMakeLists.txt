##########################################################
#   Main CMakeList for the genLogger CESE final project  #
##########################################################
cmake_minimum_required(VERSION 3.30)

##########################################################
#              Global definitions                        #
##########################################################
set(projectName genLogger)

##########################################################
#              Project main definitions                  #
##########################################################
project(${projectName}
        DESCRIPTION "genLogger is a generic datalogger. FIUBA CESE final project."
        VERSION 1.0.0
        LANGUAGES C CXX ASM)   

set(CMAKE_EXPORT_COMPILE_COMMANDS yes) # for clangd "compile_comands.json"
include(CMakePrintHelpers)

##########################################################
#                  Sub directories                       #
##########################################################
add_subdirectory(source)
add_subdirectory(test)
add_subdirectory(testProjects)

##########################################################
#              Dependencies management                   #
##########################################################
# Dependencies for this project are: lwIP, ST HAL, NuttX, googleTest
# We use CPM for CMake dependency managment (https://github.com/cpm-cmake/CPM.cmake)
# ST HAL repo: https://github.com/STMicroelectronics/stm32f4xx_hal_driver.git
# lwIP repo: https://git.savannah.nongnu.org/git/lwip.git
# googleTest: https://github.com/google/googletest.git
# little-FS: https://github.com/littlefs-project/littlefs.git
##########################################################
include(cmake/CPM.cmake)

CPMAddPackage(
    NAME stm32f4xx_hal_driver
    GITHUB_REPOSITORY STMicroelectronics/stm32f4xx_hal_driver
    GIT_TAG v1.8.3
    DOWNLOAD_ONLY ON # Prevent CMake from trying to configure the project
)

#[[CPMAddPackage(
    NAME little-FS
    GITHUB_REPOSITORY littlefs-project/littlefs    
    GIT_TAG v2.9.3
    DOWNLOAD_ONLY ON # Prevent CMake from trying to configure the project
)

CPMAddPackage(
    NAME googletest
    GITHUB_REPOSITORY google/googletest
    GIT_TAG v1.15.2
    DOWNLOAD_ONLY ON # Prevent CMake from trying to configure the project
)

CPMAddPackage(
    NAME lwIP
    GIT_REPOSITORY https://git.savannah.nongnu.org/git/lwip
    GIT_TAG STABLE-2_2_0_RELEASE
    DOWNLOAD_ONLY ON # Prevent CMake from trying to configure the project
)

CPMAddPackage(
    NAME nuttX
    GITHUB_REPOSITORY apache/nuttx
    GIT_TAG nuttx-12.6.0
    DOWNLOAD_ONLY ON # Prevent CMake from trying to configure the project
)]]

# Documentation
#find_program(DOXYGEN doxygen)

#if(DOXYGEN)
#  add_custom_target(docs
#    COMMAND ${DOXYGEN} docs/Doxyfile
#    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
#  )
#endif()

# This property will remove a directory
#set_target_properties(docs PROPERTIES ADDITIONAL_CLEAN_FILES ${CMAKE_BINARY_DIR}/doc/)

# Formating 
#find_program(CLANG_FORMAT clang-format)
#if(CLANG_FORMAT)
#  add_custom_target(format
#    COMMAND tools/format.sh
#    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
#  )
#endif()

# Static analysis 
#find_program(CPPCHECK cppcheck)

# Complexity analysis 
#find_program(LIZARD lizard)

# TODO: Code coverage 

# TODO: Code sanitizer

# TODO: Package releases