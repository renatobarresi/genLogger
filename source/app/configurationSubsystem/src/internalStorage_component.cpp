/**
 * @file internalStorage_component.cpp
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief
 * @version 0.1
 * @date 2025-01-24
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "internalStorage_component.hpp"
#include "internalStorage_wrapper.hpp"

internalStorageComponent::internalStorageComponent()
{
	this->thisMetadata = getLoggerMetadata();
}

int8_t internalStorageComponent::retrieveMetadata()
{
	int8_t status;

	status = open("metadata.txt", 'r');

	return status;
}

void internalStorageComponent::storeMetadata()
{
	//
}