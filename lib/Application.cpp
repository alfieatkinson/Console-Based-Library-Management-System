#include <iostream>
#include <sstream>
#include <limits>
#include "Application.hpp"

// Constructor
Application::Application() {
    initialiseMenus();
}

// Destructor
Application::~Application() {}

// Initialisation methods
void Application::initialiseMenus() {
    menu_stack.push(makeLoginMenu());
}

void Application::promptAdminPassword() {
    std::string input = promptInput("Enter the admin password: ");
    if (library.authenticateAdmin(input)) {
        is_admin = true;
        menu_stack.push(makeMainMenu());
    }
    else {
        std::cout << "Incorrect password. Please try again." << std::endl;
    }
}