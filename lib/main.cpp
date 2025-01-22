#include <iostream>
#include <memory>

#include "Application.hpp"
#include "Library.hpp"
#include "Networking.hpp"

int main() {
    auto library_manager = std::make_shared<LibraryManager>();
    Server server(8080, library_manager);
    server.start();
}