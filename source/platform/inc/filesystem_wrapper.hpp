#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#include <cstdint>
#include <cstdio>
#include <cstring>

#ifdef TARGET_MICRO
#include "littleFSInterface.h"
#endif

// Unified file handle interface
struct FileHandler
{
	virtual bool mount() = 0;
	virtual bool open(const char* fileName, uint8_t mode) = 0;
	virtual int read(char* buffer, size_t size) = 0;
	virtual int write(const char* buffer, size_t size) = 0;
	virtual int close() = 0;
	virtual ~FileHandler() = default;
};

// --- C FILE Implementation ---
class CFileHandler : public FileHandler
{
  private:
	FILE* file = nullptr;

  public:
	bool open(const char* fileName, uint8_t mode) override
	{
		const char* cMode = nullptr;
		switch(mode)
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

		if(file == nullptr)
		{
			return false;
		}

		return true;
	}

	bool mount() override
	{
		return true;
	}

	int read(char* buffer, size_t size) override
	{
		if(!file)
			return 0;
		return fread(buffer, 1, size, file);
	}

	int write(const char* buffer, size_t size) override
	{
		if(!file)
			return 0;
		return fwrite(buffer, 1, size, file);
	}

	int close() override
	{
		if(file)
			fclose(file);
		file = nullptr;

		return 0;
	}
};

#ifdef TARGET_MICRO
// --- LittleFS Implementation ---
class LittleFSHandler : public FileHandler
{
  private:
	lfs_t lfs;
	lfs_file_t file;

  public:
	bool mount() override
	{
		flash_init();

		int res = lfs_mount(&lfs, &cfg);
		if(res < 0)
		{
			// If the mount fails, try formatting the filesystem
			res = lfs_format(&lfs, &cfg);
			if(res < 0)
			{
				return false;
			}
			// Try mounting again
			res = lfs_mount(&lfs, &cfg);
			if(res < 0)
			{
				return false;
			}
		}

		return true;
	}

	bool open(const char* fileName, uint8_t mode) override
	{
		int flags;

		switch(mode)
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

		if(res < 0)
		{
			return false;
		}

		return true;
	}

	int write(const char* buffer, size_t size) override
	{
		return lfs_file_write(&lfs, &file, buffer, size);
	}

	int read(char* buffer, size_t size) override
	{
		return lfs_file_read(&lfs, &file, buffer, size);
	}

	int close() override
	{
		return lfs_file_close(&lfs, &file);
	}
};
#endif

// --- Wrapper Function ---
class fileSysWrapper
{
  private:
	// All possible file handler implementations
	CFileHandler cHandler;
	// If additional handlers are needed, declare them directly here:
	// CPPFileHandler cppHandler;
#ifdef TARGET_MICRO
	LittleFSHandler littleFSHandler;
#endif

	// Track the active handler
	FileHandler* activeHandler = nullptr;

  public:
	fileSysWrapper(uint8_t fs)
	{
		if(fs == 0)
		{
			this->activeHandler = &cHandler;
		}
#ifdef TARGET_MICRO
		else if(fs == 1)
		{
			activeHandler = &littleFSHandler;
		}
#endif
	}

	bool mount()
	{
#ifdef TARGET_MICRO
		return activeHandler ? activeHandler->mount() : false;
#else
		return true;
#endif
	}

	bool open(const char* fileName, uint8_t mode)
	{
		return activeHandler ? activeHandler->open(fileName, mode) : false;
	}

	int read(char* buffer, size_t size)
	{
		return activeHandler ? activeHandler->read(buffer, size) : 0;
	}

	int write(const char* buffer, size_t size)
	{
		return activeHandler ? activeHandler->write(buffer, size) : 0;
	}

	int close()
	{
		return activeHandler->close();
	}

	~fileSysWrapper()
	{
		close(); // Close the file if open
	}
};
