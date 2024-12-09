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

// Getters
int Transaction::getTransactionID() const { return transaction_id; }
std::string Transaction::getType() const { return type; }
std::string Transaction::getStatus() const { return status; }
Book* Transaction::getBook() const { return book; }
User* Transaction::getUser() const { return user; }
std::string Transaction::getDatetime() const { return datetime; }

// Setters
void Transaction::setStatus(const std::string& new_status) { status = new_status; }
void Transaction::setDatetime(const std::string& new_datetime) { datetime = new_datetime; }

// Methods
void Transaction::execute() {
    if (status == "open") {
        time_t now = time(0);
        datetime = ctime(&now);

        if (type == "borrow") {
            if (user->borrowBook(book)) {
                status = "completed";
            } else {
                throw std::invalid_argument("Book is not available to borrow.");
            }
        } else if (type == "return") {
            if (user->returnBook(book)) {
                status = "completed";
            } else {
                throw std::invalid_argument("User has not borrowed this book.");
            }
        }
    }
    throw std::logic_error("Transaction is not open.");
}

void Transaction::cancel() {
    if (status == "open") {
        status = "cancelled";
    }
}

// Method to get transaction info
std::string Transaction::getTransactionInfo() const {
    return "Transaction ID: " + std::to_string(getTransactionID()) + "\n" +
           "Type: " + getType() + "\n" +
           "Status: " + getStatus() + "\n" +
           "Book: " + getBook()->getTitle() + " by " + getBook()->getAuthor() + "\n" +
           "User: " + getUser()->getForename() + " " + getUser()->getSurname() + "\n" +
           "Datetime: " + getDatetime();
}