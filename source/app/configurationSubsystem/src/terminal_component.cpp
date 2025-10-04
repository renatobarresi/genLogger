/**
 * @file terminal_component.cpp
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-01-13
 *
 * @copyright Copyright (c) 2025
 *
 * TODO: Refactor to use state pattern
 */

////////////////////////////////////////////////////////////////////////
//							    Includes
////////////////////////////////////////////////////////////////////////

#include "terminal_component.hpp"
#include "device_version.hpp"
#include "loggerMetadata.hpp"
#include "utilities.hpp"
#include "virtualTimer.hpp"
#include <charconv>
#include <cstdio>
#include <cstring>
#include <span>
#include <system_error>

////////////////////////////////////////////////////////////////////////
//				      Private function prototypes
////////////////////////////////////////////////////////////////////////

static void printHelp();
static void printConfigHelp();

////////////////////////////////////////////////////////////////////////
//						   Stream redirection
////////////////////////////////////////////////////////////////////////

#ifdef TARGET_MICRO
#include "uart.h"
// #include <streambuf>
// class UartStreamBuf : public std::streambuf
// {
//   protected:
// 	int overflow(int c) override
// 	{
// 		if (c != EOF)
// 		{
// 			uart_tx(static_cast<char>(c), 1);
// 		}
// 		return c;
// 	}
// };

// UartStreamBuf uartStreamBuf;
// std::ostream  uartOut(&uartStreamBuf);
#endif

////////////////////////////////////////////////////////////////////////
//					   Public methods implementation
////////////////////////////////////////////////////////////////////////

void terminalStateMachine::init(terminalState state)
{
#ifdef TARGET_MICRO
	// TODO Redirect std::cout to UART

#endif

	this->activeState	  = state;
	this->availableSignal = terminalSignal::ENTRY;
	this->_loggerMetadata = getLoggerMetadata();

	printBanner();
}

/**
 * @brief the handler checks for pending signals and sends
 * them to the signal dispacher.
 * 
 */
void terminalStateMachine::handler(terminalSignal sig, const char* buff)
{
	terminalState currentState = this->activeState;
	terminalState nextState;
	terminalEvent event = terminalEvent::EVENT_IGNORED;

	event = signalDispacher(currentState, sig, buff);

	if (terminalEvent::EVENT_TRANSITION == event)
	{
		// Exit current state
		signalDispacher(currentState, terminalSignal::EXIT, nullptr);

		// Enter next state
		nextState = this->activeState;
		signalDispacher(nextState, terminalSignal::ENTRY, nullptr);
	}
}

void terminalStateMachine::setSignal(terminalSignal sig)
{
	this->availableSignal = sig;
}

terminalSignal terminalStateMachine::getSignal()
{
	return this->availableSignal;
}

////////////////////////////////////////////////////////////////////////
//					 Private methods implementation
////////////////////////////////////////////////////////////////////////

