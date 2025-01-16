#include <iostream>
#include <limits>
#include "Menu.hpp"

// Constructor
Menu::Menu(const std::string& name) : name(name) {}

// Destructor
Menu::~Menu() {}

// Getters
std::string Menu::getName() const {
    return name;
}

std::map<std::string, std::function<void()>> Menu::getOptions() const {
    return options;
}

// Setters
void Menu::setName(const std::string& name) {
    this->name = name;
}

// Method to add an option to the menu
void Menu::addOption(const std::string& description, std::function<void()> action, bool isAdminOnly) {
    if (!isAdminOnly) {
        options[description] = action;
    } else {
        options["[Admin] " + description] = action;
    }
}

// Method to display the menu
void Menu::display(bool isAdmin) {
    while (true) { // Use a loop instead of recursion for robustness
        std::cout << name << std::endl;

        int i = 0;
        for (const auto& option : options) {
            if (isAdmin || option.first.find("[Admin]") == std::string::npos) {
                std::cout << ++i << ". " << option.first << std::endl;
            }
        }

        std::cout << "Enter your choice: ";
        int choice;

        // Validate input
        if (std::cin >> choice) {
            if (choice > 0 && choice <= static_cast<int>(options.size())) {
                auto it = std::next(options.begin(), choice - 1);
                it->second();  // Execute the function associated with the option
                break; // Exit loop after valid choice
            } else {
                std::cout << "Invalid option, please try again.\n" << std::endl;
            }
        } else {
            // Handle non-integer input
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Invalid input, please enter a number.\n" << std::endl;
        }
    }
}

// Method to clear the console
void Menu::clearConsole() {
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        system("clear");
    #endif
}