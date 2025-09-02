#include <netinet/in.h>     // socket structures
#include <sys/socket.h>     // socket functions (socket, bind, listen, accept)
#include <unistd.h>         // close, read, write
#include <cstring>          // strlen, memset (C string ops)
#include <iostream>         // std::cout
#include "httpServer.hpp"

void handle_client(int client_sock) {
    char buffer[1024] = {0};
    read(client_sock, buffer, sizeof(buffer));
    std::cout << "Received:\n" << buffer << std::endl;

    const char* response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 2\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "OK";

    send(client_sock, response, strlen(response), 0);
    close(client_sock);
}

void httpServer(std::promise<void>& ready) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    // Signal that server is ready
    ready.set_value();

    int addrlen = sizeof(address);
    int client_sock = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (client_sock >= 0) {
        handle_client(client_sock);
    }

    close(server_fd);
}