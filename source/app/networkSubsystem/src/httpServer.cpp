#include "httpServer.hpp"
#include <cstring> // strlen, memset (C string ops)
#include <iostream>
#include <netinet/in.h> // socket structures
#include <sys/socket.h> // socket functions (socket, bind, listen, accept)
#include <unistd.h>		// close, read, write

void handle_client(int client_sock)
{
	// Set a receive timeout on the client socket to prevent blocking indefinitely
	// if the client is slow or sends a malformed request.
	struct timeval tv;
	tv.tv_sec  = 2; // 2-second timeout
	tv.tv_usec = 0;
	setsockopt(client_sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

	char buffer[1024] = {0};
	// Use ssize_t for the return of read() as it can be -1 on error.
	ssize_t bytes_read = read(client_sock, buffer, sizeof(buffer) - 1);

	if (bytes_read > 0)
	{
		std::cout << "Received " << bytes_read << " bytes from client:\n" << buffer << std::endl;
	}
	else
	{
		// An error occurred, which could be a timeout or the client closing the connection.
		perror("read from client failed or timed out");
	}

	const char* response = "HTTP/1.1 200 OK\r\n"
						   "Content-Length: 2\r\n"
						   "Content-Type: text/plain\r\n"
						   "\r\n"
						   "OK";

	send(client_sock, response, strlen(response), 0);
	close(client_sock);
}

void httpServer(int& ready)
{
	std::cout << "Running server" << std::endl;
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
	{
		perror("socket creation failed");
		return;
	}

	sockaddr_in address{};
	address.sin_family		= AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port		= htons(8081);

	int opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		perror("setsockopt(SO_REUSEADDR) failed");
		close(server_fd);
		ready = 2;
		return;
	}

	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		close(server_fd);
		ready = 2;
		return;
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen failed");
		close(server_fd);
		ready = 2;
		return;
	}

	// Signal that server is ready
	ready = 1;

	int addrlen		= sizeof(address);
	int client_sock = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
	if (client_sock >= 0)
	{
		handle_client(client_sock);
	}
	else
	{
		perror("accept failed");
	}

	close(server_fd);
}