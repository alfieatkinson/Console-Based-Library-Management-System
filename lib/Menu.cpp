#include <iostream>
#include <limits>
#include "Menu.hpp"

// Constructor
Menu::Menu(const std::string& name, bool paging, size_t page_size)
    : name(name), paging(paging), page_size(page_size) {}

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
bool Menu::display(bool isAdmin) {
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
            return true;   // Valid input
        } else {
            std::cout << "Invalid option, please try again.\n" << std::endl;
            return false;  // Invalid input
        }
    } else {
        // Handle non-integer input
        std::cin.clear(); // Clear error flags
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
        std::cout << "Invalid input, please enter an integer.\n" << std::endl;
        return false;  // Non-integer input
    }
}