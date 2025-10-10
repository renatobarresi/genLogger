#include "aht21_wrapper.hpp"
#include "debug_log.hpp"
#include "i2c_drv.h"
#include "timer.h"

/**
 * @brief i2c_init_adapter is a wrapper for the i2c_init function
 * @return 0 on success, 1 on failure
 */
static uint8_t i2c_init_adapter()
{
	return (i2c_init() == 0) ? 0 : 1;
}
/**
 * @brief i2c_deinit_adapter is a wrapper for the i2c_deinit function
 * @return 0 on success, 1 on failure
 */
static uint8_t i2c_deinit_adapter()
{
	return (i2c_deinit() == 0) ? 0 : 1;
}
/**
 * @brief i2c_read_adapter is a wrapper for the i2c_read function
 * @param[in] addr is the i2c address
 * @param[out] *buf points to a buffer to store the read data
 * @param[in] len is the length of the data to be read
 * @return 0 on success, 1 on failure
 */
static uint8_t i2c_read_adapter(uint8_t addr, uint8_t* buf, uint16_t len)
{
	return (i2c_read(addr, 0, buf, len) == 0) ? 0 : 1;
}
/**
 * @brief i2c_write_adapter is a wrapper for the i2c_write function
 * @param[in] addr is the i2c address
 * @param[in] *buf points to a buffer containing the data to be written
 * @param[in] len is the length of the data to be written
 * @return 0 on success, 1 on failure
 */
static uint8_t i2c_write_adapter(uint8_t addr, uint8_t* buf, uint16_t len)
{
	return (i2c_write(addr, 0, buf, len) == 0) ? 0 : 1;
}

namespace sensor::thermometer
{

static void aht21_print(const char* const fmt, ...)
{
	debug::log<true, debug::logLevel::LOG_ALL>(fmt);
}

bool AHT21::initImpl()
{
	DRIVER_AHT21_LINK_INIT(&_gs_handle, aht21_handle_t);
	DRIVER_AHT21_LINK_IIC_INIT(&_gs_handle, i2c_init_adapter);
	DRIVER_AHT21_LINK_IIC_DEINIT(&_gs_handle, i2c_deinit_adapter);
	DRIVER_AHT21_LINK_IIC_READ_CMD(&_gs_handle, i2c_read_adapter);
	DRIVER_AHT21_LINK_IIC_WRITE_CMD(&_gs_handle, i2c_write_adapter);
	DRIVER_AHT21_LINK_DELAY_MS(&_gs_handle, system_sleep);
	DRIVER_AHT21_LINK_DEBUG_PRINT(&_gs_handle, aht21_print);

	uint8_t res = aht21_init(&_gs_handle);
	if (res != 0)
	{
		printf("aht21: init failed.\n");

		return false;
	}

	return true;
}

std::optional<float> AHT21::readTemperatureImpl()
{
	float	 temperature;
	uint32_t temperature_raw;

	/* read temperature and humidity */
	if (aht21_read_temperature(&_gs_handle, (uint32_t*)&temperature_raw, &temperature) != 0)
	{
		return std::nullopt;
	}

	return temperature;
}

std::optional<uint8_t> AHT21::readHumidityImpl()
{
	uint32_t humidity_raw;
	uint8_t	 humidity;

	/* read temperature and humidity */
	if (0 != aht21_read_humidity(&_gs_handle, &humidity_raw, &humidity))
	{
		return std::nullopt;
	}

	return humidity;
}
} // namespace sensor::thermometer