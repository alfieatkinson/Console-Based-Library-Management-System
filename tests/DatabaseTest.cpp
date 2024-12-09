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

TEST_CASE("Create operations") {
    Database db;

    SECTION("Create a book") {
        db.createBook("1984", "George Orwell", "9780451524935", 1949, true);
        REQUIRE(db.getBooks().size() == 1);
        REQUIRE(db.getBooks()[0]->getTitle() == "1984");
        REQUIRE(db.getBookIDCounter() == 1);
    }

    SECTION("Create a user") {
        db.createUser("john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");
        REQUIRE(db.getUsers().size() == 1);
        REQUIRE(db.getUsers()[0]->getUsername() == "john_doe");
        REQUIRE(db.getUserIDCounter() == 1);
    }

    SECTION("Create a transaction") {
        Book book(1, "1984", "George Orwell", "9780451524935", 1949, true);
        User user(1, "john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");
        db.createTransaction("borrow", &book, &user);
        REQUIRE(db.getTransactions().size() == 1);
        REQUIRE(db.getTransactions()[0]->getType() == "borrow");
        REQUIRE(db.getTransactionIDCounter() == 1);
    }
}