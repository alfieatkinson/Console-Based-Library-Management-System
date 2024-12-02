#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "Application.hpp"

// Tests for Application initialisation

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