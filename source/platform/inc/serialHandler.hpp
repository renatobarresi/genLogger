#pragma once

#include "terminal_component.hpp"
#include <cstddef>

bool serialHandlerInit(void);

bool serialHandler(void);

char* getSerialBuffer(char* buffer, size_t bufferSize);

void clearSerialBuffer();

terminalSignal getTerminalSignal();