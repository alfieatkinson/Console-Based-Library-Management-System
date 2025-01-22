#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <string>
#include <atomic>
#include "Networking.hpp"
#include "Application.hpp"
#include "Library.hpp"

std::atomic<bool> running{true};

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
    std::cout << "Server started, waiting for connections at " << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port) << std::endl;
    while (running) {
        int client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (client_socket < 0) {
            if (!running) {
                std::cout << "Server shutting down..." << std::endl;
                break; // If shutdown was requested, exit gracefully
            }
            perror("accept failed");
            continue; // Skip this iteration and continue waiting for connections
        }
        std::cout << "Accepted connection on socket " << client_socket << std::endl;
        client_threads.emplace_back(&Server::handleClient, this, client_socket);
    }
    for (auto& thread : client_threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

// Method to stop the server
void Server::stop() {
    running = false;
    close(server_fd); // Close the server socket to unblock the accept call
}

// Method to handle a client connection
void Server::handleClient(int client_socket) {
    // RAII guard for the client socket using a custom deleter
    auto socket_guard = std::unique_ptr<int, std::function<void(int*)>>(
        new int(client_socket),  // Allocate and initialise the resource
        [](int* socket) { 
            if (socket) { 
                close(*socket);  // Close the socket using the dereferenced value
                delete socket;   // Free the allocated memory
            }
        }
    );

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