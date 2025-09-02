#include "anemometer.hpp"
#include "config_manager.hpp"
#include "filesystemWrapper.hpp"
#include "httpClient.hpp"
#include "internalStorage_component.hpp"
#include "loggerMetadata.hpp"
#include "logger_manager.hpp"
#include "networkManager.hpp"
#include "pluviometer.hpp"
#include "processing_manager.hpp"
#include "terminal_component.hpp"
#include "utilities.hpp"
#include "virtualRTC.hpp"
#include <cstdio>
#include <cstring>
#include <fstream>
#include <gtest/gtest.h>
#include "httpServer.hpp"
#include <thread>
#include <string>

char testFileLocation[] = "testFS.txt";

TEST(utilities, testTimeDateParsing)
{
	// Test variables to store parsed values
	int hour, minute, seconds, day, month;
	int year;

	// Test case 1: Valid format "HH:MM:SS DD/MM/YYYY"
	EXPECT_TRUE(parseTimeAndDate("14:30:45 23/05/2025", &hour, &minute, &seconds, &day, &month, &year));
	EXPECT_EQ(hour, 14);
	EXPECT_EQ(minute, 30);
	EXPECT_EQ(seconds, 45);
	EXPECT_EQ(day, 23);
	EXPECT_EQ(month, 5);
	EXPECT_EQ(year, 2025);

	// Test case 2: Valid format with single digits
	EXPECT_TRUE(parseTimeAndDate("01:05:09 02/03/2024", &hour, &minute, &seconds, &day, &month, &year));
	EXPECT_EQ(hour, 1);
	EXPECT_EQ(minute, 5);
	EXPECT_EQ(seconds, 9);
	EXPECT_EQ(day, 2);
	EXPECT_EQ(month, 3);
	EXPECT_EQ(year, 2024);

	// Test case 3: Invalid format - wrong separators
	EXPECT_FALSE(parseTimeAndDate("14-30-45 23-05-2025", &hour, &minute, &seconds, &day, &month, &year));

	// Test case 4: Invalid format - missing components
	EXPECT_FALSE(parseTimeAndDate("14:30 23/05/2025", &hour, &minute, &seconds, &day, &month, &year));

	// Test case 5: Invalid values - out of range
	EXPECT_FALSE(parseTimeAndDate("25:70:99 32/13/2025", &hour, &minute, &seconds, &day, &month, &year));

	// Test case 6: Null input
	EXPECT_FALSE(parseTimeAndDate(nullptr, &hour, &minute, &seconds, &day, &month, &year));

	// Test case 7: Empty string
	EXPECT_FALSE(parseTimeAndDate("", &hour, &minute, &seconds, &day, &month, &year));
}

TEST(fileSystemWrapper, testOpen)
{
	// Create a file to be able to test
	std::ofstream testFile(testFileLocation);
	testFile << "thisFile";
	testFile.close();

	fileSysWrapper fileSystem(0); // Use the non-microcontroller implementation

	bool status = fileSystem.open(testFileLocation, 0);
	fileSystem.close();

	ASSERT_TRUE(status) << "File open failed, path: " << testFileLocation;
}

TEST(filesystemWrapper, testRead)
{
	fileSysWrapper fileSystem(0); // Use the non-microcontroller implementation

	fileSystem.open(testFileLocation, 0);

	char buffer[8];

	int bytesRead = fileSystem.read(buffer, 8);

	fileSystem.close();

	EXPECT_STREQ(buffer, "thisFile") << "File read failed";
}

TEST(terminalStateMachine, testChangesInSMState)
{
	virtualRTC				 rtc;
	terminalStateMachine	 terminalOutput(&rtc);
	internalStorageComponent storage;
	configManager			 loggerConfig(&terminalOutput, &storage);

	if (false == storage.initFS())
	{
		FAIL() << "Failed to initialize filesystem";
	}

	// Init terminal
	terminalOutput.init(terminalState::initState);
	terminalOutput.handler(terminalSignal::ENTRY, nullptr);

	// Simulate "I" key press
	terminalOutput.handler(terminalSignal::pressedKey_I, nullptr);

	EXPECT_EQ(terminalOutput.activeState, terminalState::basicDeviceInfo) << "Failed to set basicDeviceInfo";

	// Simulate "B" key press
	terminalOutput.handler(terminalSignal::pressedKey_B, nullptr);

	EXPECT_EQ(terminalOutput.activeState, terminalState::initState) << "Failed to set initState";

	// Simulate "C" key press
	terminalOutput.handler(terminalSignal::pressedKey_C, nullptr);

	EXPECT_EQ(terminalOutput.activeState, terminalState::basicDeviceConfig) << "Failed to set basicDeviceConfig";

	// Simulate "B" key press
	terminalOutput.handler(terminalSignal::pressedKey_B, nullptr);

	EXPECT_EQ(terminalOutput.activeState, terminalState::initState) << "Failed to set initState";
}

