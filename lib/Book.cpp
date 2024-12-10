#include "Book.hpp"
#include <stdexcept>
#include <string>

// Constructor
Book::Book(int book_id, const std::string& title, const std::string& author, const std::string& isbn, int year_published, bool available)
    : book_id(book_id), title(title), author(author), isbn(isbn), year_published(year_published), available(available) {
    if (isbn.length() != 13) {
        throw std::invalid_argument("ISBN must be 13 characters long.");
    }
}

// Destructor
Book::~Book() {
    // No dynamic memory allocation to clean up
}

// Overloaded operator
bool Book::operator==(const Book& other) const { return book_id == other.book_id; }

// Getters
int Book::getID() const { return book_id; }
std::string Book::getTitle() const { return title; }
std::string Book::getAuthor() const { return author; }
std::string Book::getISBN() const { return isbn; }
int Book::getYearPublished() const { return year_published; }
bool Book::isAvailable() const { return available; }

// Setters
void Book::setTitle(const std::string& new_title) { title = new_title; }
void Book::setAuthor(const std::string& new_author) { author = new_author; }
void Book::setISBN(const std::string& new_isbn) {
    if (new_isbn.length() != 13) {
        throw std::invalid_argument("ISBN must be 13 characters long");
    }
    isbn = new_isbn;
}
void Book::setYearPublished(int new_year_published) { year_published = new_year_published; }
void Book::setIsAvailable(bool new_availability) { available = new_availability; }

// Methods
bool Book::borrowBook() {
    if (isAvailable()) {
        setIsAvailable(false);
        return true;
    }
    return false;
}

void Book::returnBook() {
    setIsAvailable(true);
}

std::string Book::getBookInfo() const {
    return "Book ID: " + std::to_string(getID()) + "\n" +
           "Title: " + getTitle() + "\n" +
           "Author: " + getAuthor() + "\n" +
           "ISBN: " + getISBN() + "\n" +
           "Year: " + std::to_string(getYearPublished()) + "\n" +
           "Available: " + (isAvailable() ? "true" : "false");
}