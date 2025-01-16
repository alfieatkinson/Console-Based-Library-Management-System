#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>
#include <iostream>
#include "Menu.hpp"

// Helper for redirecting console output
std::string captureConsoleOutput(std::function<void()> func) {
    std::ostringstream output;
    std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());
    func();
    std::cout.rdbuf(oldCoutBuffer); // Reset cout
    return output.str();
}

TEST_CASE("Menu Constructor Initialises Correctly") {
    Menu menu("Main Menu");

    REQUIRE(menu.getName() == "Main Menu");
    REQUIRE(menu.getOptions().empty());
}

TEST_CASE("Menu Getters and Setters") {
    Menu menu("Main Menu");

    SECTION("GetName returns correct name") {
        REQUIRE(menu.getName() == "Main Menu");
    }

    SECTION("SetName updates the menu name") {
        menu.setName("New Menu");
        REQUIRE(menu.getName() == "New Menu");
    }
}

TEST_CASE("Menu Adding Options") {
    Menu menu("Main Menu");

    SECTION("Add a non-admin option") {
        menu.addOption("Option 1", []() {});
        auto options = menu.getOptions();
        REQUIRE(options.find("Option 1") != options.end());
    }

    SECTION("Add an admin-only option") {
        menu.addOption("Option 2", []() {}, true);
        auto options = menu.getOptions();
        REQUIRE(options.find("[Admin] Option 2") != options.end());
    }
}

TEST_CASE("Menu Display Options") {
    Menu menu("Main Menu");
    menu.addOption("Option 1", []() {});
    menu.addOption("Option 2", []() {}, true);

    SECTION("Non-admin user sees only non-admin options") {
        std::string output = captureConsoleOutput([&]() {
            menu.display(false);
        });
        REQUIRE(output.find("Option 1") != std::string::npos);
        REQUIRE(output.find("Option 2") == std::string::npos);
    }

    SECTION("Admin user sees all options") {
        std::string output = captureConsoleOutput([&]() {
            menu.display(true);
        });
        REQUIRE(output.find("Option 1") != std::string::npos);
        REQUIRE(output.find("[Admin] Option 2") != std::string::npos);
    }
}