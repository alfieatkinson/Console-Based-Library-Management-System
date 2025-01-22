#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "Database.hpp"
#include "Book.hpp"
#include "User.hpp"
#include "Transaction.hpp"
#include <iostream>

std::string getCurrentYear() {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    struct tm *timeinfo = localtime(&time_t_now);
    char buffer[5];
    strftime(buffer, sizeof(buffer), "%Y", timeinfo);
    return std::string(buffer);
}

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
        db.createUser("john_doe", "John", "Doe", "johndoe@email.com", "+441234567890", "password123");
        REQUIRE(db.getUsers().size() == 1);
        REQUIRE(db.getUsers()[0]->getUsername() == "john_doe");
        REQUIRE(db.getUserIDCounter() == 1);
    }

    SECTION("Create a transaction") {
        auto book = std::make_shared<Book>(1, "1984", "George Orwell", "9780451524935", 1949, true);
        auto user = std::make_shared<User>(1, "john_doe", "John", "Doe", "johndoe@email.com", "+441234567890", "password123");
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
        db.createUser("john_doe", "John", "Doe", "johndoe@email.com", "+441234567890", "password123");
        auto user = db.readUser(1);
        REQUIRE(user->getUsername() == "john_doe");
    }

    SECTION("Read a transaction") {
        auto book = std::make_shared<Book>(1, "1984", "George Orwell", "9780451524935", 1949, true);
        auto user = std::make_shared<User>(1, "john_doe", "John", "Doe", "johndoe@email.com", "+441234567890", "password123");
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
        db.createUser("john_doe", "John", "Doe", "johndoe@email.com", "+441234567890", "password123");
        db.updateUser(1, "username", "johndoe");
        REQUIRE(db.readUser(1)->getUsername() == "johndoe");
    }

    SECTION("Update a transaction") {
        auto book = std::make_shared<Book>(1, "1984", "George Orwell", "9780451524935", 1949, true);
        auto user = std::make_shared<User>(1, "john_doe", "John", "Doe", "johndoe@email.com", "+441234567890", "password123");
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
        db.createUser("john_doe", "John", "Doe", "johndoe@email.com", "+441234567890", "password123");
        db.deleteUser(1);
        REQUIRE(db.getUsers().empty());
    }

    SECTION("Delete a transaction") {
        auto book = std::make_shared<Book>(1, "1984", "George Orwell", "9780451524935", 1949, true);
        auto user = std::make_shared<User>(1, "john_doe", "John", "Doe", "johndoe@email.com", "+441234567890", "password123");
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
    db.createUser("john_doe", "John", "Doe", "johndoe@email.com", "+441234567890", "password123");
    db.createUser("jane_doe", "Jane", "Doe", "janedoe@email.com", "+449876543210", "password456");

    auto book1 = db.readBook(1);
    auto book2 = db.readBook(2);
    auto user1 = db.readUser(1);
    auto user2 = db.readUser(2);

    db.createTransaction("borrow", book1, user1);
    db.createTransaction("borrow", book2, user2);

    auto transaction1 = db.readTransaction(1);
    auto transaction2 = db.readTransaction(2);

    SECTION("Query books by title with approximate match") {
        // Test approximate match with one character difference
        auto result = db.queryBooks("1983");  
        REQUIRE(result.size() == 1);
        REQUIRE(result[0]->getTitle() == "1984");

        // Test with no approximate match (no such book)
        result = db.queryBooks("50 Shades of Grey");
        REQUIRE(result.empty());

        // Test with multiple results (if applicable, e.g., if the DB contains multiple titles that are similar)
        db.createBook("Brave New World 2", "Aldous Huxley", "9780060850525", 1945);
        result = db.queryBooks("Brave New World");  // Should match both "Brave New World" and "Brave New World: Revised"
        REQUIRE(result.size() == 2);
    }

    SECTION("Query books by author with approximate match") {
        // Test approximate match with one character difference
        auto result = db.queryBooks("George Orwel");  
        REQUIRE(result.size() == 1);
        REQUIRE(result[0]->getAuthor() == "George Orwell");

        // Test with no approximate match (no such author)
        result = db.queryBooks("J.K. Rowling");
        REQUIRE(result.empty());
    }

    SECTION("Query books by ISBN with approximate match") {
        // Test approximate match with one character difference
        auto result = db.queryBooks("9780451524936");  
        REQUIRE(result.size() == 1);
        REQUIRE(result[0]->getISBN() == "9780451524935");

        // Test with no approximate match (incorrect ISBN)
        result = db.queryBooks("9780000000000");
        REQUIRE(result.empty());
    }

    SECTION("Query users by forename with approximate match") {
        // Test approximate match with one character difference
        auto result = db.queryUsers("Hohn");
        REQUIRE(result.size() == 1);
        REQUIRE(result[0]->getForename() == "John");

        // Test with no approximate match (incorrect forename)
        result = db.queryUsers("Johnathan");
        REQUIRE(result.empty());
    }

    SECTION("Query users by surname with approximate match") {
        // Test approximate match with one character difference
        auto result = db.queryUsers("Doee");  
        REQUIRE(result.size() == 2);
        REQUIRE(result[0]->getSurname() == "Doe");

        // Test with no approximate match (incorrect surname)
        result = db.queryUsers("Smith");
        REQUIRE(result.empty());
    }

    SECTION("Query users by username with approximate match") {
        // Test approximate match with one character difference
        auto result = db.queryUsers("john_do");  
        REQUIRE(result.size() == 1);
        REQUIRE(result[0]->getUsername() == "john_doe");

        // Test with no approximate match (incorrect username)
        result = db.queryUsers("jonnysmith123");
        REQUIRE(result.empty());
    }

    SECTION("Edge case: Query with exact match") {
        // Test with exact match (no approximation)
        auto result = db.queryBooks("1984");  
        REQUIRE(result.size() == 1);
        REQUIRE(result[0]->getTitle() == "1984");

        // Exact match should always return one result, not multiple or empty
        result = db.queryBooks("Brave New World");
        REQUIRE(result.size() == 1);
        REQUIRE(result[0]->getTitle() == "Brave New World");
    }

    SECTION("Edge case: Query with no matches") {
        // Querying for something that doesn’t exist in the DB
        auto result = db.queryBooks("Nonexistent Book Title");  
        REQUIRE(result.empty());
    }

    // New sections for queryTransactionByBookID and queryTransactionByUserID

    SECTION("Query transactions by book ID") {
        // Query by book ID
        auto result = db.queryTransactionsByBookID(1);  
        REQUIRE(result.size() == 1);
        REQUIRE(result[0]->getBook()->getTitle() == "1984");
        REQUIRE(result[0]->getUser()->getUsername() == "john_doe");

        // Query by a non-existing book ID
        result = db.queryTransactionsByBookID(999);  
        REQUIRE(result.empty());
    }

    SECTION("Query transactions by user ID") {
        // Query by user ID
        auto result = db.queryTransactionsByUserID(1);  
        REQUIRE(result.size() == 1);
        REQUIRE(result[0]->getBook()->getTitle() == "1984");
        REQUIRE(result[0]->getUser()->getUsername() == "john_doe");

        // Query by a non-existing user ID
        result = db.queryTransactionsByUserID(999);  
        REQUIRE(result.empty());
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