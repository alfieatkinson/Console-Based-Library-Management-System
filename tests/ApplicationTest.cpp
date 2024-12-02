#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "Application.hpp"

// Tests for initialisation

TEST_CASE("start sets is_running to true") {
    Application app;
    REQUIRE_FALSE(app.is_running);
    app.start();
    REQUIRE(app.is_running);
}

TEST_CASE("start initialises Library") {
    Application app;
    app.start();
    REQUIRE(app.getLibrary() != nullptr);
}

TEST_CASE("start initialises Database") {
    Application app;
    app.start();
    REQUIRE(app.getDatabase() != nullptr);
}

TEST_CASE("start initialises to login menu") {
    Application app;
    app.start();
    REQUIRE(app.getCurrentMenu() == "login");
}

TEST_CASE("start initialises user as null") {
    Application app;
    app.start();
    REQUIRE(app.getCurrentUser() == nullptr);
}

// Tests for login/logout functionality

TEST_CASE("loginUser sets current_user to the correct user") {
    Application app;
    User test_user(1, "test", "password123")
    app.start();
    app.loginUser(test_user);
    REQUIRE(app.getCurrentUser() == &test_user);
}

TEST_CASE("logoutUser sets current_user to null") {
    Application app;
    User test_user(1, "test", "password123")
    app.start();
    app.loginUser(test_user);
    app.logoutUser();
    REQUIRE(app.getCurrentUser() == nullptr);
}

// Tests for menu display functionality

TEST_CASE("displayMenu sets current_menu to the correct menu") {
    Application app;
    app.start();
    app.displayMenu("main");
    REQUIRE(app.getCurrentMenu() == "main");
}

TEST_CASE("displayMenu sets current_menu to login if no menu is specified") {
    Application app;
    app.start();
    app.displayMenu();
    REQUIRE(app.getCurrentMenu() == "login");
}

// Tests for processing user choices

TEST_CASE("processChoice calls correct function for valid choice") {
    Application app;
    app.start();

    bool option1_called = false;
    bool option2_called = false;

    // Setting up mock functions to track calls
    auto option1_fn = [&]() { option1_called = true; };
    auto option2_fn = [&]() { option2_called = true; };

    // Overriding functions for the test
    app.menus["main"] = {
        {{"Option 1", option1_fn}},
        {{"Option 2", option2_fn}}
    };

    // User selects Option 1
    app.processChoice("main", 0);
    REQUIRE(option1_called);
    REQUIRE_FALSE(option2_called);

    // User selects Option 2
    app.processChoice("main", 1);
    REQUIRE(option2_called);
}

TEST_CASE("processChoice handles invalid choice gracefully") {
    Application app;
    app.start();

    bool option1_called = false;

    // Mock function for option 1
    auto option1_fn = [&]() { option1_called = true; };

    // Set a menu with one valid option
    app.menus["main"] = {
        {{"Option 1", option1_fn}}
    };

    // Invalid choice (index out of range)
    app.processChoice("main", 5);
    REQUIRE_FALSE(option1_called);
}

TEST_CASE("processChoice works with multiple menus") {
    Application app;
    app.start();

    bool menu1_option_called = false;
    bool menu2_option_called = false;

    // Mock functions for each menu
    auto menu1_fn = [&]() { menu1_option_called = true; };
    auto menu2_fn = [&]() { menu2_option_called = true; };

    // Set menus with different options
    app.menus["menu1"] = {
        {{"Menu 1 Option", menu1_fn}},
    };
    app.menus["menu2"] = {
        {{"Menu 2 Option", menu2_fn}},
    };

    // User selects option from menu1
    app.processUserChoice("menu1", 0);
    REQUIRE(menu1_option_called);

    // User selects option from menu2
    app.processUserChoice("menu2", 0);
    REQUIRE(menu2_option_called);
}

TEST_CASE("processChoice handles invalid menu gracefully") {
    Application app;
    app.start();

    bool fn_called = false;
    auto fn = [&]() { fn_called = true; };

    // Set a valid menu
    app.menus["main"] = {
        {{"Option", fn}},
    };

    // Invalid menu
    app.processUserChoice("invalid_menu", 0);
    REQUIRE_FALSE(fn_called);
}