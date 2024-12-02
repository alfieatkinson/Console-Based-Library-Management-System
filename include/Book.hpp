#ifndef BOOK_HPP
#define BOOK_HPP

#include <string>
#include <stdexcept>

class Book {
    private:
        std::string title;
        std::string author;
        std::string isbn;
        int yearPublished;
        bool available;

    public:
        // Constructor
        Book(const std::string& title, const std::string& author, const std::string& isbn, int yearPublished, bool available);

        // Getters
        std::string getTitle() const;
        std::string getAuthor() const;
        std::string getISBN() const;
        int getYearPublished() const;
        bool isAvailable() const;

        // Setters
        void setTitle(const std::string& newTitle);
        void setAuthor(const std::string& newAuthor);
        void setISBN(const std::string& newIsbn);
        void setIsAvailable(bool newAvailability);

        // Methods for borrowing and returning the book
        bool borrowBook();
        void returnBook();

        // Method to get book info
        std::string getBookInfo() const;
};

#endif // BOOK_HPP