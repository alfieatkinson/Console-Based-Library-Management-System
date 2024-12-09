#include "Transaction.hpp"
#include "Book.hpp"
#include "User.hpp"
#include <ctime>

// Constructor
Transaction::Transaction(int transaction_id, const std::string& type, Book* book, User* user)
    : transaction_id(transaction_id), type(type), status("open"), book(book), user(user) {
    if (type != "borrow" && type != "return") {
        throw std::invalid_argument("Invalid transaction type");
    }
    datetime = ""; // Initially empty until executed
}

// Destructor
Transaction::~Transaction() {
    // No dynamic memory allocation to clean up
}