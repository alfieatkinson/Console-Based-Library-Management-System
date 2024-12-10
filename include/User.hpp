#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>
#include <stdexcept>
#include "Book.hpp"

class User {
private:
    int user_id;
    std::string username;
    std::string forename;
    std::string surname;
    std::string email;
    std::string phone_number;
    std::string password;
    std::vector<Book*> borrowed_books; // Vector of pointers to Book objects

public:
    // Constructor
    User(int user_id, const std::string& username, const std::string& forename, const std::string& surname, const std::string& email, const std::string& phone_number, const std::string& password);

    // Destructor
    ~User();

    // Overloaded operator
    bool operator==(const User& other) const;

    // Getters
    int getID() const;
    std::string getUsername() const;
    std::string getForename() const;
    std::string getSurname() const;
    std::string getEmail() const;
    std::string getPhoneNumber() const;
    std::string getPassword() const;
    std::vector<Book*> getBorrowedBooks() const;

    // Setters
    void setUsername(const std::string& new_username);
    void setForename(const std::string& new_forename);
    void setSurname(const std::string& new_surname);
    void setEmail(const std::string& new_email);
    void setPhoneNumber(const std::string& new_phone_umber);
    void setPassword(const std::string& new_password);

    // Methods for borrowing and returning books
    bool borrowBook(Book* book);
    bool returnBook(Book* book);

    // Method to check the checkout status of a book
    bool checkOutStatus(const Book* book) const;

    // Method to get user info
    std::string getInfo() const;
};

#endif // USER_HPP