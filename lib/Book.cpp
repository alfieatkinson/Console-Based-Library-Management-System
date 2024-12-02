#include "Book.hpp"
#include <stdexcept>
#include <string>

// Constructor
Book::Book(const std::string& title, const std::string& author, const std::string& isbn, int yearPublished, bool available)
    : title(title), author(author), isbn(isbn), yearPublished(yearPublished), available(available) {
    if (isbn.length() != 13) {
        throw std::invalid_argument("ISBN must be 13 characters long.");
    }
}

// Destructor
Book::~Book() {
    // No dynamic memory allocation to clean up
}