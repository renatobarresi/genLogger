#pragma once

#ifndef TARGET_MICRO
#include <atomic>
#include <chrono>
#include <cstdint>
#include <functional>
#include <thread>
#include <utility> // for std::declval
#endif
namespace systick
{

#ifndef TARGET_MICRO

class Timer1msSimulator
{
  public:
	using Callback = std::function<void(void)>;

	Timer1msSimulator() : _running(false) {}

	void start(const Callback& cb)
	{
		if (_running.load())
			return; // Prevent multiple starts
		_running.store(true);
		_timerThread = std::thread([this, cb]() {
			while (_running.load())
			{
				auto startTime = std::chrono::steady_clock::now();
				cb(); // Call the callback every 1ms
				std::this_thread::sleep_until(startTime + std::chrono::milliseconds(1));
			}
		});
	}

	void stop()
	{
		_running.store(false);
		if (_timerThread.joinable())
			_timerThread.join();
	}

	~Timer1msSimulator()
	{
		stop();
	}

  private:
	std::thread		  _timerThread;
	std::atomic<bool> _running;
};

// Internal static instance for simulation
static Timer1msSimulator _simulator;

// Externally callable function to start simulation
inline void startSystickSimulation(const Timer1msSimulator::Callback& cb)
{
	_simulator.start(cb);
}

// Optional stop function
inline void stopSystickSimulation()
{
	_simulator.stop();
}

#else

#endif
} // namespace systick