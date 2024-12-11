#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <vector>
#include <memory>
#include <stdexcept>
#include <string>
#include "Book.hpp"
#include "User.hpp"
#include "Transaction.hpp"

class Database {
private:
    std::vector<std::shared_ptr<Book>> books;
    std::vector<std::shared_ptr<User>> users;
    std::vector<std::shared_ptr<Transaction>> transactions;
    int book_id_counter = 0;
    int user_id_counter = 0;
    int transaction_id_counter = 0;

public:
    // Constructor and Destructor
    Database() = default;
    ~Database() = default;

    // Getters
    const std::vector<std::shared_ptr<Book>>& getBooks() const;
    const std::vector<std::shared_ptr<User>>& getUsers() const;
    const std::vector<std::shared_ptr<Transaction>>& getTransactions() const;
    int getBookIDCounter() const;
    int getUserIDCounter() const;
    int getTransactionIDCounter() const;

    // Setters
    void setBookIDCounter(int new_counter);
    void setUserIDCounter(int new_counter);
    void setTransactionIDCounter(int new_counter);

    // Create operations
    void createBook(const std::string& title, const std::string& author, const std::string& isbn, int year_published);
    void createUser(const std::string& username, const std::string& forename, const std::string& surname, const std::string& email, const std::string& phone, const std::string& password);
    void createTransaction(const std::string& type, std::shared_ptr<Book> book, std::shared_ptr<User> user);

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

    // Query operations with approximate search
    std::vector<std::shared_ptr<Book>> queryBooks(const std::string& search_term);
    std::vector<std::shared_ptr<User>> queryUsers(const std::string& search_term);
    std::vector<std::shared_ptr<Transaction>> queryTransactions(const std::string& search_term);
};

#endif // DATABASE_HPP