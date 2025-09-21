#include "sensorSimulatorConsumer.hpp"

#include <csignal>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

namespace sensorSimulator
{

constexpr char*	 producerPath = "/home/renato/renato/CESE_fiuba/proyecto_final/genLogger/firmware/source/app/measurementSubsystem/sensors/sensorSimulator/sensorSimulatorProducer.py";
constexpr char*	 name		  = "sensors";
constexpr size_t SIZE		  = 4096;

static int	 shm_fd;
static void* ptr;
static pid_t child_pid;

bool getMemInitFlag()
{
	return ptr != nullptr;
}

static void cleanup(int signum)
{
	std::cout << "\nSENSOR SIMULATOR: Received Ctrl+C, cleaning up..." << std::endl;

	// Kill Python child process
	if (child_pid > 0)
	{
		kill(child_pid, SIGTERM);		// Graceful termination
		waitpid(child_pid, nullptr, 0); // Wait for child to exit
		std::cout << "SENSOR SIMULATOR: Python sensor simulator terminated." << std::endl;
	}

	// Unmap and close shared memory
	munmap(ptr, SIZE);
	close(shm_fd);

	// Unlink shared memory
	shm_unlink(name);
	std::cout << "Shared memory unlinked." << std::endl;

	exit(0);
}

void init(void)
{
	std::cout << "SENSOR SIMULATOR: Running producer..." << std::endl;

	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	if (shm_fd == -1)
	{
		perror("shm_open failed");
		exit(1);
	}

	ftruncate(shm_fd, sizeof(sensorOutput));

	ptr = mmap(0, sizeof(sensorOutput), PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED)
	{
		perror("mmap failed");
		shm_unlink(name);
		exit(1);
	}

	// Set up signal handler for Ctrl+C
	signal(SIGINT, cleanup);

	// Fork and exec Python consumer
	child_pid = fork();
	if (child_pid == 0)
	{
		// Child process: run python consumer
		execlp("python3", "python3", producerPath, name, (char*)NULL);
		perror("execlp failed"); // If execlp fails
		exit(1);
	}
	else if (child_pid > 0)
	{
		// Parent process: wait for Ctrl+C
		std::cout << "SENSOR SIMULATOR: Python consumer launched. Press Ctrl+C to stop." << std::endl;

		// Infinite loop until signal
		// while (true)
		// {
		//     pause();  // Wait for signals
		// }
	}
	else
	{
		perror("fork failed");
		cleanup(SIGINT);
	}
}

sensorOutput readSharedMemory(void)
{
	sensorOutput output{};

	if (!ptr)
	{
		std::cerr << "SENSOR SIMULATOR: Shared memory not initialized." << std::endl;
		exit(1);
	}

	std::memcpy(&output, ptr, sizeof(sensorOutput));

	return output;
}
} // namespace sensorSimulator