terminalEvent terminalStateMachine::signalDispacher(terminalState state, terminalSignal sig, const char* buff)
{
	terminalEvent event = terminalEvent::EVENT_IGNORED;

	switch (state)
	{
		case terminalState::initState:
		{
			switch (sig)
			{
				case terminalSignal::ENTRY:
				{
					printHelp();
					event = terminalEvent::EVENT_HANDLED;
				}
				break;
				case terminalSignal::EXIT:
				{
					// Clean Terminal
					printf("\033[2J\033[H"); // ANSI escape sequence to clear the screen and reset cursor to top-left
					event = terminalEvent::EVENT_HANDLED;
				}
				break;
				case terminalSignal::pressedKey_I:
				{
					this->activeState = terminalState::basicDeviceInfo;
					event			  = terminalEvent::EVENT_TRANSITION;
				}
				break;
				case terminalSignal::pressedKey_C:
				{
					this->activeState = terminalState::basicDeviceConfig;
					event			  = terminalEvent::EVENT_TRANSITION;
				}
				break;
				case terminalSignal::pressedKey_S:
				{
					this->activeState = terminalState::streamADC;
					event			  = terminalEvent::EVENT_TRANSITION;
				}
				break;
				default:
				{
					event = terminalEvent::EVENT_IGNORED;
				}
				break;
			}
		}
		break;
		case terminalState::streamADC:
		{
			switch (sig)
			{
				case terminalSignal::ENTRY:
				{
					printf("Streaming ADC data, press B to go back\r\n");
					this->availableSignal = terminalSignal::streamData;
					event				  = terminalEvent::EVENT_HANDLED;
				}
				break;
				case terminalSignal::streamData:
				{
					if (false == this->_flagStreamDelay)
					{
						this->_ticks		   = systick::getTicks();
						this->_flagStreamDelay = true;
					}

					if (systick::getTicks() - _ticks > 1000)
					{
						//count++;
						float voltage = _sensorService.requestADCVoltage();
						printf("ADC Voltage: %f\r\n", voltage);
						printf("\033[A\033[2K");
						this->_flagStreamDelay = false;
					}
					//printf("\033[2J\033[H"); // ANSI escape sequence to clear the screen and reset cursor to top-left
					event = terminalEvent::EVENT_HANDLED;
				}
				break;
				case terminalSignal::EXIT:
				{
					// Clean Terminal
					printf("\033[2J\033[H"); // ANSI escape sequence to clear the screen and reset cursor to top-left
					event = terminalEvent::EVENT_HANDLED;
				}
				break;
				case terminalSignal::pressedKey_B:
				{
					this->activeState = terminalState::initState;
					event			  = terminalEvent::EVENT_TRANSITION;
				}
				break;
				default:
				{
					event = terminalEvent::EVENT_IGNORED;
				}
				break;
			}
		}
		break;
		case terminalState::basicDeviceInfo:
		{
			switch (sig)
			{
				case terminalSignal::ENTRY:
				{
					updateLoggerMetadata();

					printLoggerMetadata();

					event = terminalEvent::EVENT_HANDLED;
				}
				break;
				case terminalSignal::EXIT:
				{
					// Clean Terminal
					printf("\033[2J\033[H"); // ANSI escape sequence to clear the screen and reset cursor to top-left
					event = terminalEvent::EVENT_HANDLED;
				}
				break;
				case terminalSignal::pressedKey_B:
				{
					this->activeState = terminalState::initState;
					event			  = terminalEvent::EVENT_TRANSITION;
				}
				break;
				default:
				{
					event = terminalEvent::EVENT_IGNORED;
				}
				break;
			}
		}
		break;
		case terminalState::basicDeviceConfig:
		{
			switch (sig)
			{
				case terminalSignal::ENTRY:
				{
					printConfigHelp();
					event = terminalEvent::EVENT_HANDLED;
				}
				break;
				case terminalSignal::EXIT:
				{
					printf("\033[2J\033[H"); // ANSI escape sequence to clear the screen and reset cursor to top-left
					event = terminalEvent::EVENT_HANDLED;
				}
				break;
				case terminalSignal::pressedKey_B:
				{
					this->activeState = terminalState::initState;
					event			  = terminalEvent::EVENT_TRANSITION;
				}
				break;
				case terminalSignal::pressedKey_T:
				{
					// Set device time and date
					if (nullptr == buff)
					{
						printf("Please input the time following this format HH:MM:SS DD/MM/YYYY\r\n");
						this->_previousSignal = terminalSignal::pressedKey_T;
					}

					event = terminalEvent::EVENT_HANDLED;
				}
				break;
				case terminalSignal::pressedKey_C:
				{
					printf("Please input how often to send the file in minutes\r\n");
					this->_previousSignal = terminalSignal::pressedKey_C;

					event = terminalEvent::EVENT_HANDLED;
				}
				break;
				case terminalSignal::pressedKey_F:
				{
					printf("Please input the File creation period\r\n");
					printf("Type 1 for file creation a day\r\nType 2 for file creation only once\r\n");
					this->_previousSignal = terminalSignal::pressedKey_F;

					event = terminalEvent::EVENT_HANDLED;
				}
				break;
				case terminalSignal::pressedKey_M:
				{
					printf("Please input the measurement period in minutes\r\n");
					this->_previousSignal = terminalSignal::pressedKey_M;

					event = terminalEvent::EVENT_HANDLED;
				}
				break;
				case terminalSignal::pressedKey_Enter:
				{
					if (nullptr == buff)
					{
						printf("Invalid data, please type again\r\n");
						break;
					}

					switch (this->_previousSignal)
					{
						case terminalSignal::pressedKey_N:
						{
							// Update _configurationBuffer with the device name
							std::strncpy(_loggerMetadata->loggerName, buff, sizeof(_loggerMetadata->loggerName));
							printf("Name copied, input S to save it\r\n");
						}
						break;
						case terminalSignal::pressedKey_T:
						{
							bool validData = false;
							int	 hour;
							int	 minute;
							int	 seconds;
							int	 day;
							int	 month;
							int	 year;

							validData = utilities::parseTimeAndDate(buff, &hour, &minute, &seconds, &day, &month, &year);

							if (false == validData)
							{
								printf("Invalid data, please type again\r\n");
								break;
							}

							if (true == this->_terminalRTC.setTime(static_cast<uint8_t>(hour), static_cast<uint8_t>(minute), static_cast<uint8_t>(seconds)) && true == this->_terminalRTC.setDate(static_cast<uint8_t>(day), static_cast<uint8_t>(month), static_cast<uint16_t>(year)))
							{
								printf("RTC configured\r\n");
							}
							else
							{
								printf("Error configuring RTC\r\n");
							}
						}
						break;
						case terminalSignal::pressedKey_F:
						{
							if ('1' != buff[0] && '2' != buff[0])
							{
								printf("Invalid data\r\n");
								break;
							}

							_loggerMetadata->fileCreationPeriod = buff[0];
							printf("File creation period changed, input S to save it\r\n");
						}
						break;
						case terminalSignal::pressedKey_M:
							//clang-format off
							[[fallthrough]];
						// clang-format on
						case terminalSignal::pressedKey_C:
						{
							// Validate input
							int value;
							auto [ptr, ec] = std::from_chars(buff, buff + sizeof(buff) - 1, value);

							if (ec != std::errc())
							{
								printf("Invalid data, please type again\r\n");
							}
							else
							{
								if (terminalSignal::pressedKey_C == this->_previousSignal)
								{
									_loggerMetadata->fileTransmissionPeriod = static_cast<uint16_t>(value);
									printf("File transmission period changed, input S to save it\r\n");
								}
								else if (terminalSignal::pressedKey_M == this->_previousSignal)
								{
									_loggerMetadata->generalMeasurementPeriod = static_cast<uint16_t>(value);
									printf("Measurement period changed, input S to save it\r\n");
								}
							}

							event = terminalEvent::EVENT_HANDLED;
						}
						break;
						default:
						{
							event = terminalEvent::EVENT_IGNORED;
						}
						break;
					}

					event = terminalEvent::EVENT_HANDLED;
				}
				break;
				case terminalSignal::pressedKey_N:
				{
					// Set device name
					if (nullptr == buff)
					{
						printf("Please input the device name\r\n");
						this->_previousSignal = terminalSignal::pressedKey_N;
					}

					event = terminalEvent::EVENT_HANDLED;
				}
				break;
				case terminalSignal::pressedKey_S:
				{
					printf("Storing configuratoin in memory..\r\n");
					std::array<char, sizeof(loggerMetadata)> buffMetadata{};

					// clang-format off
					snprintf(buffMetadata.data(),
							 buffMetadata.size(),
							 "%s;%d;%d;%d;%d\r\n",
							 _loggerMetadata->loggerName,
							 _loggerMetadata->fileCreationPeriod,
							 _loggerMetadata->fileTransmissionPeriod,
							 _loggerMetadata->generalMeasurementPeriod,
							 _loggerMetadata->restRequestPeriod);
					// clang-format on

					std::span<char> buffMetadataSpan(buffMetadata.data(), buffMetadata.size());

					// Signal mediator to comunicate with internal storage component
					uint8_t res = this->configManagerInterface_->notify(this, mediatorEvents::STORE_METADATA, buffMetadataSpan);

					if (res != 1)
					{
						printf("Error trying to store in memory\r\n");
					}
					else
					{
						printf("Configuration saved!\r\n");
					}

					event = terminalEvent::EVENT_HANDLED;
				}
				break;
				default:
				{
					event = terminalEvent::EVENT_IGNORED;
				}
				break;
			}
		}
		break;
		default:
		{
			// do nothing
		}
		break;
	}

	return event;
}

