/**
 * @file filesystem_wrapper.hpp
 * @author Renato Barresi (renatobarresi@gmail.com)
 * @brief
 * @version 0.1
 * @date 2025-02-01
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

////////////////////////////////////////////////////////////////////////
//								Includes
////////////////////////////////////////////////////////////////////////

#include <cstdint>
#include <cstdio>
#include <cstring>

#ifdef TARGET_MICRO
#include "littleFSInterface.h"
#endif

////////////////////////////////////////////////////////////////////////
//							Class definition
////////////////////////////////////////////////////////////////////////

/**
 * @brief Interface for file handlers, allowing different storage implementations.
 */
struct FileHandler
{
	virtual bool mount()								  = 0;
	virtual bool open(const char* fileName, uint8_t mode) = 0;
	virtual int	 read(char* buffer, size_t size)		  = 0;
	virtual int	 write(const char* buffer, size_t size)	  = 0;
	virtual int	 close()								  = 0;
	virtual ~FileHandler()								  = default;
};

/**
 * @brief Standard file handler using C standard library file operations.
 */
class CFileHandler : public FileHandler
{
  private:
	FILE* file = nullptr; /// File pointer for standard file operations.

  public:
	/**
	 * @brief Opens a file with the given mode.
	 * @param[in] fileName Name of the file to open.
	 * @param mode Access mode: 0 = read, 1 = write, 2 = append.
	 * @return true if the file was successfully opened, false otherwise.
	 */
	bool open(const char* fileName, uint8_t mode) override
	{
		const char* cMode = nullptr;
		switch (mode)
		{
			case 0:
				cMode = "r";
				break; // Read mode
			case 1:
				cMode = "w";
				break; // Write mode
			case 2:
				cMode = "a";
				break; // Append mode
			default:
				return false;
		}
		file = fopen(fileName, cMode);

		if (file == nullptr)
		{
			return false;
		}

		return true;
	}

	/**
	 * @brief Mounts the filesystem (always returns true for standard filesystems).
	 * @return true
	 */
	bool mount() override
	{
		return true;
	}

	/**
	 * @brief Reads data from the file into a buffer.
	 * @param buffer Buffer to store the read data.
	 * @param size Number of bytes to read.
	 * @return Number of bytes successfully read.
	 */
	int read(char* buffer, size_t size) override
	{
		if (!file)
			return 0;
		return fread(buffer, 1, size, file);
	}

	/**
	 * @brief Writes data from a buffer to the file.
	 * @param buffer Data buffer to write.
	 * @param size Number of bytes to write.
	 * @return Number of bytes successfully written.
	 */
	int write(const char* buffer, size_t size) override
	{
		if (!file)
			return 0;
		return fwrite(buffer, 1, size, file);
	}

	/**
	 * @brief Closes the currently opened file.
	 * @return 0 on success.
	 */
	int close() override
	{
		if (file)
			fclose(file);
		file = nullptr;

		return 0;
	}
};

#ifdef TARGET_MICRO
/**
 * @brief File handler implementation for LittleFS on embedded devices.
 */
class LittleFSHandler : public FileHandler
{
  private:
	lfs_t	   lfs;	 ///< LittleFS instance.
	lfs_file_t file; ///< LittleFS file handle.

  public:
	/**
	 * @brief Mounts the LittleFS filesystem.
	 * @return true if successfully mounted or formatted, false otherwise.
	 */
	bool mount() override
	{
		flash_init();

		int res = lfs_mount(&lfs, &cfg);
		if (res < 0)
		{
			// If the mount fails, try formatting the filesystem
			res = lfs_format(&lfs, &cfg);
			if (res < 0)
			{
				return false;
			}
			// Try mounting again
			res = lfs_mount(&lfs, &cfg);
			if (res < 0)
			{
				return false;
			}
		}

		return true;
	}

