#pragma once

#include <cstdint>

#ifndef TARGET_MICRO
#include <atomic>
#include <chrono>
#include <functional>
#include <thread>
#include <utility> // for std::declval
#endif

#ifndef TARGET_MICRO
/**
 * @brief Allows bussiness logic to define 
 */
struct hardwareTimeouts
{
	const uint32_t*	  taskTimeout;
	volatile uint8_t* taskRunFlag;
};
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
		{
			return; // Prevent multiple starts
		}

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
		{
			_timerThread.join();
		}
	}

	~Timer1msSimulator()
	{
		stop();
	}

  private:
	std::thread		  _timerThread;
	std::atomic<bool> _running;
};

/**
 * @brief Simulates a microcontroller SysTick Handler for non-target builds.
 * @details This function is called periodically by a simulated systick timer. It increments a
 * tick counter and sets the `runMeasurementTask` flag when the measurement period elapses.
 */
void myTickHandler();

// Externally callable function to start simulation
void startSystickSimulation(struct hardwareTimeouts** taskControlParams);
// Optional stop function
void stopSystickSimulation();
void myTickHandler();
#endif

uint64_t getTicks();

} // namespace systick