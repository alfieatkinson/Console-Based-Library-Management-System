#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <string>
#include <stdexcept>
#include <ctime>

class Book;
class User;

class Transaction {
private:
    int transaction_id;
    std::string datetime;  // Initially empty until executed
    std::string type;      // Either "borrow" or "return"
    std::string status;                // "open", "completed", "cancelled"
    Book* book;
    User* user;

public:
    // Constructor
    Transaction(int transaction_id, const std::string& type, Book* book, User* user);

    // Destructor
    ~Transaction() {}

    // Overloaded operator
    bool operator==(const Transaction& other) const;

    // Getters
    int getID() const;
    std::string getType() const;
    std::string getStatus() const;
    Book* getBook() const;
    User* getUser() const;
    std::string getDatetime() const;

    // Setters
    void setStatus(const std::string& new_status);
    void setDatetime(const std::string& new_datetime);

    // Methods for executing and cancelling the transaction
    void execute();
    void cancel();

    // Method to get transaction info
    std::string getTransactionInfo() const;
};

#endif // TRANSACTION_HPP