#pragma once

#include <future>           // std::promise, std::future (sync test/server)

void httpServer(std::promise<void>& ready);