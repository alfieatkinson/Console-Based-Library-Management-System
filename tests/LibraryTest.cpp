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

TEST_CASE("Delete operations") {
    LibraryManager lm;

    SECTION("Delete a book") {
        lm.createBook("1984", "George Orwell", "9780451524935", 1949);
        lm.deleteBook(1);
        REQUIRE(lm.getBooks().empty());
    }

    SECTION("Delete a user") {
        lm.createUser("john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");
        lm.deleteUser(1);
        REQUIRE(lm.getUsers().empty());
    }

    SECTION("Delete a transaction") {
        auto book = std::make_shared<Book>(1, "1984", "George Orwell", "9780451524935", 1949, true);
        auto user = std::make_shared<User>(1, "john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");
        lm.createTransaction("borrow", book, user);
        lm.deleteTransaction(1);
        REQUIRE(lm.getTransactions().empty());
    }
}

TEST_CASE("Borrow and Return Operations") {
    LibraryManager lm;

    SECTION("Borrow a book") {
        lm.createBook("1984", "George Orwell", "9780451524935", 1949);
        lm.createUser("john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");
        lm.borrowBook(1, 1);  // Book ID 1, User ID 1
        REQUIRE(lm.getTransactions().size() == 1);
        REQUIRE(lm.getTransactions()[0]->getType() == "borrow");
    }

    SECTION("Return a book") {
        lm.createBook("1984", "George Orwell", "9780451524935", 1949);
        lm.createUser("john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");
        lm.borrowBook(1, 1);  // Borrow the book
        lm.returnBook(1);     // Return the book
        REQUIRE(lm.getTransactions().size() == 1);
        REQUIRE(lm.getTransactions()[0]->getType() == "return");
        REQUIRE(lm.getTransactions()[0]->getStatus() == "completed");
    }
}

TEST_CASE("Query operations with approximate search") {
    LibraryManager lm;

    // Setup test data
    lm.createBook("1984", "George Orwell", "9780451524935", 1949);
    lm.createBook("Brave New World", "Aldous Huxley", "9780060850524", 1932);
    lm.createUser("john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");
    lm.createUser("jane_doe", "Jane", "Doe", "janedoe@email.com", "09876543210", "password456");

    auto book1 = lm.readBook(1);
    auto book2 = lm.readBook(2);
    auto user1 = lm.readUser(1);
    auto user2 = lm.readUser(2);

    lm.createTransaction("borrow", book1, user1);
    lm.createTransaction("borrow", book2, user2);

    SECTION("Query books by title with approximate match") {
        auto result = lm.queryBooks("1983");
        REQUIRE(result.size() == 1);
        REQUIRE(result[0]->getTitle() == "1984");

        result = lm.queryBooks("50 Shades of Grey");
        REQUIRE(result.empty());

        lm.createBook("Brave New World 2", "Aldous Huxley", "9780060850525", 1945);
        result = lm.queryBooks("Brave New World");
        REQUIRE(result.size() == 2);
    }

    SECTION("Query books by author with approximate match") {
        auto result = lm.queryBooks("George Orwel");
        REQUIRE(result.size() == 1);
        REQUIRE(result[0]->getAuthor() == "George Orwell");

        result = lm.queryBooks("J.K. Rowling");
        REQUIRE(result.empty());
    }

    SECTION("Query books by ISBN with approximate match") {
        auto result = lm.queryBooks("9780451524936");
        REQUIRE(result.size() == 1);
        REQUIRE(result[0]->getISBN() == "9780451524935");

        result = lm.queryBooks("9780000000000");
        REQUIRE(result.empty());
    }

    SECTION("Query users by forename with approximate match") {
        auto result = lm.queryUsers("Hohn");
        REQUIRE(result.size() == 1);
        REQUIRE(result[0]->getForename() == "John");

        result = lm.queryUsers("Johnathan");
        REQUIRE(result.empty());
    }

    SECTION("Query users by surname with approximate match") {
        auto result = lm.queryUsers("Doee");
        REQUIRE(result.size() == 2);
        REQUIRE(result[0]->getSurname() == "Doe");

        result = lm.queryUsers("Smith");
        REQUIRE(result.empty());
    }

    SECTION("Query users by username with approximate match") {
        auto result = lm.queryUsers("john_do");
        REQUIRE(result.size() == 1);
        REQUIRE(result[0]->getUsername() == "john_doe");

        result = lm.queryUsers("jonnysmith123");
        REQUIRE(result.empty());
    }

    SECTION("Edge case: Query with exact match") {
        auto result = lm.queryBooks("1984");
        REQUIRE(result.size() == 1);
        REQUIRE(result[0]->getTitle() == "1984");

        result = lm.queryBooks("Brave New World");
        REQUIRE(result.size() == 1);
        REQUIRE(result[0]->getTitle() == "Brave New World");
    }

    SECTION("Edge case: Query with no matches") {
        auto result = lm.queryBooks("Nonexistent Book Title");
        REQUIRE(result.empty());
    }

    SECTION("Query transactions by book ID") {
        auto result = lm.queryTransactionsByBookID(1);
        REQUIRE(result.size() == 1);
        REQUIRE(result[0]->getBook()->getTitle() == "1984");
        REQUIRE(result[0]->getUser()->getUsername() == "john_doe");

        result = lm.queryTransactionsByBookID(99);
        REQUIRE(result.empty());
    }

    SECTION("Query transactions by user ID") {
        auto result = lm.queryTransactionsByUserID(1);
        REQUIRE(result.size() == 1);
        REQUIRE(result[0]->getBook()->getTitle() == "1984");
        REQUIRE(result[0]->getUser()->getUsername() == "john_doe");

        result = lm.queryTransactionsByUserID(99);
        REQUIRE(result.empty());
    }
}