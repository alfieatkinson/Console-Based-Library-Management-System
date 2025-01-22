#include "Library.hpp"
#include "Database.hpp"
#include "Book.hpp"
#include "User.hpp"
#include "Transaction.hpp"
#include <iostream>
#include <mutex>
#include <lock_guard>

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
Database& LibraryManager::getDatabase() {
    return db;
}

// Methods for borrowing and returning books
void LibraryManager::borrowBook(int book_id, int user_id) {
    std::lock_guard<std::mutex> lock(mtx);
    // Get the book and user from the database
    auto book = db.readBook(book_id);
    auto user = db.readUser(user_id);

    // Create a new transaction
    int id = db.createTransaction("borrow", book, user);

    // Execute the transaction
    auto transaction = db.readTransaction(id);
    transaction->execute();
}

void LibraryManager::returnBook(int book_id, int user_id) {
    std::lock_guard<std::mutex> lock(mtx);
    // Get the book and user from the database
    auto book = db.readBook(book_id);
    auto user = db.readUser(user_id);

    // Create a new transaction
    int id = db.createTransaction("return", book, user);

    // Execute the transaction
    auto transaction = db.readTransaction(id);
    transaction->execute();
}

// Create operations
void LibraryManager::createBook(const std::vector<std::string>& book_info) {
    std::lock_guard<std::mutex> lock(mtx);
    if (book_info.size() != 4) throw std::invalid_argument("Invalid number of arguments for creating a book.");

    // Create a new book
    db.createBook(book_info[0], book_info[1], book_info[2], std::stoi(book_info[3]));
}

void LibraryManager::createUser(const std::vector<std::string>& user_info) {
    std::lock_guard<std::mutex> lock(mtx);
    if (user_info.size() != 6) throw std::invalid_argument("Invalid number of arguments for creating a user.");

    // Create a new user
    db.createUser(user_info[0], user_info[1], user_info[2], user_info[3], user_info[4], user_info[5]);
}

// Read operations
std::shared_ptr<Book> LibraryManager::readBook(int id) { 
    std::lock_guard<std::mutex> lock(mtx);
    return db.readBook(id); 
}
std::shared_ptr<User> LibraryManager::readUser(int id) { 
    std::lock_guard<std::mutex> lock(mtx);
    return db.readUser(id); 
}
std::shared_ptr<Transaction> LibraryManager::readTransaction(int id) { 
    std::lock_guard<std::mutex> lock(mtx);
    return db.readTransaction(id); 
}

// Update operations
void LibraryManager::updateBook(int id, const std::string& field, const std::string& value) { 
    std::lock_guard<std::mutex> lock(mtx);
    db.updateBook(id, field, value); 
}
void LibraryManager::updateUser(int id, const std::string& field, const std::string& value) { 
    std::lock_guard<std::mutex> lock(mtx);
    db.updateUser(id, field, value); 
}
void LibraryManager::updateTransaction(int id, const std::string& field, const std::string& value) { 
    std::lock_guard<std::mutex> lock(mtx);
    db.updateTransaction(id, field, value); 
}

// Delete operations
void LibraryManager::deleteBook(int id) { 
    std::lock_guard<std::mutex> lock(mtx);
    db.deleteBook(id); 
}
void LibraryManager::deleteUser(int id) { 
    std::lock_guard<std::mutex> lock(mtx);
    db.deleteUser(id); 
}
void LibraryManager::deleteTransaction(int id) { 
    std::lock_guard<std::mutex> lock(mtx);
    db.deleteTransaction(id); 
}

// Methods for searching
std::vector<std::shared_ptr<Book>> LibraryManager::queryBooks(const std::string& search_term) { 
    std::lock_guard<std::mutex> lock(mtx);
    return db.queryBooks(search_term); 
}
std::vector<std::shared_ptr<User>> LibraryManager::queryUsers(const std::string& search_term) { 
    std::lock_guard<std::mutex> lock(mtx);
    return db.queryUsers(search_term); 
}
std::vector<std::shared_ptr<Transaction>> LibraryManager::queryTransactionsByBookID(int id) { 
    std::lock_guard<std::mutex> lock(mtx);
    return db.queryTransactionsByBookID(id); 
}
std::vector<std::shared_ptr<Transaction>> LibraryManager::queryTransactionsByUserID(int id) { 
    std::lock_guard<std::mutex> lock(mtx);
    return db.queryTransactionsByUserID(id); 
}

// User authentication methods
std::shared_ptr<User> LibraryManager::authenticateUser(const std::string& username, const std::string& password) {
    std::lock_guard<std::mutex> lock(mtx);
    return db.authenticateUser(username, password);
}

bool LibraryManager::authenticateAdmin(const std::string& password) {
    std::lock_guard<std::mutex> lock(mtx);
    return db.authenticateAdmin(password);
}

