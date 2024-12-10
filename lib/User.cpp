#include "User.hpp"
#include <stdexcept>
#include <regex>

// Constructor
User::User(int user_id, const std::string& username, const std::string& forename, const std::string& surname, const std::string& email, const std::string& phone_number, const std::string& password)
    : user_id(user_id), username(username), forename(forename), surname(surname), email(email), phone_number(phone_number), password(password) {

    // Validate phone number length
    if (phone_number.length() != 11) {
        throw std::invalid_argument("Phone number must be 11 characters long.");
    }

    // Validate email format
    std::regex email_regex(R"(?:[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*|"(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21\x23-\x5b\x5d-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])*")@(?:(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?|\[(?:(?:(2(5[0-5]|[0-4][0-9])|1[0-9][0-9]|[1-9]?[0-9]))\.){3}(?:(2(5[0-5]|[0-4][0-9])|1[0-9][0-9]|[1-9]?[0-9])|[a-z0-9-]*[a-z0-9]:(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21-\x5a\x53-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])+)\])");
    if (!std::regex_match(email, email_regex)) {
        throw std::invalid_argument("Invalid email format.");
    }
}

// Destructor
User::~User() {
    // No dynamic memory allocation to clean up
}

// Getters
int User::getID() const { return user_id; }
std::string User::getUsername() const { return username; }
std::string User::getForename() const { return forename; }
std::string User::getSurname() const { return surname; }
std::string User::getEmail() const { return email; }
std::string User::getPhoneNumber() const { return phone_number; }
std::string User::getPassword() const { return password; }
std::vector<Book*> User::getBorrowedBooks() const { return borrowed_books; }

// Setters
void User::setUsername(const std::string& new_username) { username = new_username; }
void User::setForename(const std::string& new_forename) { forename = new_forename; }
void User::setSurname(const std::string& new_surname) { surname = new_surname; }
void User::setEmail(const std::string& new_email) { email = new_email; }
void User::setPhoneNumber(const std::string& new_phone_number) { phone_number = new_phone_number; }
void User::setPassword(const std::string& new_password) { password = new_password; }

// Methods for borrowing and returning books
bool User::borrowBook(Book* book) {
    if (book->isAvailable()) {
        borrowed_books.push_back(book);
        book->borrowBook();
        return true;
    }
    return false;
}

bool User::returnBook(Book* book) {
    auto it = std::find(borrowed_books.begin(), borrowed_books.end(), book);
    if (it != borrowed_books.end()) {
        borrowed_books.erase(it);
        book->returnBook();
        return true;
    }
    return false;
}

// Method to check the checkout status of a book
bool User::checkOutStatus(const Book* book) const {
    return std::find(borrowed_books.begin(), borrowed_books.end(), book) != borrowed_books.end();
}

// Method to get user info
std::string User::getUserInfo() const {
    return "User ID: " + std::to_string(user_id) + "\n" +
           "Username: " + username + "\n" +
           "Forename: " + forename + "\n" +
           "Surname: " + surname + "\n" +
           "Email: " + email + "\n" +
           "Phone Number: " + phone_number;
}