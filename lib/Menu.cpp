#include <iostream>
#include <sstream>
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
void Menu::addOption(const std::string& description, std::function<void()> action, bool is_admin_only) {
    if (!is_admin_only) {
        options[description] = action;
    } else {
        options["[Admin] " + description] = action;
    }
}

// Helper method to display a single page of options
std::string Menu::displayPage(size_t page, bool is_admin) {
    std::ostringstream oss;

    // Adjust page size by +1 to account for the "[BACK]" option
    size_t adjusted_page_size = page_size + 1;

    size_t start_index = page * page_size;
    size_t end_index = std::min(start_index + page_size, options.size());

    auto it = options.begin();
    std::advance(it, start_index);

    oss << std::endl;
    oss << name << " - Page " << (page + 1) << "/"
              << (options.size() + adjusted_page_size) / adjusted_page_size << std::endl;

    int i = 1;
    for (size_t index = start_index; index < end_index; ++index) {
        if (is_admin || it->first.find("[Admin]") == std::string::npos) {
            oss << i << ". " << it->first << std::endl;
        }
        ++it;
        ++i;
    }

    // Always display "[BACK]" as the last option
    oss << i << ". " << "[BACK]" << std::endl;

    // Navigation options
    if (page == 0) {
        oss << "[N] Next" << std::endl;
    } else if (end_index == options.size()) {
        oss << "[P] Previous" << std::endl;
    } else {
        oss << "[P] Previous  [N] Next" << std::endl;
    }

    return oss.str();
}

// Helper methods for handling navigation and choices
bool Menu::handleNavigation(char choice) {
    if (choice == 'P' || choice == 'p') {
        if (current_page > 0) --current_page;
    } else if (choice == 'N' || choice == 'n') {
        if ((current_page + 1) * (page_size + 1) <= options.size()) ++current_page;
    } else {
        return false;
    }
    return true;
}

bool Menu::handleChoice(int choice, bool is_admin) {
    if (paging) {
        choice += current_page * (page_size + 1);
    }

    if (choice > 0 && choice <= static_cast<int>(options.size())) {
        auto it = std::next(options.begin(), choice - 1);

        // Check if the selected option is an admin option and the user is not an admin
        if ((!is_admin && it->first.find("[Admin]") != std::string::npos)
            || (paging && choice == current_page * (page_size + 1) + page_size)) {
            // Use the last option in the map as fallback
            auto last_it = std::prev(options.end());
            last_it->second(); // Execute the last option's action
            return true;
        }

        // Execute the chosen option if it's valid for the user
        it->second();
        return true;
    } else {
        return false; // Invalid input
    }
}

// Method to display the menu
std::string Menu::display(bool is_admin, int page) {
    std::ostringstream oss;

    if (page != -1) {
        current_page = page;
    }

    if (paging && options.size() > page_size - 1) {
        oss << displayPage(current_page, is_admin);
    } else {
        oss << std::endl;
        oss << name << std::endl;
        size_t i = 1;
        for (const auto& option : options) {
            if (is_admin || option.first.find("[Admin]") == std::string::npos) {
                oss << i++ << ". " << option.first << std::endl;
            }
        }
    }

    oss << "\nEnter your choice: ";
    return oss.str();
}

bool Menu::handleInput(std::string input, bool is_admin) {
    std::istringstream iss(input);
    char choice;

    if (iss >> choice) {
        if (paging) {
            if (!handleNavigation(choice)) {
                try {
                    return handleChoice(std::stoi(std::string(1, choice)), is_admin);
                } catch (const std::invalid_argument&) {
                    return false;
                }
            }
            return true;
        } else {
            int index;
            try {
                return handleChoice(std::stoi(std::string(1, choice)), is_admin);
            } catch (const std::invalid_argument&) {
                return false;
            }
        }
    } else {
        return false;
    }
}