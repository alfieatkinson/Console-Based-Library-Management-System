#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "User.hpp"
#include "Book.hpp"

TEST_CASE("User Constructor initialises correctly") {
    User user(1, "john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");

    SECTION("User ID is correct") {
        REQUIRE(user.getUserID() == 1);
    }

    SECTION("Username is correct") {
        REQUIRE(user.getUsername() == "john_doe");
    }

    SECTION("Forename is correct") {
        REQUIRE(user.getForename() == "John");
    }

    SECTION("Surname is correct") {
        REQUIRE(user.getSurname() == "Doe");
    }

    SECTION("Email is correct") {
        REQUIRE(user.getEmail() == "johndoe@email.com");
    }

    SECTION("Phone number is correct") {
        REQUIRE(user.getPhoneNumber() == "01234567890");
    }

    SECTION("Password is correct") {
        REQUIRE(user.getPassword() == "password123");
    }

    SECTION("Borrowed books array is empty initially") {
        REQUIRE(user.getBorrowedBooks().empty());
    }
}

TEST_CASE("User can borrow books") {
    Book book("1984", "George Orwell", "9780451524935", 1949, true);
    User user(1, "john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");

    SECTION("User can borrow an available book") {
        user.borrowBook(book);
        REQUIRE(user.checkOutStatus(book));  // Should be true
    }

    SECTION("User cannot borrow an unavailable book") {
        book.borrowBook();  // Mark the book as unavailable
        bool result = user.borrowBook(book);  // Trying to borrow again
        REQUIRE_FALSE(result);  // Should fail to borrow again
    }

    SECTION("User's borrowed_books contains the borrowed book") {
        user.borrowBook(book);
        REQUIRE(user.getBorrowedBooks().size() == 1);
        REQUIRE(user.getBorrowedBooks()[0] == book);
    }
}

TEST_CASE("User can return books") {
    Book book("1984", "George Orwell", "9780451524935", 1949, true);
    User user(1, "john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");

    SECTION("User can return a borrowed book") {
        user.borrowBook(book);
        user.returnBook(book);
        REQUIRE_FALSE(user.checkOutStatus(book));  // Should be false
    }

    SECTION("User cannot return a book they haven't borrowed") {
        bool result = user.returnBook(book);
        REQUIRE_FALSE(result);  // Should fail to return
    }

    SECTION("User's borrowed_books does not contain the returned book") {
        user.borrowBook(book);
        user.returnBook(book);
        REQUIRE(user.getBorrowedBooks().empty());
    }
}

