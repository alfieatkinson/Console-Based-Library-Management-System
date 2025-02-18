#include "User.hpp"
#include <stdexcept>
#include <regex>

// Constructor
User::User(int user_id, const std::string& username, const std::string& forename, const std::string& surname, const std::string& email, const std::string& phone_number, const std::string& password)
    : user_id(user_id), username(username), forename(forename), surname(surname), email(email), phone_number(phone_number), password(password) {

    // Validate phone number length
    std::regex phone_number_regex("^[+]{1}(?:[0-9\\-\\(\\)\\/""\\.]\\s?){6,15}[0-9]{1}$");
    if (!std::regex_match(phone_number, phone_number_regex)) {
        throw std::invalid_argument("Invalid phone number format. Try including the country code.");
    }

    // Validate email format
    std::regex email_regex("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    if (!std::regex_match(email, email_regex)) throw std::invalid_argument("Invalid email format.");
}

// Destructor
User::~User() {
    // No dynamic memory allocation to clean up
}

// Overloaded operator
bool User::operator==(const User& other) const { return user_id == other.user_id; }

// Getters
int User::getID() const { return user_id; }
std::string User::getUsername() const { return username; }
std::string User::getForename() const { return forename; }
std::string User::getSurname() const { return surname; }
std::string User::getEmail() const { return email; }
std::string User::getPhoneNumber() const { return phone_number; }
std::string User::getPassword() const { return password; }
std::vector<std::shared_ptr<Book>> User::getBorrowedBooks() const { return borrowed_books; }

// Setters
void User::setUsername(const std::string& new_username) { username = new_username; }
void User::setForename(const std::string& new_forename) { forename = new_forename; }
void User::setSurname(const std::string& new_surname) { surname = new_surname; }
void User::setEmail(const std::string& new_email) {
    // Validate email format
    std::regex email_regex("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    if (!std::regex_match(new_email, email_regex)) throw std::invalid_argument("Invalid email format.");
    email = new_email;
}
void User::setPhoneNumber(const std::string& new_phone_number) {
    // Validate phone number length
    std::regex phone_number_regex("^[+]{1}(?:[0-9\\-\\(\\)\\/""\\.]\\s?){6,15}[0-9]{1}$");
    if (!std::regex_match(phone_number, phone_number_regex)) {
        throw std::invalid_argument("Invalid phone number format. Try including the country code.");
    }
    phone_number = new_phone_number;
}
void User::setPassword(const std::string& new_password) { password = new_password; }

// Methods for borrowing and returning books
bool User::borrowBook(std::shared_ptr<Book> book) {
    if (book->borrowBook()) {
        borrowed_books.push_back(book);
        return true;
    }
    return false;
}

bool User::returnBook(std::shared_ptr<Book> book) {
    auto it = std::find(borrowed_books.begin(), borrowed_books.end(), book);
    if (it != borrowed_books.end()) {
        borrowed_books.erase(it);
        book->returnBook();
        return true;
    }
    return false;
}

// Method to check the checkout status of a book
bool User::checkOutStatus(const std::shared_ptr<Book> book) const {
    return std::find(borrowed_books.begin(), borrowed_books.end(), book) != borrowed_books.end();
}

// Method to get user info
std::string User::getInfo() const {
    return "User ID: " + std::to_string(user_id) + "\n" +
           "Username: " + username + "\n" +
           "Forename: " + forename + "\n" +
           "Surname: " + surname + "\n" +
           "Email: " + email + "\n" +
           "Phone Number: " + phone_number;
}