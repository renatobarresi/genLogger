/**
 * @file terminal_component.hpp
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief Header file for the terminal component, implementing a state machine 
 * for managing terminal states and events.
 * @version 0.1
 * @date 2025-01-13
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
#include "virtualRTC.hpp"

////////////////////////////////////////////////////////////////////////
//							     Types
////////////////////////////////////////////////////////////////////////

/**
 * @brief Enumeration representing the various states of the terminal state machine.
 *
 */
enum class terminalState
{
	initState = 1,
	basicDeviceInfo,   ///< State for displaying basic device information.
	basicDeviceConfig, ///< State for configuring basic device settings.
	sensorConfig,	   ///< State for configuring sensors.
	networkConfig,	   ///< State for configuring network settings.

	numStates ///< Number of states (used for validation or iteration).
};

/**
 * @brief Enumeration representing the signals or events that can trigger state transitions in the
 * terminal state machine.
 *
 */
enum class terminalSignal
{
	ENTRY, ///< Signal indicating entry into a state.
	EXIT,  ///< Signal indicating exit from a state.
	IDLE,
	pressedKey_I, ///< Signal triggered when the 'I' key is pressed.
	pressedKey_C, ///< Signal triggered when the 'C' key is pressed.
	pressedKey_B, ///< Signal triggered when the 'B' key is pressed.
	pressedKey_N, ///< Signal triggered when the 'N' key is pressed.
	pressedKey_T,
	pressedKey_S,
	pressedKey_Enter,

	numSignals ///< Number of signals (used for validation or iteration).
};

/**
 * @brief Enumeration representing the outcomes of event handling within the terminal state machine.
 *
 */
enum class terminalEvent
{
	EVENT_HANDLED,	 ///< Event was successfully handled within the current state.
	EVENT_IGNORED,	 ///< Event was ignored in the current state.
	EVENT_TRANSITION ///< Event triggered a state transition.
};

////////////////////////////////////////////////////////////////////////
//							Class definition
////////////////////////////////////////////////////////////////////////

/**
 * @brief Class implementing a state machine for managing terminal states and events.
 *
 * The terminalStateMachine class inherits from configComponent and provides functionality
 * to initialize, handle, and dispatch events across various terminal states.
 */
class terminalStateMachine : public configComponent
{
  public:
	terminalState activeState; ///< The currently active state of the terminal state machine.

	/**
	 * @brief Construct a new terminalStateMachine object.
	 *
	 * Initializes the terminal state machine object. The initial state must be set separately
	 * using the init() method.
	 */
	terminalStateMachine(virtualRTC* rtc) : _terminalRTC(rtc) {}

	/**
	 * @brief Initialize the terminal state machine with a specified initial state.
	 *
	 * @param state The initial state to set for the state machine.
	 */
	void init(terminalState state);

	/**
	 * @brief Main handler for processing events and executing state transitions.
	 *
	 * This method should be called periodically to evaluate and process terminal events.
	 */
	void handler(terminalSignal sig, const char* buff);

	/**
	 * @brief Set the Signal object
	 * 
	 * @param sig 
	 */
	void setSignal(terminalSignal sig);

  private:
	virtualRTC* _terminalRTC;
	//char				  _timeBuff[9];
	struct loggerMetadata _loggerMetadata;
	void*				  paramToConfig;
	terminalSignal		  _previousSignal;

	terminalSignal availableSignal;
	/**
	 * @brief Dispatch a signal to the appropriate handler based on the current state.
	 *
	 * @param state The current state of the terminal state machine.
	 * @return terminalEvents The result of the event handling, indicating whether the event
	 * was handled, ignored, or triggered a state transition.
	 */
	terminalEvent signalDispacher(terminalState state, terminalSignal sig, const char* buff);

	/**
	 * @brief Signals to mediator that an update to the logger's metadata is needed
	 *
	 */
	bool updateLoggerMetadata();

	/**
	 * @brief 
	 * 
	 */
	void printLoggerMetadata();

	/**
	 * @brief 
	 * 
	 */
	void printBanner();
};
