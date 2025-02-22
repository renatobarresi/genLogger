#pragma once

#include "rtcInterface.hpp"
#include <cstdint>

class stm429RTC : public rtcInterface
{
  public:
	bool init() override;
	bool setTime(uint8_t hour, uint8_t minute, uint8_t seconds) override;
	void getTime(char* buffer, size_t bufferSize) const override;
	bool setDate(uint8_t day, uint8_t month, uint16_t year) override;
	void getDate(char* buffer, size_t bufferSize) const override;
};