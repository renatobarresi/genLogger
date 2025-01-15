/**
 * @file terminal_component.hpp
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief
 * @version 0.1
 * @date 2025-01-13
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

enum class terminalState
{
	basicDeviceInfo,
	basicDeviceConfig,
	sensorConfig,
	networkConfig,

	numStates
};

enum class terminalSignals
{
	ENTRY,
	EXIT,
	pressedKey_C,
	pressedKey_B,
	pressedKey_S,
	pressedKey_N,

	numSignals
};

enum class terminalEvents
{
	EVENT_HANDLED,
	EVENT_IGNORED,
	EVENT_TRANSITION
};

class terminalStateMachine
{
  public:
	terminalStateMachine() {}

	void init(terminalState state);
	void handler();

  private:
	terminalState activeState;
	terminalEvents eventDispacher(terminalState state);
};