// Database persistence methods
void LibraryManager::saveDatabase() {
    std::lock_guard<std::mutex> lock(mtx);
    // Save the database to a file
    std::cout << "Saving database..." << std::endl;
    // TODO: Implement the save operation
}

void LibraryManager::loadDatabase() {
    std::lock_guard<std::mutex> lock(mtx);
    // Load the database from a file
    std::cout << "Loading database..." << std::endl;
    // TODO: Implement the load operation

    // Create some sample data for testing
    /*
    // Books
    db.createBook("The Great Gatsby", "F. Scott Fitzgerald", "9780743273565", 1925);
    db.createBook("To Kill a Mockingbird", "Harper Lee", "9780061120084", 1960);
    db.createBook("1984", "George Orwell", "9780451524935", 1949);
    db.createBook("Pride and Prejudice", "Jane Austen", "9780141439518", 1813);
    db.createBook("The Catcher in the Rye", "J.D. Salinger", "9780316769488", 1951);
    db.createBook("Animal Farm", "George Orwell", "9780451526342", 1945);
    db.createBook("Brave New World", "Aldous Huxley", "9780060850524", 1932);
    db.createBook("The Lord of the Rings", "J.R.R. Tolkien", "9780618640157", 1954);
    db.createBook("The Hobbit", "J.R.R. Tolkien", "9780618260300", 1937);
    db.createBook("The Da Vinci Code", "Dan Brown", "9780307474278", 2003);
    db.createBook("The Alchemist", "Paulo Coelho", "9780061122415", 1988);

    // Users
    db.createUser("aatkinson", "Alfie", "Atkinson", "aatkinson@email.com", "+447123456789", "password");
    db.createUser("bbrown", "Ben", "Brown", "bbrown@email.com", "+447123456790", "password123");
    db.createUser("charris", "Charlie", "Harris", "charris@email.com", "+447123456791", "123password");
    db.createUser("ddoe", "Dana", "Doe", "ddoe@email.com", "+447123456792", "doepassword");
    db.createUser("eevans", "Ella", "Evans", "eevans@email.com", "+447123456793", "evanspass");
    db.createUser("ffoster", "Finn", "Foster", "ffoster@email.com", "+447123456794", "fosterpass");

    // Transactions
    db.createTransaction("borrow", db.readBook(1), db.readUser(1));  // "The Great Gatsby" by Alfie Atkinson
    db.createTransaction("return", db.readBook(1), db.readUser(1));  // "The Great Gatsby" by Alfie Atkinson
    db.createTransaction("borrow", db.readBook(9), db.readUser(2));  // "The Hobbit" by Charlie Harris
    db.createTransaction("borrow", db.readBook(5), db.readUser(3));  // "The Catcher in the Rye" by Dana Doe
    db.createTransaction("return", db.readBook(5), db.readUser(3));  // "The Catcher in the Rye" by Dana Doe
    db.createTransaction("borrow", db.readBook(7), db.readUser(5));  // "Brave New World" by Finn Foster
    db.createTransaction("borrow", db.readBook(2), db.readUser(1));  // "To Kill a Mockingbird" by Alfie Atkinson
    db.createTransaction("return", db.readBook(2), db.readUser(1));  // "To Kill a Mockingbird" by Alfie Atkinson
    db.createTransaction("borrow", db.readBook(11), db.readUser(3));  // "The Alchemist" by Dana Doe
    db.createTransaction("return", db.readBook(11), db.readUser(3));  // "The Alchemist" by Dana Doe
    db.createTransaction("borrow", db.readBook(8), db.readUser(5));  // "The Lord of the Rings" by Finn Foster
    db.createTransaction("return", db.readBook(8), db.readUser(5));  // "The Lord of the Rings" by Finn Foster
    db.createTransaction("borrow", db.readBook(3), db.readUser(1)); // "1984" by Alfie Atkinson
    db.createTransaction("return", db.readBook(3), db.readUser(1)); // "1984" by Alfie Atkinson
    db.createTransaction("borrow", db.readBook(4), db.readUser(1)); // "Pride and Prejudice" by Alfie Atkinson
    db.createTransaction("return", db.readBook(4), db.readUser(1)); // "Pride and Prejudice" by Alfie Atkinson
    db.createTransaction("borrow", db.readBook(6), db.readUser(1)); // "Animal Farm" by Alfie Atkinson
    db.createTransaction("return", db.readBook(6), db.readUser(1)); // "Animal Farm" by Alfie Atkinson
    db.createTransaction("borrow", db.readBook(10), db.readUser(1)); // "The Da Vinci Code" by Alfie Atkinson
    db.createTransaction("return", db.readBook(10), db.readUser(1)); // "The Da Vinci Code" by Alfie Atkinson

    // Execute transactions
    for (const auto& transaction : db.getTransactions()) {
        try {
            transaction->execute(); // Execute the transaction
        } catch (const std::logic_error& e) {
            std::cout << "Conflict occurred during transaction execution: " << e.what() << std::endl;
        }
    }
    */
}
