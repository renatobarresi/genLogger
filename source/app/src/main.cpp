/**
 * @file main.cpp
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief genLogger main application source file
 *
 * genLogger (generic datalogger) is a datalogger
 *
 * The application has five main subsystems:
 *	1) Configuration subsystem
 *	2) Sensor management subsystem
 *  3) Processing subsystem
 *	4) Storage susbsystem
 *	5) Network subsystem
 *
 * The macro TARGET_MICRO must be passed as a compiler definition if
 * the build type is for the target microcontroller. When its defined,
 * components related to the target board are included.
 *
 * @version 0.1
 * @date 2025-01-12
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "configuration_subsystem.hpp"

// Includes related to the used board
#ifdef TARGET_MICRO
#include "init.h"
#endif

configManager stationConfigManager;

int main()
{
#ifdef TARGET_MICRO
	stm32f429_init();
#endif

	stationConfigManager.init();

	while(1)
	{
	}

	return 0;
}