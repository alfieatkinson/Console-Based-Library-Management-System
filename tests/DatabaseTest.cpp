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
        db.createBook("1984", "George Orwell", "9780451524935", 1949);
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
        auto book = std::make_shared<Book>(1, "1984", "George Orwell", "9780451524935", 1949, true);
        auto user = std::make_shared<User>(1, "john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");
        db.createTransaction("borrow", book, user);
        REQUIRE(db.getTransactions().size() == 1);
        REQUIRE(db.getTransactions()[0]->getType() == "borrow");
        REQUIRE(db.getTransactionIDCounter() == 1);
    }
}

TEST_CASE("Read operations") {
    Database db;

    SECTION("Read a book") {
        db.createBook("1984", "George Orwell", "9780451524935", 1949);
        auto book = db.readBook(1);
        REQUIRE(book->getTitle() == "1984");
    }

    SECTION("Read a user") {
        db.createUser("john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");
        auto user = db.readUser(1);
        REQUIRE(user->getUsername() == "john_doe");
    }

    SECTION("Read a transaction") {
        auto book = std::make_shared<Book>(1, "1984", "George Orwell", "9780451524935", 1949, true);
        auto user = std::make_shared<User>(1, "john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");
        db.createTransaction("borrow", book, user);
        auto transaction = db.readTransaction(1);
        REQUIRE(transaction->getType() == "borrow");
    }
}

TEST_CASE("Update operations") {
    Database db;

    SECTION("Update a book") {
        db.createBook("1984", "George Orwell", "9780451524935", 1949);
        db.updateBook(1, "title", "Animal Farm");
        REQUIRE(db.readBook(1)->getTitle() == "Animal Farm");
    }

    SECTION("Update a user") {
        db.createUser("john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");
        db.updateUser(1, "username", "johndoe");
        REQUIRE(db.readUser(1)->getUsername() == "johndoe");
    }

    SECTION("Update a transaction") {
        auto book = std::make_shared<Book>(1, "1984", "George Orwell", "9780451524935", 1949, true);
        auto user = std::make_shared<User>(1, "john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");
        db.createTransaction("borrow", book, user);
        db.updateTransaction(1, "status", "completed");
        REQUIRE(db.readTransaction(1)->getStatus() == "completed");
    }
}

TEST_CASE("Delete operations") {
    Database db;

    SECTION("Delete a book") {
        db.createBook("1984", "George Orwell", "9780451524935", 1949);
        db.deleteBook(1);
        REQUIRE(db.getBooks().empty());
    }

    SECTION("Delete a user") {
        db.createUser("john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");
        db.deleteUser(1);
        REQUIRE(db.getUsers().empty());
    }

    SECTION("Delete a transaction") {
        auto book = std::make_shared<Book>(1, "1984", "George Orwell", "9780451524935", 1949, true);
        auto user = std::make_shared<User>(1, "john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");
        db.createTransaction("borrow", book, user);
        db.deleteTransaction(1);
        REQUIRE(db.getTransactions().empty());
    }
}

TEST_CASE("Invalid operations") {
    Database db;

    SECTION("Read an invalid book") {
        REQUIRE_THROWS_AS(db.readBook(1), std::invalid_argument);
    }

    SECTION("Read an invalid user") {
        REQUIRE_THROWS_AS(db.readUser(1), std::invalid_argument);
    }

    SECTION("Read an invalid transaction") {
        REQUIRE_THROWS_AS(db.readTransaction(1), std::invalid_argument);
    }

    SECTION("Update an invalid book") {
        REQUIRE_THROWS_AS(db.updateBook(1, "title", "Animal Farm"), std::invalid_argument);
    }

    SECTION("Update an invalid user") {
        REQUIRE_THROWS_AS(db.updateUser(1, "username", "johndoe"), std::invalid_argument);
    }

    SECTION("Update an invalid transaction") {
        REQUIRE_THROWS_AS(db.updateTransaction(1, "status", "completed"), std::invalid_argument);
    }

    SECTION("Delete an invalid book") {
        REQUIRE_THROWS_AS(db.deleteBook(1), std::invalid_argument);
    }

    SECTION("Delete an invalid user") {
        REQUIRE_THROWS_AS(db.deleteUser(1), std::invalid_argument);
    }

    SECTION("Delete an invalid transaction") {
        REQUIRE_THROWS_AS(db.deleteTransaction(1), std::invalid_argument);
    }
}