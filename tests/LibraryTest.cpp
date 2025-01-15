#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "Library.hpp"
#include "Book.hpp"
#include "User.hpp"
#include "Transaction.hpp"

std::string getCurrentYear() {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    struct tm *timeinfo = localtime(&time_t_now);
    char buffer[5];
    strftime(buffer, sizeof(buffer), "%Y", timeinfo);
    return std::string(buffer);
}

TEST_CASE("LibraryManager constructor initialises correctly") {
    LibraryManager lm;

    SECTION("Books is empty") {
        REQUIRE(lm.db.getBooks().empty());
    }

    SECTION("Users is empty") {
        REQUIRE(lm.db.getUsers().empty());
    }

    SECTION("Transactions is empty") {
        REQUIRE(lm.db.getTransactions().empty());
    }
}