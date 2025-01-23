#include <iostream>
#include <memory>
#include <csignal>

#include "Application.hpp"
#include "Library.hpp"
#include "Networking.hpp"

std::shared_ptr<Server> server;

void signalHandler(int signum) {
    if (server) {
        server->stop();
    }
    exit(signum);
}

int main() {
    auto library_manager = std::make_shared<LibraryManager>();
    server = std::make_shared<Server>(8080, library_manager);

    std::signal(SIGINT, signalHandler);

    server->start();
}