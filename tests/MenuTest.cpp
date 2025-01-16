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