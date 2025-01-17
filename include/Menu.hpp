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
    std::string name; // Name of the menu
    std::map<std::string, std::function<void()>> options; // Map of options
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
    void addOption(const std::string& description, std::function<void()> action, bool isAdminOnly = false);

    // Method to display the menu
    bool display(bool isAdmin = false);
};

#endif // MENU_HPP