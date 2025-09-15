# Project description

genLogger (generic datalogger) is a device capable of communicating with multiple sensors, processing the data, storing the information, and transmitting it to the cloud.
It can be used with weather sensors to create a weather station, or with air quality sensors to create an air quality station, etc.

# Repository description

This is the repository for the "genLogger" (generic datalogger) project.
Here you will find the source code, build system, and documentation to modify, test, and build your own firmware.

# Folder structure

* `cmake`: Files relevant to the CMake build system (e.g., toolchain configuration files, libraries).
* `source`: All source code (source and header files) for the project.
* `test`: Unit tests for the source code.
* `tools`: Dockerfiles and other tools to aid in development.

# Build system description

The build system uses CMake to generate Makefiles for both host and target builds.

# Dependencies

Most dependencies are downloaded automatically by CMake. FatFS and mongoose are the exceptions, which are included as middleware in the `source` folder.

* LittleFS
* Mongoose
* FatFS
* STM32F4 HAL
* CMSIS
* Googletest


# Build instructions

After ensuring all requirements are met, you can build the firmware from the root directory.

1.  Create a build directory and navigate into it:
    ```bash
    mkdir build
    cd build
    ```

2.  Generate the Makefiles for your desired platform.

    *   **For a host build:**
        ```bash
        cmake .. -Dtarget_type=host
        ```

    *   **For a target (STM32F429ZI) build:**
        ```bash
        cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/cross/stm32f429.cmake -DCMAKE_BUILD_TYPE=Debug -Dtarget_type=target
        ```

3.  Compile the code:
    ```bash
    make
    ```

# Development Tools

A serial terminal emulator, such as Cutecom, is useful for viewing logs and interacting with the device during development.