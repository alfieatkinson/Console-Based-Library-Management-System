#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "User.hpp"

TEST_CASE("User Constructor initialises correctly") {
    User user(1, "John", "Doe", "johndoe@email.com", "01234567890", "john_doe", "password123");

    SECTION("User ID is correct") {
        REQUIRE(user.getUserID() == 1);
    }

    SECTION("Forename is correct") {
        REQUIRE(user.getForename() == "John");
    }

    SECTION("Surname is correct") {
        REQUIRE(user.getSurname() == "Doe");
    }

    SECTION("Email is correct") {
        REQUIRE(user.getEmail() == "johndoe@email.com");
    }

    SECTION("Phone number is correct") {
        REQUIRE(user.getPhoneNumber() == "01234567890");
    }

    SECTION("Username is correct") {
        REQUIRE(user.getUsername() == "john_doe");
    }

    SECTION("Password is correct") {
        REQUIRE(user.getPassword() == "password123");
    }
}