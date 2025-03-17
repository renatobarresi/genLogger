#pragma once

#include <cstddef>

bool serialHandlerInit(void);

bool serialHandler(void);

void processSerialBuffer();

void getSerialBuffer(char* buffer, size_t bufferSize);
