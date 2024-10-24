#pragma once

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
        ADS1115_errorFatal,
        ADS1115_noError
    } ADS1115_error_t;

    /**
     * @brief 
     */
    typedef enum
    {
        ADS1115_singleShot,
        ADS1115_continousMode
    } ADS1115_mode_t;

    /**
     * @brief Construct a new ADS1115 object
     */
    ADS1115();

    /**
     * @name 
     * @brief
     * @return ADS1115_error_t 
     */
    ADS1115_error_t init();

    /**
     * @name 
     * @brief
     * @return ADS1115_error_t 
     */
    ADS1115_error_t setMode(ADS1115_mode_t mode);

    /**
     * @name 
     * @brief
     * @return ADS1115_error_t 
     */
    ADS1115_error_t setVoltRange();

    /**
     * @name 
     * @brief
     * @return ADS1115_error_t 
     */
    ADS1115_error_t setCompChannel(uint8_t compChannel);

    /**
     * @name 
     * @brief
     * @return ADS1115_error_t 
     */
    void startSingleMeasurement();

    /**
     * @name 
     * @brief
     * @return ADS1115_error_t 
     */
    float getVoltage();

    private:

    /**
     * @name 
     * @brief
     * @return ADS1115_error_t 
     */
    ADS1115_error_t writeConfigRegister();

    /**
     * @name 
     * @brief
     * @return ADS1115_error_t 
     */
    ADS1115_error_t readConvRegister();
};