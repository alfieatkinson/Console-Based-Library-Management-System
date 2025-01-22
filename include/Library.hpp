#ifndef LIBRARY_HPP
#define LIBRARY_HPP

#include <vector>
#include <memory>
#include <stdexcept>
#include <string>
#include <mutex>
#include <lock_guard>
#include "Database.hpp"

class LibraryManager {
private:
    Database db;
    std::mutex mtx;

public:
    // Constructor
    LibraryManager();

    // Destructor
    ~LibraryManager();

    // Getters
    Database& getDatabase();

    // Methods for borrowing and returning books
    void borrowBook(int book_id, int user_id);
    void returnBook(int book_id, int user_id);

    // Create operations
    void createBook(const std::vector<std::string>& book_info);
    void createUser(const std::vector<std::string>& user_info);

    // Read operations
    std::shared_ptr<Book> readBook(int id);
    std::shared_ptr<User> readUser(int id);
    std::shared_ptr<Transaction> readTransaction(int id);

    // Update operations
    void updateBook(int id, const std::string& field, const std::string& value);
    void updateUser(int id, const std::string& field, const std::string& value);
    void updateTransaction(int id, const std::string& field, const std::string& value);

    // Delete operations
    void deleteBook(int id);
    void deleteUser(int id);
    void deleteTransaction(int id);

    // Methods for searching
    std::vector<std::_shared_ptr<Book>> queryBooks(const std::string& search_term);
    std::vector<std::_shared_ptr<User>> queryUsers(const std::string& search_term);
    std::vector<std::_shared_ptr<Transaction>> queryTransactionsByBookID(int id);
    std::vector<std::_shared_ptr<Transaction>> queryTransactionsByUserID(int id);

    // User authentication methods
    std::_shared_ptr<User> authenticateUser(const std::string& username, const std::string& password);
    bool authenticateAdmin(const std::string& password);

    // Database persistence methods
    void saveDatabase();
    void loadDatabase();
};

#endif // LIBRARY_HPP
