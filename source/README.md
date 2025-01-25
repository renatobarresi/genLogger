# Description

Since the architecture is a layered one, the subfolders follow this concept as well.

* App: bussiness logic of the product is implemented here
* Platform: wrapper for elements that need distinction between host or target build 
* VirtualDevices: all devices have an upper layer, which abstracts the hardware, this upper layer, or interface is implemented here.
* Drivers: peripherals and external devices handlers are implemented here, e.g ADS1115, SPI SD card driver, etc.
* BSP: hardware dependent code is here, e.g linker script, startup, peripheral, clock and interrupts management.