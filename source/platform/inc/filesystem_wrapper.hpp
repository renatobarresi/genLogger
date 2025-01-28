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
	virtual size_t read(char* buffer, size_t size) = 0;
	virtual size_t write(const char* buffer, size_t size) = 0;
	virtual void close() = 0;
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

	size_t read(char* buffer, size_t size) override
	{
		if(!file)
			return 0;
		return fread(buffer, 1, size, file);
	}

	size_t write(const char* buffer, size_t size) override
	{
		if(!file)
			return 0;
		return fwrite(buffer, 1, size, file);
	}

	void close() override
	{
		if(file)
			fclose(file);
		file = nullptr;
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
	bool open(const char* fileName, uint8_t mode) override
	{
		const char* fsMode = nullptr;
		switch(mode)
		{
			case 0:
				fsMode = "r";
				break; // Read mode
			case 1:
				fsMode = "w";
				break; // Write mode
			case 2:
				fsMode = "a";
				break; // Append mode
			default:
				return false;
		}

		if(lfs_file_open(&lfs, &file, fileName, LFS_O_WRONLY | LFS_O_CREAT) < 0)
		{
			return false;
		}

		return true;
	}

	bool mount() override
	{
		flash_init();
		int res = lfs_mount(&lfs, &cfg);
		if(res < 0)
		{
			// If the mount fails, try formatting the filesystem
			res = lfs_format(&lfs, &cfg);
		}
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
		return true;
	}
	size_t read(char* buffer, size_t size) override
	{
		return 0;
	}

	size_t write(const char* buffer, size_t size) override
	{
		return 0;
	}

	void close() override
	{
		//
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

	size_t read(char* buffer, size_t size)
	{
		return activeHandler ? activeHandler->read(buffer, size) : 0;
	}

	size_t write(const char* buffer, size_t size)
	{
		return activeHandler ? activeHandler->write(buffer, size) : 0;
	}

	void close()
	{
		if(activeHandler)
			activeHandler->close();
	}

	~fileSysWrapper()
	{
		close(); // Close the file if open
	}
};
