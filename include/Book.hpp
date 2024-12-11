#ifndef BOOK_HPP
#define BOOK_HPP

#include <string>
#include <stdexcept>

class Book {
private:
    int book_id;
    std::string title;
    std::string author;
    std::string isbn;
    int year_published;
    bool available;

public:
    // Constructor
    Book(int book_id, const std::string& title, const std::string& author, const std::string& isbn, int year_published, bool available);

    // Destructor
    ~Book();

    // Overloaded operator
    bool operator==(const Book& other) const;

    // Getters
    int getID() const;
    std::string getTitle() const;
    std::string getAuthor() const;
    std::string getISBN() const;
    int getYearPublished() const;
    bool isAvailable() const;

    // Setters
    void setTitle(const std::string& new_title);
    void setAuthor(const std::string& new_author);
    void setISBN(const std::string& new_isbn);
    void setYearPublished(int new_year_published);
    void setIsAvailable(bool new_availability);

    // Methods for borrowing and returning the book
    bool borrowBook();
    void returnBook();

    // Method to get book info
    std::string getInfo() const;
};

#endif // BOOK_HPP