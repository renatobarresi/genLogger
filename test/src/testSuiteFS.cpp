#include "config_manager.hpp"
#include "filesystemWrapper.hpp"
#include "internalStorage_component.hpp"
#include "terminal_component.hpp"
#include "virtualRTC.hpp"
#include <fstream>
#include <gtest/gtest.h>
#include <string>

char testFileLocation[] = "testFS.txt";

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

TEST(terminalStateMachine, testChangeToDeviceInfoState)
{
	virtualRTC				 rtc;
	terminalStateMachine	 terminalOutput(&rtc);
	internalStorageComponent storage;
	configManager			 loggerConfig(&terminalOutput, &storage);

	// Init terminal
	terminalOutput.init(terminalState::initState);
	terminalOutput.handler(terminalSignal::ENTRY);

	// Simulate "I" key press
	terminalOutput.handler(terminalSignal::pressedKey_I);

	EXPECT_EQ(terminalOutput.activeState, terminalState::basicDeviceInfo) << "Failed to set basicDeviceInfo";

	// Simulate "B" key press
	terminalOutput.handler(terminalSignal::pressedKey_B);

	EXPECT_EQ(terminalOutput.activeState, terminalState::initState) << "Failed to set initState";

	// Simulate "C" key press
	terminalOutput.handler(terminalSignal::pressedKey_C);

	EXPECT_EQ(terminalOutput.activeState, terminalState::basicDeviceConfig) << "Failed to set basicDeviceConfig";

	// Simulate "B" key press
	terminalOutput.handler(terminalSignal::pressedKey_B);

	EXPECT_EQ(terminalOutput.activeState, terminalState::initState) << "Failed to set initState";
}