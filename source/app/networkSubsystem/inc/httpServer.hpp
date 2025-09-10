#pragma once

#include <future> // std::promise, std::future (sync test/server)

void httpServer(int& ready);