#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "Application.hpp"

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