#pragma once

#include <cstdint>

namespace sensor
{

enum class pluviometerType
{
	DAVIS,
	// Add more pluviometer types here
};

class pluviometer
{
  public:
	virtual bool	 init()	   = 0;
	virtual uint16_t getRain() = 0;
};

class davisPluviometer : public pluviometer
{
  public:
	virtual bool	 init() override;
	virtual uint16_t getRain() override;
};

class pluviometerFactory
{
  public:
	static sensor::pluviometer* createPluviometer(pluviometerType type)
	{
		switch (type)
		{
			case pluviometerType::DAVIS:
			{
				static sensor::davisPluviometer davis;
				return &davis;
			}
			// case PluviometerType::OTHER: ...
			default:
				return nullptr;
		}
	}
};

} // namespace sensor