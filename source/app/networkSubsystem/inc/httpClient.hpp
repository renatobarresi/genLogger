#pragma once

#include "processing_manager.hpp"

class httpClient
{
  public:
	bool connect(const char*& serverLink);
};