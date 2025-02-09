#include "filesystemWrapper.hpp"
#include <gtest/gtest.h>
#include <string>

char testFileLocation[] = "/home/renato/renato/CESE_fiuba/proyecto_final/genLogger/firmware/test/"
						  "simulationFiles/testFS.txt";
TEST(fileSystemWrapper, testOpen)
{
	//
	fileSysWrapper fileSystem(0); // Use the non-microcontroller implementation

	bool status = fileSystem.open(testFileLocation, 0);
	fileSystem.close();

	ASSERT_TRUE(status) << "File open failed";
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