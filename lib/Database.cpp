#include "Database.hpp"
#include "Book.hpp"
#include "User.hpp"
#include "Transaction.hpp"
#include <algorithm>
#include <stdexcept>
#include <memory>
#include <cmath>

// Levenshtein distance for approximate string matching
int levenshtein(const std::string& s1, const std::string& s2) {
    int len1 = s1.length();
    int len2 = s2.length();
    std::vector<std::vector<int>> dist(len1 + 1, std::vector<int>(len2 + 1));

    for (int i = 0; i <= len1; i++) {
        for (int j = 0; j <= len2; j++) {
            if (i == 0) dist[i][j] = j;
            else if (j == 0) dist[i][j] = i;
            else {
                dist[i][j] = std::min({dist[i - 1][j] + 1, dist[i][j - 1] + 1, dist[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 0 : 1)});
            }
        }
    }
    return dist[len1][len2];
}

// Helper function for searching by ID (for Book, User, Transaction)
template <typename T>
std::shared_ptr<T> findByID(std::vector<std::shared_ptr<T>>& vec, int id) {
    auto it = std::find_if(vec.begin(), vec.end(), [id](const std::shared_ptr<T>& obj) {
        return obj->getID() == id;
    });

    if (it != vec.end()) {
        return *it;
    }

    throw std::invalid_argument("Object not found");
}

// Getters
const std::vector<std::shared_ptr<Book>>& Database::getBooks() const { return books; }
const std::vector<std::shared_ptr<User>>& Database::getUsers() const { return users; }
const std::vector<std::shared_ptr<Transaction>>& Database::getTransactions() const { return transactions; }
int Database::getBookIDCounter() const { return book_id_counter; }
int Database::getUserIDCounter() const { return user_id_counter; }
int Database::getTransactionIDCounter() const { return transaction_id_counter; }

// Setters
void Database::setBookIDCounter(int new_counter) { book_id_counter = new_counter; }
void Database::setUserIDCounter(int new_counter) { user_id_counter = new_counter; }
void Database::setTransactionIDCounter(int new_counter) { transaction_id_counter = new_counter; }

// Create operations
void Database::createBook(const std::string& title, const std::string& author, const std::string& isbn, int year) {
    books.push_back(std::make_shared<Book>(++book_id_counter, title, author, isbn, year, true));
}

void Database::createUser(const std::string& username, const std::string& forename, const std::string& surname, const std::string& email, const std::string& phone, const std::string& password) {
    users.push_back(std::make_shared<User>(++user_id_counter, username, forename, surname, email, phone, password));
}

void Database::createTransaction(const std::string& type, std::shared_ptr<Book> book, std::shared_ptr<User> user) {
    transactions.push_back(std::make_shared<Transaction>(++transaction_id_counter, type, book, user));
}

// Read operations
std::shared_ptr<Book> Database::readBook(int id) { return findByID(books, id); }
std::shared_ptr<User> Database::readUser(int id) { return findByID(users, id); }
std::shared_ptr<Transaction> Database::readTransaction(int id) { return findByID(transactions, id); }

// Update operations
void Database::updateBook(int id, const std::string& field, const std::string& value) {
    auto book = findByID(books, id);
    if (field == "title") book->setTitle(value);
    else if (field == "author") book->setAuthor(value);
    else if (field == "isbn") book->setISBN(value);
    else if (field == "year_published") book->setYearPublished(std::stoi(value));
    else if (field == "available") book->setIsAvailable(value == "true");
    else throw std::invalid_argument("Invalid field");
}

void Database::updateUser(int id, const std::string& field, const std::string& value) {
    auto user = findByID(users, id);
    if (field == "username") user->setUsername(value);
    else if (field == "forename") user->setForename(value);
    else if (field == "surname") user->setSurname(value);
    else if (field == "email") user->setEmail(value);
    else if (field == "phone_number") user->setPhoneNumber(value);
    else if (field == "password") user->setPassword(value);
    else throw std::invalid_argument("Invalid field");
}

void Database::updateTransaction(int id, const std::string& field, const std::string& value) {
    auto transaction = findByID(transactions, id);
    if (field == "status") transaction->setStatus(value);
    else if (field == "datetime") transaction->setDatetime(value);
    else throw std::invalid_argument("Invalid field");
}

// Delete operations
void Database::deleteBook(int id) {
    auto book = findByID(books, id);
    books.erase(std::remove(books.begin(), books.end(), book), books.end());
}

void Database::deleteUser(int id) {
    auto user = findByID(users, id);
    users.erase(std::remove(users.begin(), users.end(), user), users.end());
}

void Database::deleteTransaction(int id) {
    auto transaction = findByID(transactions, id);
    transactions.erase(std::remove(transactions.begin(), transactions.end(), transaction), transactions.end());
}

// Query operations with approximate search
std::vector<std::shared_ptr<Book>> Database::queryBooks(const std::string& search_term, int threshold) {
    std::vector<std::shared_ptr<Book>> result;
    for (auto& book : books) {
        if (levenshtein(book->getTitle(), search_term) <= threshold || 
            levenshtein(book->getAuthor(), search_term) <= threshold || 
            levenshtein(book->getISBN(), search_term) <= threshold) {
            result.push_back(book);
        }
    }
    return result;
}

std::vector<std::shared_ptr<User>> Database::queryUsers(const std::string& search_term, int threshold) {
    std::vector<std::shared_ptr<User>> result;
    for (auto& user : users) {
        if (levenshtein(user->getForename(), search_term) <= threshold || 
            levenshtein(user->getSurname(), search_term) <= threshold || 
            levenshtein(user->getUsername(), search_term) <= threshold || 
            levenshtein(user->getEmail(), search_term) <= threshold) {
            result.push_back(user);
        }
    }
    return result;
}

std::vector<std::shared_ptr<Transaction>> Database::queryTransactions(const std::string& search_term, int threshold) {
    std::vector<std::shared_ptr<Transaction>> result;
    for (auto& transaction : transactions) {
        auto book = transaction->getBook();
        auto user = transaction->getUser();

        if (levenshtein(transaction->getStatus(), search_term) <= threshold ||
            levenshtein(transaction->getDatetime(), search_term) <= threshold ||
            levenshtein(book->getTitle(), search_term) <= threshold ||
            levenshtein(book->getAuthor(), search_term) <= threshold ||
            levenshtein(book->getISBN(), search_term) <= threshold ||
            levenshtein(user->getUsername(), search_term) <= threshold ||
            levenshtein(user->getForename(), search_term) <= threshold ||
            levenshtein(user->getSurname(), search_term) <= threshold ||
            levenshtein(user->getEmail(), search_term) <= threshold) {
            result.push_back(transaction);
        }
    }
    return result;
}