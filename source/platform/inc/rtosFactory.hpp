#pragma once

#include "rtosAdapter.hpp"

class rtosFactory
{
  public:
	virtual rtosAdapter* factory() = 0;
	virtual ~rtosFactory() {}
};