	/**
	 * @brief Opens a file on the LittleFS filesystem.
	 * @param fileName Name of the file to open.
	 * @param mode Access mode: 0 = read, 1 = write, 2 = append.
	 * @return true if the file was successfully opened, false otherwise.
	 */
	bool open(const char* fileName, uint8_t mode) override
	{
		int flags;

		switch (mode)
		{
			case 0:
				flags = LFS_O_RDONLY;
				break; // Read mode
			case 1:
				flags = LFS_O_WRONLY | LFS_O_CREAT;
				break; // Write mode
			case 2:
				break; // Append mode
			default:
				return false;
		}

		int res = lfs_file_open(&lfs, &file, fileName, flags);

		if (res < 0)
		{
			return false;
		}

		return true;
	}

	/**
	 * @brief Writes data to the file.
	 * @param buffer Data buffer to write.
	 * @param size Number of bytes to write.
	 * @return Number of bytes successfully written.
	 */
	int write(const char* buffer, size_t size) override
	{
		return lfs_file_write(&lfs, &file, buffer, size);
	}

	/**
	 * @brief Reads data from the file.
	 * @param buffer Buffer to store the read data.
	 * @param size Number of bytes to read.
	 * @return Number of bytes successfully read.
	 */
	int read(char* buffer, size_t size) override
	{
		return lfs_file_read(&lfs, &file, buffer, size);
	}

	/**
	 * @brief Closes the currently opened file.
	 * @return 0 on success.
	 */
	int close() override
	{
		return lfs_file_close(&lfs, &file);
	}
};
#endif

/**
 * @brief Wrapper class for handling different filesystem implementations.
 */
class fileSysWrapper
{
  private:
	CFileHandler cHandler; ///< Standard file handler.
#ifdef TARGET_MICRO
	LittleFSHandler littleFSHandler; ///< LittleFS handler for embedded systems.
#endif
	FileHandler* activeHandler = nullptr; ///< Pointer to the active file handler.

  public:
	/**
	 * @brief Constructs a file system wrapper with a specified handler.
	 * @param fs Type of filesystem (0 = standard, 1 = LittleFS if enabled).
	 */
	fileSysWrapper(uint8_t fs)
	{
		if (fs == 0)
		{
			this->activeHandler = &cHandler;
		}
#ifdef TARGET_MICRO
		else if (fs == 1)
		{
			activeHandler = &littleFSHandler;
		}
#endif
	}
	/**
	 * @brief Mounts the selected filesystem.
	 * @return true if successful, false otherwise.
	 */
	bool mount()
	{
#ifdef TARGET_MICRO
		return activeHandler ? activeHandler->mount() : false;
#else
		return true;
#endif
	}

	/**
	 * @brief Opens a file using the selected filesystem.
	 * @param fileName Name of the file to open.
	 * @param mode Access mode: 0 = read, 1 = write, 2 = append.
	 * @return true if the file was successfully opened, false otherwise.
	 */
	bool open(const char* fileName, uint8_t mode)
	{
		return activeHandler ? activeHandler->open(fileName, mode) : false;
	}

	/**
	 * @brief Reads data from an open file.
	 * @param buffer Buffer to store the read data.
	 * @param size Number of bytes to read.
	 * @return Number of bytes successfully read.
	 */
	int read(char* buffer, size_t size)
	{
		return activeHandler ? activeHandler->read(buffer, size) : 0;
	}

	/**
	 * @brief Writes data to an open file.
	 * @param buffer Data buffer to write.
	 * @param size Number of bytes to write.
	 * @return Number of bytes successfully written.
	 */
	int write(const char* buffer, size_t size)
	{
		return activeHandler ? activeHandler->write(buffer, size) : 0;
	}

	/**
	 * @brief Closes the currently opened file.
	 * @return 0 on success.
	 */
	int close()
	{
		return activeHandler->close();
	}

	/**
	 * @brief Destructor, ensuring the file is closed upon object destruction.
	 */
	~fileSysWrapper()
	{
		close(); // Close the file if open
	}
};
