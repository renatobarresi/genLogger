#pragma once

#include "virtualCounter.hpp"
#include <cstdint>

namespace sensor
{

template<typename P>
class IPluviometer
{
  public:
	uint16_t getRain()
	{
		return static_cast<P*>(this)->getRainImpl();
	}
};

class davisPluviometer : public IPluviometer<davisPluviometer>
{
  public:
	uint16_t getRainImpl()
	{
		return pluviometerConstant * counterObj.readandResetCounter(); // todo
	}

  private:
	virtualDevice::virtualCounter counterObj;
	static constexpr float		  pluviometerConstant = 0.2; // maybe this should be possible to write during run time if there's a change in the pluviometer type
};

} // namespace sensor