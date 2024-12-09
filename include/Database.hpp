#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <vector>
#include <stdexcept>
#include "Book.hpp"
#include "User.hpp"
#include "Transaction.hpp"

class Database {
private:
    std::vector<Book> books;
    std::vector<User> users;
    std::vector<Transaction> transactions;
    int book_id_counter = 0;
    int user_id_counter = 0;
    int transaction_id_counter = 0;

public:
    // Constructor
    Database() = default;

    // Destructor
    ~Database() = default;

    // Getters
    const std::vector<Book>& getBooks() const;
    const std::vector<User>& getUsers() const;
    const std::vector<Transaction>& getTransactions() const;
    int getBookIDCounter() const;
    int getUserIDCounter() const;
    int getTransactionIDCounter() const;

    // Setters
    void setBookIDCounter(int new_counter);
    void setUserIDCounter(int new_counter);
    void setTransactionIDCounter(int new_counter);

    // Create operations
    void createBook(const std::string& title, const std::string& author, const std::string& isbn, int year);
    void createUser(const std::string& username, const std::string& forename, const std::string& surname, const std::string& email, const std::string& phone, const std::string& password);
    void createTransaction(const std::string& type, Book& book, User& user);

    // Read operations
    Book* readBook(int id);
    User* readUser(int id);
    Transaction* readTransaction(int id);

    // Update operations
    void updateBook(int id, const std::string& field, const std::string& value);
    void updateUser(int id, const std::string& field, const std::string& value);
    void updateTransaction(int id, const std::string& field, const std::string& value);

    // Delete operations
    void deleteBook(int id);
    void deleteUser(int id);
    void deleteTransaction(int id);
};

#endif // DATABASE_HPP