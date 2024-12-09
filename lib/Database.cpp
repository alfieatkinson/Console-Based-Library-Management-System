#include "Database.hpp"
#include "Book.hpp"
#include "User.hpp"
#include "Transaction.hpp"
#include <algorithm>
#include <stdexcept>

// Getters
const std::vector<Book>& Database::getBooks() const { return books; }
const std::vector<User>& Database::getUsers() const { return users; }
const std::vector<Transaction>& Database::getTransactions() const { return transactions; }
int Database::getBookIDCounter() const { return book_id_counter; }
int Database::getUserIDCounter() const { return user_id_counter; }
int Database::getTransactionIDCounter() const { return transaction_id_counter; }

// Setters
void Database::setBookIDCounter(int new_counter) { book_id_counter = new_counter; }
void Database::setUserIDCounter(int new_counter) { user_id_counter = new_counter; }
void Database::setTransactionIDCounter(int new_counter) { transaction_id_counter = new_counter; }

// Helper function for searching by ID (for Book, User, Transaction)
template <typename T>
T* findByID(std::vector<T>& vec, int id) {
    auto it = std::lower_bound(vec.begin(), vec.end(), id, 
        [](const T& obj, int id) { return obj.getID() < id; });

    if (it != vec.end() && it->getID() == id) {
        return &(*it);  // Return pointer to found object
    }

    throw std::invalid_argument("Object not found");
}

// Create operations
void Database::createBook(const std::string& title, const std::string& author, const std::string& isbn, int year) {
    books.push_back(Book(++book_id_counter, title, author, isbn, year, true));
}

void Database::createUser(const std::string& username, const std::string& forename, const std::string& surname, const std::string& email, const std::string& phone, const std::string& password) {
    users.push_back(User(++user_id_counter, username, forename, surname, email, phone, password));
}

void Database::createTransaction(const std::string& type, Book& book, User& user) {
    transactions.push_back(Transaction(++transaction_id_counter, type, &book, &user));
}

// Read operations
Book* Database::readBook(int id) { return findByID(books, id); }
User* Database::readUser(int id) { return findByID(users, id); }
Transaction* Database::readTransaction(int id) { return findByID(transactions, id); }

// Update operations
void Database::updateBook(int id, const std::string& field, const std::string& value) {
    Book* book = findByID(books, id);
    if (field == "title") {
        book->setTitle(value);
    } else if (field == "author") {
        book->setAuthor(value);
    } else if (field == "isbn") {
        book->setISBN(value);
    } else if (field == "year_published") {
        book->setYearPublished(std::stoi(value));
    } else if (field == "availabile") {
        book->setIsAvailable(value == "true");
    } else {
        throw std::invalid_argument("Invalid field");
    }
}

void Database::updateUser(int id, const std::string& field, const std::string& value) {
    User* user = findByID(users, id);
    if (field == "username") {
        user->setUsername(value);
    } else if (field == "forename") {
        user->setForename(value);
    } else if (field == "surname") {
        user->setSurname(value);
    } else if (field == "email") {
        user->setEmail(value);
    } else if (field == "phone_number") {
        user->setPhoneNumber(value);
    } else if (field == "password") {
        user->setPassword(value);
    } else {
        throw std::invalid_argument("Invalid field");
    }
}

void Database::updateTransaction(int id, const std::string& field, const std::string& value) {
    Transaction* transaction = findByID(transactions, id);
    if (field == "status") {
        transaction->setStatus(value);
    } else if (field == "datetime") {
        transaction->setDatetime(value);
    } else {
        throw std::invalid_argument("Invalid field");
    }
}