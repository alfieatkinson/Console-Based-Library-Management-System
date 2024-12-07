#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "Transaction.hpp"
#include "Book.hpp"
#include "User.hpp"

TEST_CASE("Transaction constructor initialises correctly") {
    Book book(1, "1984", "George Orwell", "9780451524935", 1949, true);
    User user(1, "john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");
    Transaction transaction(1, "borrow", &book, &user);

    SECTION("Transaction ID is correct") {
        REQUIRE(transaction.getTransactionID() == 1);
    }

    SECTION("Type is correct") {
        REQUIRE(transaction.getType() == "borrow");
    }

    SECTION("Status is correct") {
        REQUIRE(transaction.getStatus() == "open");
    }

    SECTION("Book is correct") {
        REQUIRE(transaction.getBook() == &book);
    }

    SECTION("User is correct") {
        REQUIRE(transaction.getUser() == &user);
    }

    SECTION("Datetime is initially empty") {
        REQUIRE(transaction.getDatetime().empty());  // Datetime should be empty initially
    }
}

TEST_CASE("Execute borrow transaction") {
    Book book(1, "1984", "George Orwell", "9780451524935", 1949, true);
    User user(1, "john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");
    Transaction transaction(1, "borrow", &book, &user);

    SECTION("Transaction status changes to completed after execution") {
        transaction.execute();
        REQUIRE(transaction.getStatus() == "completed");
    }

    SECTION("Book becomes unavailable after execution") {
        transaction.execute();
        REQUIRE_FALSE(book.isAvailable());
    }

    SECTION("User's borrowed_books contains the borrowed book after execution") {
        transaction.execute();
        REQUIRE(user.getBorrowedBooks().size() == 1);
        REQUIRE(user.getBorrowedBooks()[0] == &book);
    }

    SECTION("Transaction datetime is set after execution") {
        transaction.execute();
        REQUIRE_FALSE(transaction.getDatetime().empty());
    }

    SECTION("Cannot execute a completed transaction") {
        transaction.execute();  // Execute the transaction once
        REQUIRE_THROWS_AS(transaction.execute(), std::logic_error);  // Try to execute again
        REQUIRE(transaction.getStatus() == "completed");  // Status should remain completed
    }
}