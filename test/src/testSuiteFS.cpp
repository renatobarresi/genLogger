#include "filesystemWrapper.hpp"
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