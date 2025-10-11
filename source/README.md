# Folder Description

This is general overview of how dependencies are managed: app -> platform -> virtualDevices -> drivers -> microcontroller

* app: bussiness logic of the product is implemented here
* platform: wrapper for elements that need distinction between host or target build (also component interfaces are defined here (e.g littleFS interface))
* middleware: some middleware are not possible to download using cmake.
* virtualDevices: all devices have an upper layer, which abstracts the hardware, this upper layer, or interface, is implemented here.
* drivers: peripherals and external devices handlers are implemented here, e.g ADS1115, SPI SD card driver, etc.
* microcontroller: microcontroller dependent code is here, e.g linker script, startup, peripheral, clock and interrupts management.