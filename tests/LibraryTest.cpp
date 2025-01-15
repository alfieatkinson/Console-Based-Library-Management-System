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

TEST_CASE("Create operations") {
    LibraryManager lm;

    SECTION("Create a book") {
        lm.createBook("1984", "George Orwell", "9780451524935", 1949);
        REQUIRE(lm.db.getBooks().size() == 1);
        REQUIRE(lm.db.getBooks()[0]->getTitle() == "1984");
    }

    SECTION("Create a user") {
        lm.createUser("john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");
        REQUIRE(lm.db.getUsers().size() == 1);
        REQUIRE(lm.db.getUsers()[0]->getUsername() == "john_doe");
    }

    SECTION("Create a transaction") {
        auto book = std::make_shared<Book>(1, "1984", "George Orwell", "9780451524935", 1949, true);
        auto user = std::make_shared<User>(1, "john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");
        lm.createTransaction("borrow", book, user);
        REQUIRE(lm.db.getTransactions().size() == 1);
        REQUIRE(lm.db.getTransactions()[0]->getType() == "borrow");
    }
}

TEST_CASE("Read operations") {
    LibraryManager lm;

    SECTION("Read a book") {
        lm.createBook("1984", "George Orwell", "9780451524935", 1949);
        auto book = lm.readBook(1);
        REQUIRE(book->getTitle() == "1984");
    }

    SECTION("Read a user") {
        lm.createUser("john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");
        auto user = lm.readUser(1);
        REQUIRE(user->getUsername() == "john_doe");
    }

    SECTION("Read a transaction") {
        auto book = std::make_shared<Book>(1, "1984", "George Orwell", "9780451524935", 1949, true);
        auto user = std::make_shared<User>(1, "john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");
        lm.createTransaction("borrow", book, user);
        auto transaction = lm.readTransaction(1);
        REQUIRE(transaction->getType() == "borrow");
    }
}

TEST_CASE("Update operations") {
    LibraryManager lm;

    SECTION("Update a book") {
        lm.createBook("1984", "George Orwell", "9780451524935", 1949);
        lm.updateBook(1, "title", "Animal Farm");
        REQUIRE(lm.readBook(1)->getTitle() == "Animal Farm");
    }

    SECTION("Update a user") {
        lm.createUser("john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");
        lm.updateUser(1, "username", "johndoe");
        REQUIRE(lm.readUser(1)->getUsername() == "johndoe");
    }

    SECTION("Update a transaction") {
        auto book = std::make_shared<Book>(1, "1984", "George Orwell", "9780451524935", 1949, true);
        auto user = std::make_shared<User>(1, "john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");
        lm.createTransaction("borrow", book, user);
        lm.updateTransaction(1, "status", "completed");
        REQUIRE(lm.readTransaction(1)->getStatus() == "completed");
    }
}