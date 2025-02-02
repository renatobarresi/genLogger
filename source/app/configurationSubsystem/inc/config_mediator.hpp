/**
 * @file config_mediator.hpp
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief Mediator interface and base component definitions for the configuration subsystem
 * @version 0.1
 * @date 2025-01-24
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

////////////////////////////////////////////////////////////////////////
//								Types
////////////////////////////////////////////////////////////////////////

/**
 * @brief Type of events that the mediator handles
 * 
 */
enum class mediatorEvents
{
	UPDATE_METADATA,
	METADATA_UPDATED
};

////////////////////////////////////////////////////////////////////////
//							Class definition
////////////////////////////////////////////////////////////////////////

/**
 * @brief Forward declaration of the base component for components that are part of
 * the configuration subsystem.
 */
class configComponent;

/**
 * @brief Interface for the mediator in the configuration subsystem.
 *
 * The configManagerInterface defines a contract for mediators that manage communication
 * between different configuration components.
 * @ref configManager is the mediator.
 */
class configManagerInterface
{
  public:
	/**
	 * @brief Notify the mediator about an event from a specific component.
	 *
	 * Handles events triggered by configuration components and facilitates communication
	 * between them as needed.
	 *
	 * @param sender Pointer to the component that triggered the event.
	 * @param event Character representing the type of event.
	 * @param data Character representing additional data associated with the event.
	 */
	virtual void notify(configComponent* sender, mediatorEvents event, const char* data) const = 0;

	/**
	 * @brief Virtual destructor for the mediator interface.
	 *
	 * Ensures proper cleanup of derived classes.
	 */
	virtual ~configManagerInterface() = default;
};

/**
 * @brief Base class for all components that are part of the configuration subsystem.
 *
 * The configComponent class provides a common interface for configuration components
 * and a reference to the mediator for facilitating communication.
 */
class configComponent
{
  protected:
	configManagerInterface* configManagerInterface_; ///< Pointer to the mediator interface.

  public:
	/**
	 * @brief Construct a new configComponent object.
	 *
	 * Initializes the configuration component and optionally sets a mediator interface.
	 *
	 * @param cnfInterface Pointer to the mediator interface (default is nullptr).
	 */
	configComponent(configManagerInterface* cnfInterface = nullptr) : configManagerInterface_(cnfInterface) {}

	/**
	 * @brief Set the mediator for the configuration component.
	 *
	 * Updates the mediator interface for facilitating communication.
	 *
	 * @param cnfInterface Pointer to the mediator interface.
	 */
	void setMediator(configManagerInterface* cnfInterface)
	{
		this->configManagerInterface_ = cnfInterface;
	}
};
