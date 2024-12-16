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
    LibraryManager() = default;

    // Destructor
    LibraryManager() = default;

    // Getters
    const Database& getDatabase() const;

    // Setters
    void setDatabase(const Database& new_db);

    // Methods for borrowing and returning books
    void borrowBook(int book_id, int user_id);
    void returnBook(int book_id, int user_id);

    // Methods for searching books
    std::vector<std::shared_ptr<Book>> queryBooks(const std::string& search_term);
    std::vector<std::shared_ptr<User>> queryUsers(const std::string& search_term);
    std::vector<std::shared_ptr<Transaction>> queryTransactionsByBookID(int id);
    std::vector<std::shared_ptr<Transaction>> queryTransactionsByUserID(int id);
};

#endif // LIBRARY_HPP