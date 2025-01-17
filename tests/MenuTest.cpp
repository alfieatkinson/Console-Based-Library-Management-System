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

TEST_CASE("Menu Display Options with Input Handling") {
    Menu menu("Main Menu");
    menu.addOption("Option 1", []() {});
    menu.addOption("Option 2", []() {}, true);

    SECTION("Non-admin user sees only non-admin options") {
        std::istringstream simulatedInput("1\n"); // Simulate selecting an option
        std::cin.rdbuf(simulatedInput.rdbuf()); // Redirect std::cin

        std::ostringstream simulatedOutput;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(simulatedOutput.rdbuf());

        menu.display(false); // Non-admin display

        std::cout.rdbuf(oldCoutBuffer); // Restore std::cout
        std::cin.rdbuf(std::cin.rdbuf()); // Restore std::cin

        REQUIRE(simulatedOutput.str().find("Option 1") != std::string::npos);
        REQUIRE(simulatedOutput.str().find("Option 2") == std::string::npos);
    }

    SECTION("Admin user sees all options") {
        std::istringstream simulatedInput("2\n"); // Simulate selecting an admin option
        std::cin.rdbuf(simulatedInput.rdbuf()); // Redirect std::cin

        std::ostringstream simulatedOutput;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(simulatedOutput.rdbuf());

        menu.display(true); // Admin display

        std::cout.rdbuf(oldCoutBuffer); // Restore std::cout
        std::cin.rdbuf(std::cin.rdbuf()); // Restore std::cin

        REQUIRE(simulatedOutput.str().find("Option 1") != std::string::npos);
        REQUIRE(simulatedOutput.str().find("[Admin] Option 2") != std::string::npos);
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

    SECTION("Menu handles non-integer input") {
        std::istringstream simulatedInput("invalid\n"); // Simulate non-integer input
        std::cin.rdbuf(simulatedInput.rdbuf()); // Redirect std::cin

        std::ostringstream simulatedOutput;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(simulatedOutput.rdbuf());

        bool validInput = menu.display(false);

        std::cout.rdbuf(oldCoutBuffer); // Restore std::cout
        std::cin.rdbuf(std::cin.rdbuf()); // Restore std::cin

        // Verify output contains non-integer input handling
        REQUIRE(simulatedOutput.str().find("Invalid input, please enter an integer.") != std::string::npos);
    }

    SECTION("Menu handles invalid option") {
        std::istringstream simulatedInput("3\n"); // Simulate invalid option followed by a valid option
        std::cin.rdbuf(simulatedInput.rdbuf()); // Redirect std::cin

        std::ostringstream simulatedOutput;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(simulatedOutput.rdbuf());

        bool validInput = menu.display(false);

        std::cout.rdbuf(oldCoutBuffer); // Restore std::cout
        std::cin.rdbuf(std::cin.rdbuf()); // Restore std::cin

        // Verify output contains invalid option handling
        REQUIRE(simulatedOutput.str().find("Invalid option, please try again.") != std::string::npos);
    }

    SECTION("Admin-only options displayed correctly") {
        menu.addOption("Admin Option", []() { std::cout << "Admin Option selected" << std::endl; }, true);

        std::istringstream simulatedInput("3\n"); // Admin input
        std::cin.rdbuf(simulatedInput.rdbuf());

        std::ostringstream simulatedOutput;
        std::streambuf* oldCoutBuffer = std::cout.rdbuf(simulatedOutput.rdbuf());

        menu.display(true); // Admin user

        std::cout.rdbuf(oldCoutBuffer); // Restore std::cout
        std::cin.rdbuf(std::cin.rdbuf()); // Restore std::cin

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
        // First Page Navigation
        SECTION("First Page Navigation") {
            std::istringstream simulatedInput1("N\n");  // Navigate Next
            std::cin.rdbuf(simulatedInput1.rdbuf()); 

            std::ostringstream simulatedOutput1;
            std::streambuf* oldCoutBuffer1 = std::cout.rdbuf(simulatedOutput1.rdbuf());

            menu.display(false); // Non-admin display

            std::cout.rdbuf(oldCoutBuffer1);
            std::cin.rdbuf(std::cin.rdbuf());

            std::cout << simulatedOutput1.str() << std::endl;

            REQUIRE(simulatedOutput1.str().find("Option 3") != std::string::npos);  // Next Page, Option 3 should be on the next page
        }

        // Last Page Navigation
        SECTION("Last Page Navigation") {
            std::istringstream simulatedInput2("P\n");  // Navigate Previous
            std::cin.rdbuf(simulatedInput2.rdbuf());

            std::ostringstream simulatedOutput2;
            std::streambuf* oldCoutBuffer2 = std::cout.rdbuf(simulatedOutput2.rdbuf());

            menu.display(false, 1); // Start on the second page

            std::cout.rdbuf(oldCoutBuffer2);
            std::cin.rdbuf(std::cin.rdbuf());

            std::cout << simulatedOutput2.str() << std::endl;

            REQUIRE(simulatedOutput2.str().find("Option 1") != std::string::npos);  // Back to first page
        }

        // Specific Page Navigation
        SECTION("Specific Page Navigation") {
            std::istringstream simulatedInput3("S\n2\n");  // Navigate to second page
            std::cin.rdbuf(simulatedInput3.rdbuf());

            std::ostringstream simulatedOutput3;
            std::streambuf* oldCoutBuffer3 = std::cout.rdbuf(simulatedOutput3.rdbuf());

            menu.display(false);

            std::cout.rdbuf(oldCoutBuffer3);
            std::cin.rdbuf(std::cin.rdbuf());

            std::cout << simulatedOutput3.str() << std::endl;

            REQUIRE(simulatedOutput3.str().find("Option 3") != std::string::npos);  // On second page

            // Invalid Specific Page Navigation
            std::istringstream simulatedInput4("S\n3\n");  // Invalid page
            std::cin.rdbuf(simulatedInput4.rdbuf());

            std::ostringstream simulatedOutput4;
            std::streambuf* oldCoutBuffer4 = std::cout.rdbuf(simulatedOutput4.rdbuf());

            bool validInput4 = menu.display(false);

            std::cout.rdbuf(oldCoutBuffer4);
            std::cin.rdbuf(std::cin.rdbuf());

            REQUIRE(simulatedOutput4.str().find("Invalid page number.") != std::string::npos);  // Invalid page should show an error
        }
    }
}