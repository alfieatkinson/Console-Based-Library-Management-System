#ifndef MENU_HPP
#define MENU_HPP

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <map>

class Menu {
private:
    std::string name;
    std::map<std::string, std::function<void()>> options;
public:
    Menu(const std::string& name);
    ~Menu();

    // Getters
    std::string getName() const;
    std::map<std::string, std::function<void()>> getOptions() const;

    // Setters
    void setName(const std::string& name);

    // Method to add an option to the menu
    void addOption(const std::string& description, std::function<void()> action, bool isAdminOnly = false);

    // Method to display the menu
    void display(bool isAdmin = false);

    // Method to clear the console
    void clearConsole();
};

#endif // MENU_HPP