# Project description

genLogger (generic datalogger) is a device capable of communicating with multiple sensors, process the data, store the information and transmit it to the cloud. 
Could be used in conjunction with weather sensors to create a weather station, or with air quality sensors, to create an air quality station, etc.

# Repository description

This is the repository for the "genLogger" product/project, which is part of my Thesis for FIUBA's CESE [(their website)](https://lse-posgrados.fi.uba.ar/posgrados/especializaciones/cese).
Here you will find the source code of the project, the build system and the documentation to be able to modify, test and build your own firmware.

# Folder structure

* cmake: Files (toolchain configuration files, libraries, etc) relevant to the build system CMake
* prototypes: Small projects (source files) used to test some features of the genLogger device
* source: All the source code (source files and header files) are located in this folder and its subfolders
* test: unit tests for the source code
* tools: dockerfiles and other tools used to help with the development

# Build system description

The build system generator used is CMake, configured to generate makefiles for the host and the target.

## Build instructions

After making sure all the requirements are met:

* Create a build folder named "build" in the root directory
* go to to build
* To create a host makefile, execute
    - cmake .. -Dtarget_type=host 
* To create a target (stm32f429zi) makefile, execute
    - cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/cross/stm32f429.cmake -DCMAKE_BUILD_TYPE=Debug -Dtarget_type=target