bool terminalStateMachine::updateLoggerMetadata()
{
	std::span<const char> spanBuff("123");
	uint8_t				  res = this->configManagerInterface_->notify(this, mediatorEvents::UPDATE_METADATA, spanBuff);

	if (res != 1)
	{
		return false;
	}

	return true;
}

void terminalStateMachine::printBanner()
{
	printf("#############################\r\n");
	printf("genLogger version: %c.%c.%c.%s\r\n", MAJOR, MINOR, PATCH, DEVELOPMENT);
	printf("#############################\r\n");
}

void terminalStateMachine::printLoggerMetadata()
{
	char timBuff[9];
	char dateBuff[15];

	_terminalRTC.getTime(&timBuff[0], sizeof(timBuff));
	_terminalRTC.getDate(&dateBuff[0], sizeof(dateBuff));

	printf("#############################\r\n");
	printf("Device name: %s\r\n", _loggerMetadata->loggerName);
	printf("Device time: %s\r\n", timBuff);
	printf("Device date: %s\r\n", dateBuff);
	printf("File creation period: %c\r\n", _loggerMetadata->fileCreationPeriod);
	printf("File transmission period: %u\r\n", _loggerMetadata->fileTransmissionPeriod);
	printf("Measurement period: %u\r\n", _loggerMetadata->generalMeasurementPeriod);
	printf("HTTP POST period: %u\r\n", _loggerMetadata->restRequestPeriod);
	printf("Firmware version: %c.%c.%c.%s\r\n", MAJOR, MINOR, PATCH, DEVELOPMENT);
	printf("B - return\r\n");
	printf("#############################\r\n");
}

////////////////////////////////////////////////////////////////////////
//				      Private function implementation
////////////////////////////////////////////////////////////////////////

static void printHelp()
{
	printf("#############################\r\n");
	printf("Help Menu:\r\n");
	printf("#############################\r\n");
	printf("I - Print Device Info\r\n");
	printf("C - Configure Device\r\n");
	printf("S - Stream RAW ADC data\r\n");
	printf("#############################\r\n");
}

static void printConfigHelp()
{
	printf("#############################\r\n");
	printf("Configure menu\r\n");
	printf("#############################\r\n");
	printf("T - Set device time and date\r\n");
	printf("N - set device name\r\n");
	printf("F - set the device creation period\r\n");
	printf("C - set the device file transmission period\r\n");
	printf("M - set the device measurement period\r\n");
	printf("S - Store configuration in memory\r\n");
	printf("B - return\r\n");
	printf("#############################\r\n");
}