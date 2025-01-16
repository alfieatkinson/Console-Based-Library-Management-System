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

TEST_CASE("Menu Handles User Input") {
    Menu menu("Main Menu");

    // Add options to the menu
    menu.addOption("Option 1", []() {
        std::cout << "Option 1 selected" << std::endl;
    });
    menu.addOption("Option 2", []() {
        std::cout << "Option 2 selected" << std::endl;
    });

    SECTION("Menu handles valid input") {
        std::istringstream simulatedInput("1\n"); // Simulate entering "1"
        std::cin.rdbuf(simulatedInput.rdbuf());   // Redirect std::cin

        std::ostringstream simulatedOutput;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(simulatedOutput.rdbuf()); // Capture std::cout

        menu.display(false); // Call display for a non-admin user

        std::cout.rdbuf(oldCoutBuffer); // Restore std::cout
        std::cin.rdbuf(std::cin.rdbuf()); // Restore std::cin

        // Verify output
        REQUIRE(simulatedOutput.str().find("Option 1 selected") != std::string::npos);
    }

    SECTION("Menu handles invalid input") {
        std::istringstream simulatedInput("invalid\n1\n"); // Simulate invalid input followed by "1"
        std::cin.rdbuf(simulatedInput.rdbuf());

        std::ostringstream simulatedOutput;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(simulatedOutput.rdbuf());

        menu.display(false);

        std::cout.rdbuf(oldCoutBuffer); // Restore std::cout
        std::cin.rdbuf(std::cin.rdbuf()); // Restore std::cin

        // Verify output contains invalid input handling and success case
        REQUIRE(simulatedOutput.str().find("Invalid option, please try again.") != std::string::npos);
        REQUIRE(simulatedOutput.str().find("Option 1 selected") != std::string::npos);
    }
}

TEST_CASE("Clear Console Works") {
    Menu menu("Main Menu");

    SECTION("ClearConsole does not throw errors") {
        REQUIRE_NOTHROW(menu.clearConsole());
    }
}