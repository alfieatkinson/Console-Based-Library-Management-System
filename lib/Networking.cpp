#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <string>
#include "Networking.hpp"
#include "Application.hpp"
#include "Library.hpp"

// Constructor
Server::Server(int port, std::shared_ptr<LibraryManager> library_manager)
    : library_manager(library_manager), addrlen(sizeof(address)) {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

// Destructor
Server::~Server() {
    close(server_fd);
    for (auto& thread : client_threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

// Method to start the server
void Server::start() {
    std::cout << "Server started, waiting for connections..." << std::endl;
    while (true) {
        int client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (client_socket < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        client_threads.emplace_back(&Server::handleClient, this, client_socket);
    }
}

// Method to handle a client connection
void Server::handleClient(int client_socket) {
    std::unique_ptr<int, decltype(&close)> socket_guard(&client_socket, close); // RAII guard for the client socket
    std::cout << "Client connected at socket " << client_socket << std::endl;

    try {
        // Create and run the application
        auto app = std::make_shared<Application>(client_socket, library_manager);
        app->run();
    } catch (const std::runtime_error& e) {
        // Handle any runtime errors
        std::cerr << "Error: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "An unknown error occurred." << std::endl;
    }

    // Clean up and close the socket
    close(client_socket);
}