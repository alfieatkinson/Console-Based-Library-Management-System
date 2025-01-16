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

