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

// Getters
std::string Book::getTitle() const { return title; }
std::string Book::getAuthor() const { return author; }
std::string Book::getISBN() const { return isbn; }
int Book::getYearPublished() const { return yearPublished; }
bool Book::isAvailable() const { return available; }