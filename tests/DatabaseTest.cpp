#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "Database.hpp"
#include "Book.hpp"
#include "User.hpp"
#include "Transaction.hpp"

TEST_CASE("Database constructor initialises correctly") {
    Database db;

    SECTION("Books is empty") {
        REQUIRE(db.getBooks().empty());
    }

    SECTION("Book ID counter is 0") {
        REQUIRE(db.getBookIDCounter() == 0);
    }

    SECTION("Users is empty") {
        REQUIRE(db.getUsers().empty());
    }

    SECTION("User ID counter is 0") {
        REQUIRE(db.getUserIDCounter() == 0);
    }

    SECTION("Transactions is empty") {
        REQUIRE(db.getTransactions().empty());
    }

    SECTION("Transaction ID counter is 0") {
        REQUIRE(db.getTransactionIDCounter() == 0);
    }
}