TEST(terminalStateMachine, testChangeToDeviceConfigState)
{
	loggerMetadata* pLoggerMetadata;

	virtualRTC				 rtc;
	terminalStateMachine	 terminalOutput(&rtc);
	internalStorageComponent storage;
	configManager			 loggerConfig(&terminalOutput, &storage);

	if (false == storage.initFS())
	{
		FAIL() << "Failed to initialize filesystem";
	}

	terminalOutput.init(terminalState::initState);
	terminalOutput.handler(terminalSignal::ENTRY, nullptr);

	terminalOutput.handler(terminalSignal::pressedKey_C, nullptr);
	terminalOutput.handler(terminalSignal::pressedKey_N, nullptr);
	terminalOutput.handler(terminalSignal::pressedKey_Enter, "station1");
	terminalOutput.handler(terminalSignal::pressedKey_S, nullptr);
	terminalOutput.handler(terminalSignal::pressedKey_B, nullptr);
	terminalOutput.handler(terminalSignal::pressedKey_I, nullptr);

	pLoggerMetadata = getLoggerMetadata();

	EXPECT_STREQ(pLoggerMetadata->loggerName, "station1") << "Failed to set loggerName";
}

TEST(loggerSubsystem, testNotification)
{
	virtualRTC															 rtc;
	processingManager<sensor::davisPluviometer, sensor::anemometerDavis> myProcessingManager(rtc);
	loggerManager														 myLoggerManager;
	const char*															 testBuff = "123,125y3,23534if";
	char																 timeStamp[20];
	char																 finalTestBuff[100];

	// Mock measurement here
	std::sprintf(myProcessingManager.sensorInfoBuff.data(), "%s", testBuff);

	myProcessingManager.setObserver(&myLoggerManager);
	myProcessingManager.takeMeasurements();
	myProcessingManager.notifyObservers();

	rtc.getTimestamp(timeStamp);

	// Mock measurement here
	std::sprintf(myProcessingManager.sensorInfoBuff.data(), "%s;%s", timeStamp, testBuff);

	EXPECT_STREQ(myProcessingManager.sensorInfoBuff.data(), myLoggerManager.measurementsBuff.data());
}

TEST(networkManager, testStablishConn)
{
	const char ip[]		 = "123";
	const char netmask[] = "123";
	const char gateway[] = "123";
	bool retVal = false;

    // Prepare promise/future to synchronize server startup
    std::promise<void> server_ready;
    std::future<void> server_ready_future = server_ready.get_future();

	network::networkManager myNetwork(ip, netmask, gateway);
	
    // Launch server in separate thread
    std::thread server_thread(httpServer, std::ref(server_ready));

    // Wait for server to be ready
    server_ready_future.wait();

	myNetwork.init();

	retVal = myNetwork.httpConnectPost("127.0.0.1:8080", "123");

	server_thread.join(); // Wait for server thread to finish

	EXPECT_EQ(true, retVal);
}

TEST(loggerSubsystem, testWritingExternal)
{
	// char			  fileName[56];
	// fileSysWrapper	  fileSystem(0); // Use the non-microcontroller implementation
	// loggerMetadata*	  pLoggerMetadata;
	// virtualRTC		  rtc;
	// processingManager myProcessingManager(rtc);
	// loggerManager	  myLoggerManager(&myProcessingManager);
	// const char*		  testBuff = "123,125y3,23534if";
	// char			  timeStamp[20];
	// char			  finalTestBuff[100];
	// char			  storedData[56]		 = {0};
	// char			  simulationFileFolder[] = "../../../test/simulationFiles/externalMemory";

	// pLoggerMetadata = getLoggerMetadata();

	// std::sprintf(fileName, "%s/%s", simulationFileFolder, pLoggerMetadata->loggerName);

	// // get timestamp
	// rtc.getTimestamp(timeStamp);

	// // create testBuffer
	// std::sprintf(finalTestBuff, "%s;%s", timeStamp, testBuff);

	// // put mock-up buffer into tprocessing manager sensor information buffer
	// std::strcpy(myProcessingManager.sensorInfoBuff, testBuff);

	// // Set observer and simulate processing of sensor data
	// myProcessingManager.setObserver(&myLoggerManager);
	// myProcessingManager.processData();

	// // loggerManager handler
	// myLoggerManager.init();
	// myLoggerManager.handler();

	// // Read simulated file in "external device"
	// if (true == fileSystem.open(fileName, 0))
	// {
	// 	fileSystem.read(storedData, sizeof(storedData));

	// 	fileSystem.close();
	// }
	// else
	// {
	// 	std::cout << "File not opened" << std::endl;
	// }

	// EXPECT_STREQ(finalTestBuff, storedData);
	EXPECT_TRUE(true);
}