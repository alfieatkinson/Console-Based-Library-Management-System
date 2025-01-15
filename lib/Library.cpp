#include "Library.hpp"
#include "Database.hpp"
#include "Book.hpp"
#include "User.hpp"
#include "Transaction.hpp"
#include <iostream>

// Constructor
LibraryManager::LibraryManager() {
    // Load the database on construction
    loadDatabase();
}

// Destructor
LibraryManager::~LibraryManager() {
    // Save the database on destruction
    saveDatabase();
}

// Getters
const Database& LibraryManager::getDatabase() const {
    return db;
}

// Setters
void LibraryManager::setDatabase(const Database& new_db) {
    db = new_db;
}

// Methods for borrowing and returning books
void LibraryManager::borrowBook(int book_id, int user_id) {
    // Get the book and user from the database
    auto book = db.readBook(book_id);
    auto user = db.readUser(user_id);

    // Create a new transaction
    int id = db.createTransaction("borrow", book, user);

    // Check if the book is available
    if (user->borrowBook(book)) {
        // Execute the transaction
        auto transaction = db.readTransaction(id);
        transaction->execute();
    } else {
        throw std::invalid_argument("Book is not available to borrow.");
    }
}

void LibraryManager::returnBook(int book_id, int user_id) {
    // Get the book and user from the database
    auto book = db.readBook(book_id);
    auto user = db.readUser(user_id);

    // Create a new transaction
    int id = db.createTransaction("return", book, user);

    // Check if the user has borrowed the book
    if (user->returnBook(book)) {
        // Execute the transaction
        auto transaction = db.readTransaction(id);
        transaction->execute();
    } else {
        throw std::invalid_argument("User has not borrowed this book.");
    }
}

// Create operations
void LibraryManager::createBook(const std::vector<std::string>& book_info) {
    if (book_info.size() != 4) throw std::invalid_argument("Invalid number of arguments for creating a book.");

    // Create a new book
    db.createBook(book_info[0], book_info[1], book_info[2], std::stoi(book_info[3]));
}

void LibraryManager::createUser(const std::vector<std::string>& user_info) {
    if (user_info.size() != 6) throw std::invalid_argument("Invalid number of arguments for creating a user.");

    // Create a new user
    db.createUser(user_info[0], user_info[1], user_info[2], user_info[3], user_info[4], user_info[5]);
}

// Read operations
std::shared_ptr<Book> LibraryManager::readBook(int id) { return db.readBook(id); }
std::shared_ptr<User> LibraryManager::readUser(int id) { return db.readUser(id); }
std::shared_ptr<Transaction> LibraryManager::readTransaction(int id) { return db.readTransaction(id); }

// Update operations
void LibraryManager::updateBook(int id, const std::string& field, const std::string& value) { db.updateBook(id, field, value); }
void LibraryManager::updateUser(int id, const std::string& field, const std::string& value) { db.updateUser(id, field, value); }
void LibraryManager::updateTransaction(int id, const std::string& field, const std::string& value) { db.updateTransaction(id, field, value); }

// Delete operations
void LibraryManager::deleteBook(int id) { db.deleteBook(id); }
void LibraryManager::deleteUser(int id) { db.deleteUser(id); }
void LibraryManager::deleteTransaction(int id) { db.deleteTransaction(id); }

// Methods for searching
std::vector<std::shared_ptr<Book>> LibraryManager::queryBooks(const std::string& search_term) { return db.queryBooks(search_term); }
std::vector<std::shared_ptr<User>> LibraryManager::queryUsers(const std::string& search_term) { return db.queryUsers(search_term); }
std::vector<std::shared_ptr<Transaction>> LibraryManager::queryTransactionsByBookID(int id) { return db.queryTransactionsByBookID(id); }
std::vector<std::shared_ptr<Transaction>> LibraryManager::queryTransactionsByUserID(int id) { return db.queryTransactionsByUserID(id); }

// Database persistence methods
void LibraryManager::saveDatabase() const {
    // Save the database to a file
    std::cout << "Saving database..." << std::endl;
    // TODO: Implement the save operation
}

void LibraryManager::loadDatabase() const {
    // Load the database from a file
    std::cout << "Loading database..." << std::endl;
    // TODO: Implement the load operation
}