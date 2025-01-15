#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <vector>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <set>
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

    // Maps for quick searches
    std::unordered_map<std::string, std::vector<std::shared_ptr<Book>>> book_author_map;
    std::unordered_map<std::string, std::vector<std::shared_ptr<Book>>> book_title_map;
    std::unordered_map<std::string, std::vector<std::shared_ptr<Book>>> book_isbn_map;

    std::unordered_map<std::string, std::vector<std::shared_ptr<User>>> user_forename_map;
    std::unordered_map<std::string, std::vector<std::shared_ptr<User>>> user_surname_map;
    std::unordered_map<std::string, std::vector<std::shared_ptr<User>>> user_username_map;

    // Maps for transactions by book ID and user ID
    std::unordered_map<int, std::vector<std::shared_ptr<Transaction>>> transactions_by_book_id;
    std::unordered_map<int, std::vector<std::shared_ptr<Transaction>>> transactions_by_user_id;

public:
    // Constructor
    Database() = default;

    // Destructor
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

    // Save and load operations
    void save() const;
    void load() const; // Placeholder, no functionality yet

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
    std::vector<std::shared_ptr<Book>> queryBooks(const std::string& search_term, int threshold = 2);
    std::vector<std::shared_ptr<User>> queryUsers(const std::string& search_term, int threshold = 2);
    std::vector<std::shared_ptr<Transaction>> queryTransactionsByBookID(int id);
    std::vector<std::shared_ptr<Transaction>> queryTransactionsByUserID(int id);
};

#endif // DATABASE_HPP