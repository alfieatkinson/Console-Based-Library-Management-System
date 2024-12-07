#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "Book.hpp"

TEST_CASE("Book Constructor initialises correctly") {
    Book book(1, "The Great Gatsby", "F. Scott Fitzgerald", "9780743273565", 1925, true);

    SECTION("Book ID is correct") {
        REQUIRE(book.getBookID() == 1);
    }

    SECTION("Title is correct") {
        REQUIRE(book.getTitle() == "The Great Gatsby");
    }
    
    SECTION("Author is correct") {
        REQUIRE(book.getAuthor() == "F. Scott Fitzgerald");
    }

    SECTION("ISBN is correct") {
        REQUIRE(book.getISBN() == "9780743273565");
    }

    SECTION("Year Published is correct") {
        REQUIRE(book.getYearPublished() == 1925);
    }

    SECTION("Availability is correct") {
        REQUIRE(book.isAvailable());
    }
}

TEST_CASE("Borrowing a book") {
    Book book(1, "1984", "George Orwell", "9780451524935", 1949, true);

    SECTION("Book is available before borrowing") {
        REQUIRE(book.isAvailable());
    }

    SECTION("Book becomes unavailable after borrowing") {
        book.borrowBook();
        REQUIRE_FALSE(book.isAvailable());
    }

    SECTION("Attempting to borrow an unavailable book") {
        book.borrowBook();
        bool result = book.borrowBook();  // Trying to borrow again while unavailable
        REQUIRE_FALSE(result);
    }
}

TEST_CASE("Returning a book") {
    Book book(1, "Moby Dick", "Herman Melville", "9780142437247", 1851, false);
    book.borrowBook();  // Borrow the book first

    SECTION("Book becomes available after returning") {
        book.returnBook();
        REQUIRE(book.isAvailable());
    }

    SECTION("Returning a book that is already available") {
        book.returnBook();  // Return the book twice
        book.returnBook();
        REQUIRE(book.isAvailable());
    }
}

TEST_CASE("Get Book Information") {
    Book book(1, "Pride and Prejudice", "Jane Austen", "9781503290563", 1813, true);

    SECTION("Correct book info returned with newlines") {
        std::string expectedInfo = 
            "Book ID: 1\n"
            "Title: Pride and Prejudice\n"
            "Author: Jane Austen\n"
            "ISBN: 9781503290563\n"
            "Year: 1813\n"
            "Available: true";

        REQUIRE(book.getBookInfo() == expectedInfo);
    }
}

TEST_CASE("Setting book attributes using setters") {
    Book book(1, "War and Peace", "Leo Tolstoy", "9780198800545", 1869, true);
    
    book.setTitle("War and Peace: Updated Edition");
    book.setAuthor("Leo Tolstoy (Edited by X)");
    book.setISBN("9780198800546");
    book.setIsAvailable(false);
    
    SECTION("Title updated correctly") {
        REQUIRE(book.getTitle() == "War and Peace: Updated Edition");
    }

    SECTION("Author updated correctly") {
        REQUIRE(book.getAuthor() == "Leo Tolstoy (Edited by X)");
    }

    SECTION("ISBN updated correctly") {
        REQUIRE(book.getISBN() == "9780198800546");
    }

    SECTION("Availability updated correctly") {
        REQUIRE_FALSE(book.isAvailable());
    }
}

TEST_CASE("Book constructor handles invalid ISBN length") {
    REQUIRE_THROWS_AS(Book(1, "Invalid Book", "Unknown Author", "123", 2024, true),std::invalid_argument);
}

TEST_CASE("Book state integrity") {
    Book book(1, "Test Book", "Author", "1234567890000", 2022, true);

    SECTION("Book cannot be borrowed twice if already borrowed") {
        book.borrowBook();
        REQUIRE_FALSE(book.borrowBook());  // Should fail to borrow again
    }

    SECTION("Returning a book multiple times doesn't affect state") {
        book.borrowBook();
        book.returnBook();  // Should become available
        book.returnBook();  // Should stay available
        REQUIRE(book.isAvailable());
    }
}