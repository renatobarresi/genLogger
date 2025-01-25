/**
 * @file internalStorage_component.hpp
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief
 * @version 0.1
 * @date 2025-01-24
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

////////////////////////////////////////////////////////////////////////
//							    Includes
////////////////////////////////////////////////////////////////////////

#include "config_mediator.hpp"
#include "loggerMetadata.hpp"
#include <cstdint>

/**
 * @brief A component representing the internal storage system.
 *
 */
class internalStorageComponent : public configComponent
{
	// Functions that it should have
	// retrieveMetadata
	// storeMetadata
  public:
	internalStorageComponent();
	int8_t retrieveMetadata();
	void storeMetadata();

  private:
	struct loggerMetadata* thisMetadata;
};