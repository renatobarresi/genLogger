/**
 * @file config_manager.cpp
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief
 * @version 0.1
 * @date 2025-01-24
 *
 * @copyright Copyright (c) 2025
 *
 */

////////////////////////////////////////////////////////////////////////
//							    Includes
////////////////////////////////////////////////////////////////////////

#include "config_manager.hpp"
#include "config_mediator.hpp"

////////////////////////////////////////////////////////////////////////
//					   Public methods implementation
////////////////////////////////////////////////////////////////////////

configManager::configManager(terminalStateMachine* terminal, internalStorageComponent* storage)
	: outputTerminal(terminal), internalStorage(storage)
{
	this->outputTerminal->setMediator(this);
	this->internalStorage->setMediator(this);
}

void configManager::notify(configComponent* sender, mediatorEvents event, const char* data) const
{
	if(event == mediatorEvents::UPDATE_METADATA)
	{
		// Notify internal storage
		this->internalStorage->retrieveMetadata();
	}

	if(event == mediatorEvents::METADATA_UPDATED)
	{
		// Signal terminal
	}
}