/**
 * @file config_manager.hpp
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief Header file for the configuration manager, implementing the mediator pattern for managing
 * interactions between components.
 * @version 0.1
 * @date 2025-01-24
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

////////////////////////////////////////////////////////////////////////
//								Includes
////////////////////////////////////////////////////////////////////////

#include "config_mediator.hpp"
#include "internalStorage_component.hpp"
#include "terminal_component.hpp"

////////////////////////////////////////////////////////////////////////
//							Class definition
////////////////////////////////////////////////////////////////////////

/**
 * @brief Configuration manager implementing the mediator pattern.
 *
 * The configManager class serves as a mediator to facilitate communication between various
 * configuration components, such as the terminal state machine and internal storage.
 */
class configManager : public configManagerInterface
{
  private:
	terminalStateMachine*	  outputTerminal;  ///< Pointer to the terminal state machine component.
	internalStorageComponent* internalStorage; ///< Pointer to the internal storage component.

  public:
	/**
	 * @brief Construct a new configManager object.
	 *
	 * Initializes the configuration manager with references to the terminal state machine and
	 * internal storage components.
	 *
	 * @param[in] terminal Pointer to the terminal state machine component.
	 * @param[in] storage Pointer to the internal storage component.
	 */
	configManager(terminalStateMachine* terminal, internalStorageComponent* storage);

	/**
	 * @brief Notify the mediator about an event from a specific component.
	 *
	 * Handles events triggered by configuration components and facilitates communication
	 * between them as needed.
	 *
	 * @param[in] sender Pointer to the component that triggered the event.
	 * @param event Character representing the type of event.
	 * @param[in] data Character representing additional data associated with the event.
	 */
	void notify(configComponent* sender, mediatorEvents event, const char* data) const override;
};
