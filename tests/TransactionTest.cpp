#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "Transaction.hpp"
#include "Book.hpp"
#include "User.hpp"
#include <memory> // For smart pointers

TEST_CASE("Transaction constructor initialises correctly") {
    auto book = std::make_shared<Book>(1, "1984", "George Orwell", "9780451524935", 1949, true);
    auto user = std::make_shared<User>(1, "john_doe", "John", "Doe", "johndoe@email.com", "+441234567890", "password123");
    Transaction transaction(1, "borrow", book, user);

    SECTION("Transaction ID is correct") {
        REQUIRE(transaction.getID() == 1);
    }

    SECTION("Type is correct") {
        REQUIRE(transaction.getType() == "borrow");
    }

    SECTION("Status is correct") {
        REQUIRE(transaction.getStatus() == "open");
    }

    SECTION("Book is correct") {
        REQUIRE(transaction.getBook() == book);
    }

    SECTION("User is correct") {
        REQUIRE(transaction.getUser() == user);
    }

    SECTION("Datetime is initially empty") {
        REQUIRE(transaction.getDatetime().empty());  // Datetime should be empty initially
    }
}

TEST_CASE("Execute borrow transaction") {
    auto book = std::make_shared<Book>(1, "1984", "George Orwell", "9780451524935", 1949, true);
    auto user = std::make_shared<User>(1, "john_doe", "John", "Doe", "johndoe@email.com", "+441234567890", "password123");
    Transaction transaction(1, "borrow", book, user);

    SECTION("Transaction status changes to completed after execution") {
        transaction.execute();
        REQUIRE(transaction.getStatus() == "completed");
    }

    SECTION("Book becomes unavailable after execution") {
        transaction.execute();
        REQUIRE_FALSE(book->isAvailable());
    }

    SECTION("User's borrowed_books contains the borrowed book after execution") {
        transaction.execute();
        REQUIRE(user->getBorrowedBooks().size() == 1);
        REQUIRE(user->getBorrowedBooks()[0] == book);
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

TEST_CASE("Execute return transaction") {
    auto book = std::make_shared<Book>(1, "1984", "George Orwell", "9780451524935", 1949, true);
    auto user = std::make_shared<User>(1, "john_doe", "John", "Doe", "johndoe@email.com", "+441234567890", "password123");
    user->borrowBook(book);  // Borrow the book first
    Transaction transaction(1, "return", book, user);

    SECTION("Transaction status changes to completed after execution") {
        transaction.execute();
        REQUIRE(transaction.getStatus() == "completed");
    }

    SECTION("Book becomes available after execution") {
        transaction.execute();
        REQUIRE(book->isAvailable());
    }

    SECTION("User's borrowed_books does not contain the returned book after execution") {
        transaction.execute();
        REQUIRE(user->getBorrowedBooks().empty());
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

    SECTION("Cannot return a book not in borrowed_books") {
        auto book2 = std::make_shared<Book>(2, "Pride and Prejudice", "Jane Austen", "9781503290563", 1813, true);
        Transaction transaction2(2, "return", book2, user);

        REQUIRE_THROWS_AS(transaction2.execute(), std::invalid_argument);  // Try to return a book not borrowed
    }
}

TEST_CASE("Cancel transaction") {
    auto book = std::make_shared<Book>(1, "1984", "George Orwell", "9780451524935", 1949, true);
    auto user = std::make_shared<User>(1, "john_doe", "John", "Doe", "johndoe@email.com", "+441234567890", "password123");
    Transaction transaction(1, "borrow", book, user);

    SECTION("Transaction status changes to cancelled after execution") {
        transaction.cancel();
        REQUIRE(transaction.getStatus() == "cancelled");
    }

    SECTION("Book remains available after cancellation") {
        transaction.cancel();
        REQUIRE(book->isAvailable());
    }

    SECTION("User's borrowed_books does not contain the book after cancellation") {
        transaction.cancel();
        REQUIRE(user->getBorrowedBooks().empty());
    }

    SECTION("Transaction datetime is set after cancellation") {
        transaction.cancel();
        REQUIRE_FALSE(transaction.getDatetime().empty());
    }

    SECTION("Cannot cancel a completed transaction") {
        transaction.execute();  // Execute the transaction first
        REQUIRE_THROWS_AS(transaction.cancel(), std::logic_error);  // Try to cancel
        REQUIRE(transaction.getStatus() == "completed");  // Status should remain completed
    }

    SECTION("Cannot cancel a cancelled transaction") {
        transaction.cancel();  // Cancel the transaction first
        REQUIRE_THROWS_AS(transaction.cancel(), std::logic_error);  // Try to cancel again
        REQUIRE(transaction.getStatus() == "cancelled");  // Status should remain cancelled
    }
}

TEST_CASE("Get Transaction Information") {
    auto book = std::make_shared<Book>(1, "1984", "George Orwell", "9780451524935", 1949, true);
    auto user = std::make_shared<User>(1, "john_doe", "John", "Doe", "johndoe@email.com", "+441234567890", "password123");
    Transaction transaction(1, "borrow", book, user);
    transaction.execute();  // Execute the transaction

    SECTION("Correct transaction info returned with newlines") {
        std::string expectedInfo = 
            "Transaction ID: 1\n"
            "Type: borrow\n"
            "Status: completed\n"
            "Book: 1984 by George Orwell\n"
            "User: John Doe\n"
            "Datetime: " + transaction.getDatetime();

        REQUIRE(transaction.getInfo() == expectedInfo);
    }
}

TEST_CASE("Setting transaction attributes using setters") {
    auto book = std::make_shared<Book>(1, "1984", "George Orwell", "9780451524935", 1949, true);
    auto user = std::make_shared<User>(1, "john_doe", "John", "Doe", "johndoe@email.com", "+441234567890", "password123");
    Transaction transaction(1, "borrow", book, user);

    transaction.setStatus("cancelled");
    transaction.setDatetime("2021-10-01 12:00:00");

    SECTION("Status updated correctly") {
        REQUIRE(transaction.getStatus() == "cancelled");
    }

    SECTION("Datetime updated correctly") {
        REQUIRE(transaction.getDatetime() == "2021-10-01 12:00:00");
    }
}

TEST_CASE("Invalid Transaction Type") {
    auto book = std::make_shared<Book>(1, "1984", "George Orwell", "9780451524935", 1949, true);
    auto user = std::make_shared<User>(1, "john_doe", "John", "Doe", "johndoe@email.com", "+441234567890", "password123");

    SECTION("Invalid transaction type throws an exception") {
        REQUIRE_THROWS_AS(Transaction(1, "invalid", book, user), std::invalid_argument);
    }
}