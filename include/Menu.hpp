#ifndef MENU_HPP
#define MENU_HPP

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <map>

class Menu {
private:
    bool paging; // Flag to enable pagination
    size_t page_size; // Number of options per page
    int current_page = 0; // Current page number
    std::string name; // Name of the menu
    std::map<std::string, std::function<void()>> options; // Map of options

    // Helper method to display a single page of options
    std::string displayPage(size_t page, bool is_admin);

    // Helper methods for handling navigation and choices
    bool handleNavigation(char choice);
    bool handleChoice(int choice, bool is_admin);

public:
    // Constructor
    Menu(const std::string& name, bool paging = false, size_t page_size = 8);

    // Destructor
    ~Menu();

    // Getters
    std::string getName() const;
    std::map<std::string, std::function<void()>> getOptions() const;

    // Setters
    void setName(const std::string& name);

    // Method to add an option to the menu
    void addOption(const std::string& description, std::function<void()> action, bool is_admin_only = false);

    // Method to display the menu
    std::string display(bool is_admin = false, int page = -1);

    // Method to handle user input
    bool handleInput(std::string input, bool is_admin);
};

#endif // MENU_HPP