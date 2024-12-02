#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "Book.hpp"

TEST_CASE("Book Constructor initializes correctly") {
    Book book("The Great Gatsby", "F. Scott Fitzgerald", "9780743273565", 1925, true);

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
        REQUIRE(book.isAvailable() == true);
    }
}