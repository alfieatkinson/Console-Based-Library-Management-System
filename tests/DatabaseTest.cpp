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

TEST_CASE("Query operations with approximate search") {
    Database db;

    // Setup test data
    db.createBook("1984", "George Orwell", "9780451524935", 1949);
    db.createBook("Brave New World", "Aldous Huxley", "9780060850524", 1932);
    db.createUser("john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");
    db.createUser("jane_doe", "Jane", "Doe", "janedoe@email.com", "09876543210", "password456");

    auto book1 = db.readBook(1);
    auto book2 = db.readBook(2);
    auto user1 = db.readUser(1);
    auto user2 = db.readUser(2);

    auto transaction1 = std::make_shared<Transaction>(1, "borrow", book1, user1, "2024-12-11 10:00:00");
    auto transaction2 = std::make_shared<Transaction>(2, "return", book2, user2, "2024-12-12 15:30:00");
    db.createTransaction("borrow", book1, user1);
    db.createTransaction("return", book2, user2);

    SECTION("Query books by title with approximate match") {
        auto result = db.queryBooks("1983");  // One character difference from "1984"
        REQUIRE(result.size() == 1);
        REQUIRE(result[0]->getTitle() == "1984");
    }

    SECTION("Query books by author with approximate match") {
        auto result = db.queryBooks("George Orwel");  // One character difference from "George Orwell"
        REQUIRE(result.size() == 1);
        REQUIRE(result[0]->getAuthor() == "George Orwell");
    }

    SECTION("Query books by ISBN with approximate match") {
        auto result = db.queryBooks("9780451524936");  // One character difference from "9780451524935"
        REQUIRE(result.size() == 1);
        REQUIRE(result[0]->getISBN() == "9780451524935");
    }

    SECTION("Query users by username with approximate match") {
        auto result = db.queryUsers("john_do");  // One character difference from "john_doe"
        REQUIRE(result.size() == 1);
        REQUIRE(result[0]->getUsername() == "john_doe");
    }

    SECTION("Query users by email with approximate match") {
        auto result = db.queryUsers("johndoe@emal.com");  // One character difference from "johndoe@email.com"
        REQUIRE(result.size() == 1);
        REQUIRE(result[0]->getEmail() == "johndoe@email.com");
    }

    SECTION("Query transactions by status with approximate match") {
        auto result = db.queryTransactions("borro");  // One character difference from "borrow"
        REQUIRE(result.size() == 1);
        REQUIRE(result[0]->getStatus() == "borrow");
    }

    SECTION("Query transactions by datetime with approximate match") {
        auto result = db.queryTransactions("2024-12-11 10:00:01");  // One character difference from "2024-12-11 10:00:00"
        REQUIRE(result.size() == 1);
        REQUIRE(result[0]->getDatetime() == "2024-12-11 10:00:00");
    }

    SECTION("Query transactions by book title with approximate match") {
        auto result = db.queryTransactions("1983");  // One character difference from "1984"
        REQUIRE(result.size() == 1);
        REQUIRE(result[0]->getBook()->getTitle() == "1984");
    }

    SECTION("Query transactions by user username with approximate match") {
        auto result = db.queryTransactions("john_do");  // One character difference from "john_doe"
        REQUIRE(result.size() == 1);
        REQUIRE(result[0]->getUser()->getUsername() == "john_doe");
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