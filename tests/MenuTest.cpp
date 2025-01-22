#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>
#include <iostream>
#include "Menu.hpp"

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

TEST_CASE("Menu Display Options with Input Handling") {
    Menu menu("Main Menu");
    menu.addOption("Option 1", []() {});
    menu.addOption("Option 2", []() {}, true);

    SECTION("Non-admin user sees only non-admin options") {
        std::string output = menu.display(false);
        REQUIRE(output.find("Option 1") != std::string::npos);
        REQUIRE(output.find("Option 2") == std::string::npos);
    }

    SECTION("Admin user sees all options") {
        std::string output = menu.display(true);
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
        std::ostringstream simulatedOutput;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(simulatedOutput.rdbuf()); // Capture std::cout

        menu.handleInput("1", false); // Call handleInput directly

        std::cout.rdbuf(oldCoutBuffer); // Restore std::cout

        // Verify output
        REQUIRE(simulatedOutput.str().find("Option 1 selected") != std::string::npos);
    }

    SECTION("Menu handles non-integer input") {
        std::ostringstream simulatedOutput;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(simulatedOutput.rdbuf());

        bool validInput = menu.handleInput("invalid", false);

        std::cout.rdbuf(oldCoutBuffer); // Restore std::cout

        // Verify output contains non-integer input handling
        REQUIRE_FALSE(validInput);
    }

    SECTION("Menu handles invalid option") {
        std::ostringstream simulatedOutput;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(simulatedOutput.rdbuf());

        bool validInput = menu.handleInput("3", false);

        std::cout.rdbuf(oldCoutBuffer); // Restore std::cout

        // Verify output contains invalid option handling
        REQUIRE_FALSE(validInput);
    }

    SECTION("Admin-only options displayed correctly") {
        menu.addOption("Admin Option", []() { std::cout << "Admin Option selected" << std::endl; }, true);

        std::ostringstream simulatedOutput;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(simulatedOutput.rdbuf());

        menu.handleInput("3", true); // Admin user

        std::cout.rdbuf(oldCoutBuffer); // Restore std::cout

        REQUIRE(simulatedOutput.str().find("Admin Option selected") != std::string::npos);
        REQUIRE(simulatedOutput.str().find("Option 1 selected") == std::string::npos); // Should not display non-admin option
    }
}

TEST_CASE("Menu Display Options with Paging and Input Handling") {
    Menu menu("Main Menu", true, 2);  // Enable paging with a page size of 2
    menu.addOption("Option 1", []() {});
    menu.addOption("Option 2", []() {});
    menu.addOption("Option 3", []() {});

    SECTION("Navigate through pages and validate options") {
        // Next Page Navigation
        SECTION("Next Page Navigation") {
            menu.handleInput("N", false); // Navigate to next page
            std::string output = menu.display(false);
            REQUIRE(output.find("Option 1") == std::string::npos);
            REQUIRE(output.find("Option 2") == std::string::npos);
            REQUIRE(output.find("Option 3") != std::string::npos);
        }

        // Previous Page Navigation
        SECTION("Previous Page Navigation") {
            menu.handleInput("P", false); // Navigate to previous page
            std::string output = menu.display(false);
            REQUIRE(output.find("Option 1") != std::string::npos);
            REQUIRE(output.find("Option 2") != std::string::npos);
            REQUIRE(output.find("Option 3") == std::string::npos);
        }
    }
}