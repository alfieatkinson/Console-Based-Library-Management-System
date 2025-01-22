#ifndef NETWORKING_HPP
#define NETWORKING_HPP

#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <string>
#include <netinet/in.h>
#include <mutex>
#include "Application.hpp"
#include "Library.hpp"

class Server {
private:
    int server_fd;
    struct sockaddr_in address;
    int addrlen;
    std::shared_ptr<LibraryManager> library_manager;
    std::vector<std::thread> client_threads;
    std::mutex client_threads_mutex;

    // Method to handle a client connection
    void handleClient(int client_socket);

public:
    // Constructor
    Server(int port, std::shared_ptr<LibraryManager> library_manager);

    // Destructor
    ~Server();

    // Method to start the server
    void start();

    // Method to stop the server
    void stop();
};

#endif // NETWORKING_HPP
