#include <csignal>
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <unordered_map>

// Mapping key presses to signals
std::unordered_map<char, int> keyToSignal = {
	{'I', SIGUSR1},						 // Signal for 'I'
	{'C', SIGRTMIN},	 {'B', SIGUSR2}, // Signal for 'C'
	{'N', SIGRTMIN + 1},				 // Real-time signal for 'N'
	{'T', SIGRTMIN + 2},				 // Real-time signal for 'T'
	{'S', SIGRTMIN + 3}					 // Real-time signal for 'S'
};

// Function to read a single keypress (non-blocking, no Enter required)
int getKeyPress()
{
	struct termios oldt, newt;
	int			   ch;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	ch = getchar(); // Read a single character

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}

int main()
{
	int pid;
	std::cout << "Enter the PID of the receiver process: ";
	std::cin >> pid;
	std::cin.ignore(); // Ignore newline left in buffer

	std::cout << "Press I, C, B, N, T, S to send signals, Q to quit.\n";

	while (true)
	{
		char key = getKeyPress();
		key		 = toupper(key); // Convert to uppercase for consistency

		if (keyToSignal.find(key) != keyToSignal.end())
		{
			kill(pid, keyToSignal[key]);
			std::cout << "Sent signal for key '" << key << "' to PID " << pid << "\n";
		}
		else if (key == 'Q')
		{
			std::cout << "Exiting sender...\n";
			break;
		}
	}

	return 0;
}
