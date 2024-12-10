#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "User.hpp"
#include "Book.hpp"

TEST_CASE("User Constructor initialises correctly") {
    User user(1, "john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");

    SECTION("User ID is correct") {
        REQUIRE(user.getID() == 1);
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
    Book book(1, "1984", "George Orwell", "9780451524935", 1949, true);
    User user(1, "john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");

    SECTION("User can borrow an available book") {
        user.borrowBook(&book);
        REQUIRE(user.checkOutStatus(&book));  // Should be true
    }

    SECTION("User cannot borrow an unavailable book") {
        book.borrowBook();  // Mark the book as unavailable
        bool result = user.borrowBook(&book);  // Trying to borrow again
        REQUIRE_FALSE(result);  // Should fail to borrow again
    }

    SECTION("User's borrowed_books contains the borrowed book") {
        user.borrowBook(&book);
        REQUIRE(user.getBorrowedBooks().size() == 1);
        REQUIRE(user.getBorrowedBooks()[0] == &book);
    }
}

TEST_CASE("User can return books") {
    Book book(1, "1984", "George Orwell", "9780451524935", 1949, true);
    User user(1, "john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");

    SECTION("User can return a borrowed book") {
        user.borrowBook(&book);
        user.returnBook(&book);
        REQUIRE_FALSE(user.checkOutStatus(&book));  // Should be false
    }

    SECTION("User cannot return a book they haven't borrowed") {
        bool result = user.returnBook(&book);
        REQUIRE_FALSE(result);  // Should fail to return
    }

    SECTION("User's borrowed_books does not contain the returned book") {
        user.borrowBook(&book);
        user.returnBook(&book);
        REQUIRE(user.getBorrowedBooks().empty());
    }
}

TEST_CASE("User Information Retrieval") {
    User user(1, "john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");

    SECTION("Correct user info returned") {
        std::string expectedInfo = 
            "User ID: 1\n"
            "Username: john_doe\n"
            "Forename: John\n"
            "Surname: Doe\n"
            "Email: johndoe@email.com\n"
            "Phone Number: 01234567890";

        REQUIRE(user.getUserInfo() == expectedInfo);
    }
}

TEST_CASE("Setting user attributes using setters") {
    User user(1, "john_doe", "John", "Doe", "johndoe@email.com", "01234567890", "password123");

    user.setUsername("new_username");
    user.setForename("Jane");
    user.setSurname("Smith");
    user.setEmail("newemail@email.com");
    user.setPhoneNumber("09876543210");
    user.setPassword("newpassword123");

    SECTION("Username updated correctly") {
        REQUIRE(user.getUsername() == "new_username");
    }

    SECTION("Forename updated correctly") {
        REQUIRE(user.getForename() == "Jane");
    }

    SECTION("Surname updated correctly") {
        REQUIRE(user.getSurname() == "Smith");
    }

    SECTION("Email updated correctly") {
        REQUIRE(user.getEmail() == "newemail@email.com");
    }

    SECTION("Phone number updated correctly") {
        REQUIRE(user.getPhoneNumber() == "09876543210");
    }

    SECTION("Password updated correctly") {
        REQUIRE(user.getPassword() == "newpassword123");
    }
}

TEST_CASE("User constructor handles invalid phone number length") {
    REQUIRE_THROWS_AS(User(1, "john_doe", "John", "Doe", "johndoe@email.com", "12345", "password123"), std::invalid_argument);  // Invalid phone number length
}

TEST_CASE("User constructor handles invalid email format") {
    REQUIRE_THROWS_AS(User(1, "john_doe", "John", "Doe", "invalidemail.com", "01234567890", "password123"), std::invalid_argument);  // Missing '@'
    REQUIRE_THROWS_AS(User(1, "john_doe", "John", "Doe", "user@domain", "01234567890", "password123"), std::invalid_argument);  // Missing domain
    REQUIRE_THROWS_AS(User(1, "john_doe", "John", "Doe", "user@domain@domain.com", "01234567890", "password123"), std::invalid_argument);  // Extra '@'
}