#pragma once
/**
 * @file ads1115.hpp
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-10-30
 *
 * @copyright Copyright (c) 2024
 *
 */

/***************
 *   Includes   *
 ***************/

#include <cstdint>

/***************
 *   Class      *
 ***************/

/**
 * @name ADS1115
 * @brief Class representing the ADS1115 ADC
 */
class ADS1115
{
  public:
	/**
	 * @brief
	 */
	typedef enum
	{
		errorFatal,
		noError
	} error_t;

	/**
	 * @brief
	 */
	typedef enum
	{
		singleShot,
		continousMode
	} mode_t;

	typedef enum
	{

	} compareChannel_t;

	typedef enum
	{
		range_6144,
		range_4096,
		range_2048,
		range_1024,
		range_0512,
		range_0256
	} voltageRange_t;

	uint16_t devAddress;

	/**
	 * @brief Construct a new ADS1115 object
	 */
	ADS1115();

	/**
	 * @name
	 * @brief
	 * @return error_t
	 */
	error_t init();

	/**
	 * @name
	 * @brief
	 * @return error_t
	 */
	error_t setMode(mode_t mode);

	/**
	 * @name
	 * @brief
	 * @return error_t
	 */
	error_t setVoltRange(voltageRange_t range);

	/**
	 * @name
	 * @brief
	 * @return error_t
	 */
	error_t setCompChannel(compareChannel_t compChannel);

	/**
	 * @name
	 * @brief
	 * @return error_t
	 */
	void startSingleMeasurement();

	/**
	 * @name
	 * @brief
	 * @return error_t
	 */
	float getVoltage();

  protected:
	virtual int i2cTx(uint16_t memAddr, uint8_t* pData, uint16_t size) = 0;
	virtual int i2cRx(uint16_t memAddr, uint8_t* pBuf, uint16_t size) = 0;

  private:
	ads1115I2cInterface i2cInterface;
	/**
	 * @name
	 * @brief
	 * @return error_t
	 */
	error_t writeConfigRegister();

	/**
	 * @name
	 * @brief
	 * @return error_t
	 */
	error_t readConvRegister();
};