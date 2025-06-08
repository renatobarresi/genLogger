/*
 * W25Qx_module.h
 *
 *  Created on: Apr 5, 2023
 *      Author: renatobarresi
 */
#pragma once

#ifdef __cplusplus

#include "spi_drv.h"
#include <cstdint>

/**
 * @brief W25Q64 SPI Flash Memory Class.
 */
class W25Q64
{
  public:
	/**
	 * @brief Constructor for W25Q64 class.
	 */
	W25Q64();

	/**
	 * @brief Initializes the W25Q64 Flash Memory.
	 * @return The initialization status.
	 */
	bool init();

	/**
	 * @brief Enables write operations.
	 */
	void write_enable();

	/**
	 * @brief Disables write operations.
	 */
	void write_disable();

	/**
	 * @brief Reads the status register.
	 * @return The value of the status register.
	 */
	uint8_t read_status_register();

	/**
	 * @brief Writes the status register.
	 * @param status The value to be written to the status register.
	 */
	void write_status_register(uint8_t status);

	/**
	 * @brief Performs a chip erase operation.
	 */
	void chip_erase();

	/**
	 * @brief Performs a sector erase operation at the specified address.
	 * @param addr The address of the sector to be erased.
	 */
	void sector_erase(uint32_t addr);

	/**
	 * @brief Performs a block erase operation at the specified address.
	 * @param addr The address of the block to be erased.
	 */
	void block_erase(uint32_t addr);

	/**
	 * @brief Performs a page program operation at the specified address.
	 * @param addr The starting address for page programming.
	 * @param data Pointer to the data buffer to be programmed.
	 * @param size The size of the data buffer.
	 */
	void page_program(uint32_t addr, uint8_t* data, uint16_t size);

	/**
	 * @brief Reads data from the specified address.
	 * @param addr The starting address for data read.
	 * @param data Pointer to the buffer to store the read data.
	 * @param size The size of the buffer.
	 */
	void read_data(uint32_t addr, uint8_t* data, uint16_t size);

	/**
	 * @brief Waits until the flash memory is ready for the next operation.
	 */
	void wait_until_ready();

	/**
	 * @brief Reads the JEDEC ID of the flash memory.
	 */
	void read_JDEC();

  private:
	/**
	 * @brief Selects the chip by pulling the chip select pin low.
	 */
	void cs_select();

	/**
	 * @brief Deselects the chip by pulling the chip select pin high.
	 */
	void cs_deselect();

	// Interface TX
	uint16_t (*write)(uint8_t*, uint16_t);

	// Interface RX
	uint16_t (*read)(uint8_t*, uint16_t);

	// Interface TxRx
	uint16_t (*writeRead)(uint8_t*, uint8_t*, uint16_t);

	// Interface GPIO
	void (*writePin)(SPI_Devices_t, uint8_t);
};

#endif
