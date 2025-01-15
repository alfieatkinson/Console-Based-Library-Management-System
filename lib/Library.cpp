#include "Library.hpp"
#include "Database.hpp"
#include "Book.hpp"
#include "User.hpp"
#include "Transaction.hpp"

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