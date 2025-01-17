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
void Menu::addOption(const std::string& description, std::function<void()> action, bool is_admin_only) {
    if (!is_admin_only) {
        options[description] = action;
    } else {
        options["[Admin] " + description] = action;
    }
}

// Helper method to display a single page of options
void Menu::displayPage(size_t page, bool is_admin) {
    size_t start_index = page * page_size;
    size_t end_index = std::min(start_index + page_size, options.size());

    auto it = options.begin();
    std::advance(it, start_index);

    std::cout << std::endl;
    std::cout << name << " - Page " << (page + 1) << "/"
              << (options.size() + page_size - 1) / page_size << std::endl;

    size_t i = 1;
    for (size_t index = start_index; index < end_index; ++index) {
        if (is_admin || it->first.find("[Admin]") == std::string::npos) {
            std::cout << i << ". " << it->first << std::endl;
        }
        ++it;
        ++i;
    }

    if (page == 0) {
        std::cout << "[N] Next  [S] Specific Page" << std::endl;
    }
    else if (end_index == options.size()) {
        std::cout << "[P] Previous  [S] Specific Page" << std::endl;
    }
    else {
        std::cout << "[P] Previous  [N] Next  [S] Specific Page" << std::endl;
    }
}

// Helper methods for handling navigation and choices
bool Menu::handleNavigation(char choice, size_t& current_page) {
    if (choice == 'P' || choice == 'p') {
        if (current_page > 0) --current_page;
    } else if (choice == 'N' || choice == 'n') {
        if ((current_page + 1) * page_size < options.size()) ++current_page;
    } else if (choice == 'S' || choice == 's') {
        int target_page;
        std::cout << "\nEnter the page number you want to navigate to: ";
        if (std::cin >> target_page && target_page > 0 && target_page <= (options.size() + page_size - 1) / page_size) {
            current_page = target_page - 1;
            return true;
        } else {
            std::cout << "\nInvalid page number.\n" << std::endl;
            return false;
        }
    } else {
        return false;
    }
    return true;
}

bool Menu::handleChoice(int choice) {
    if (choice > 0 && choice <= static_cast<int>(options.size())) {
        auto it = std::next(options.begin(), choice - 1);
        it->second();  // Execute the function associated with the option
        return true;   // Valid input
    } else {
        std::cout << "\nInvalid option, please try again.\n" << std::endl;
        return false;  // Invalid input
    }
}

// Method to display the menu
bool Menu::display(bool is_admin, size_t current_page) {
    while (true) {
        if (paging && options.size() > page_size) {
            displayPage(current_page, is_admin);
        } else {
            std::cout << std::endl;
            std::cout << name << std::endl;
            size_t i = 1;
            for (const auto& option : options) {
                if (is_admin || option.first.find("[Admin]") == std::string::npos) {
                    std::cout << i++ << ". " << option.first << std::endl;
                }
            }
        }

        std::cout << "\nEnter your choice: ";
        char choice;

        if (std::cin >> choice) {
            if (paging) {
                if (!handleNavigation(choice, current_page)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "\nInvalid input, please try again.\n" << std::endl;
                }
            } else {
                int index;
                if (handleChoice(std::stoi(std::string(1, choice)))) {
                    return true;
                } else {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "\nInvalid option, please try again.\n" << std::endl;
                    return false;
                }
            }
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nInvalid input, please enter an integer.\n" << std::endl;
            return false;
        }
    }
}