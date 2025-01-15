#ifndef LIBRARY_HPP
#define LIBRARY_HPP

#include <vector>
#include <memory>
#include <stdexcept>
#include <string>
#include "Database.hpp"

class LibraryManager {
private:
    Database db;

public:
    // Constructor
    LibraryManager();

    // Destructor
    ~LibraryManager();

    // Getters
    const Database& getDatabase() const;

    // Setters
    void setDatabase(const Database& new_db);

    // Methods for borrowing and returning books
    void borrowBook(int book_id, int user_id);
    void returnBook(int book_id, int user_id);

    // CRUD Operations
    void createBook(const std::vector<std::string>& book_info);
    void createUser(const std::vector<std::string>& user_info);

    void readBook(int id);
    void readUser(int id);
    void readTransaction(int id);

    void updateBook(int id, const std::string& field, const std::string& value);
    void updateUser(int id, const std::string& field, const std::string& value);
    void updateTransaction(int id, const std::string& field, const std::string& value);

    void deleteBook(int id);
    void deleteUser(int id);
    void deleteTransaction(int id);

    // Methods for searching
    std::vector<std::shared_ptr<Book>> queryBooks(const std::string& search_term);
    std::vector<std::shared_ptr<User>> queryUsers(const std::string& search_term);
    std::vector<std::shared_ptr<Transaction>> queryTransactionsByBookID(int id);
    std::vector<std::shared_ptr<Transaction>> queryTransactionsByUserID(int id);

    // Database persistence methods
    void saveDatabase() const;
    void loadDatabase() const;
};

#endif // LIBRARY_HPP