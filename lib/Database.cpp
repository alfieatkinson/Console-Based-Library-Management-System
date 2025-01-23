#include "Database.hpp"
#include "Book.hpp"
#include "User.hpp"
#include "Transaction.hpp"
#include "Utils.hpp"
#include <algorithm>
#include <stdexcept>
#include <memory>
#include <cmath>
#include <cctype>
#include <iostream>

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

// Constructor
Database::Database() : persistence_manager(std::make_unique<PersistenceManager>("database.json")) {}

// Destructor
Database::~Database() {}

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

// Save and load operations
bool Database::save() const {
    return persistence_manager->save(*this);
}

bool Database::load() {
    return persistence_manager->load(*this);
}

// Create operations
int Database::createBook(const std::string& title, const std::string& author, const std::string& isbn, int year) {
    auto new_book = std::make_shared<Book>(++book_id_counter, title, author, isbn, year, true);
    books.push_back(new_book);

    // Update maps for searching
    book_author_map[toLowerCase(author)].push_back(new_book);
    book_title_map[toLowerCase(title)].push_back(new_book);
    book_isbn_map[toLowerCase(isbn)].push_back(new_book);

    // Return the new book ID
    return book_id_counter;
}

int Database::createUser(const std::string& username, const std::string& forename, const std::string& surname, const std::string& email, const std::string& phone, const std::string& password) {
    auto new_user = std::make_shared<User>(++user_id_counter, username, forename, surname, email, phone, password);
    users.push_back(new_user);

    // Update maps for searching
    user_forename_map[toLowerCase(forename)].push_back(new_user);
    user_surname_map[toLowerCase(surname)].push_back(new_user);
    user_username_map[toLowerCase(username)].push_back(new_user);

    // Return the new user ID
    return user_id_counter;
}

int Database::createTransaction(const std::string& type, std::shared_ptr<Book> book, std::shared_ptr<User> user) {
    auto new_transaction = std::make_shared<Transaction>(++transaction_id_counter, type, book, user);
    transactions.push_back(new_transaction);

    // Update maps for quick lookup
    transactions_by_book_id[book->getID()].push_back(new_transaction);
    transactions_by_user_id[user->getID()].push_back(new_transaction);

    // Return the new transaction ID
    return transaction_id_counter;
}

// Read operations
std::shared_ptr<Book> Database::readBook(int id) { return findByID(books, id); }
std::shared_ptr<User> Database::readUser(int id) { return findByID(users, id); }
std::shared_ptr<Transaction> Database::readTransaction(int id) { return findByID(transactions, id); }

// Update operations
void Database::updateBook(int id, const std::string& field, const std::string& value) {
    auto book = findByID(books, id);
    if (field == "title") {
        book->setTitle(value);
        book_title_map[toLowerCase(value)].push_back(book); // Update the map for title change
    }
    else if (field == "author") {
        book->setAuthor(value);
        book_author_map[toLowerCase(value)].push_back(book); // Update the map for author change
    }
    else if (field == "isbn") {
        book->setISBN(value);
        book_isbn_map[toLowerCase(value)].push_back(book); // Update the map for ISBN change
    }
    else if (field == "year_published") book->setYearPublished(std::stoi(value));
    else if (field == "available") book->setIsAvailable(value == "true");
}

void Database::updateUser(int id, const std::string& field, const std::string& value) {
    auto user = findByID(users, id);
    if (field == "username") {
        user->setUsername(value);
        // Update the map for username change
        user_username_map[toLowerCase(value)].push_back(user); // Update the map for username change
    }
    else if (field == "forename") {
        user->setForename(value);
        user_forename_map[toLowerCase(value)].push_back(user); // Update the map for forename change
    }
    else if (field == "surname") {
        user->setSurname(value);
        user_surname_map[toLowerCase(value)].push_back(user); // Update the map for surname change
    }
    else if (field == "email") user->setEmail(value);
    else if (field == "phone") user->setPhoneNumber(value);
    else if (field == "password") user->setPassword(value);
}

void Database::updateTransaction(int id, const std::string& field, const std::string& value) {
    auto transaction = findByID(transactions, id);
    if (field == "status") transaction->setStatus(value);
    else if (field == "datetime") transaction->setDatetime(value);
}

// Delete operations
void Database::deleteBook(int id) {
    auto book = findByID(books, id);
    books.erase(std::remove(books.begin(), books.end(), book), books.end());
    book_author_map[toLowerCase(book->getAuthor())].erase(
        std::remove(book_author_map[toLowerCase(book->getAuthor())].begin(), book_author_map[toLowerCase(book->getAuthor())].end(), book),
        book_author_map[toLowerCase(book->getAuthor())].end());
    book_title_map[toLowerCase(book->getTitle())].erase(
        std::remove(book_title_map[toLowerCase(book->getTitle())].begin(), book_title_map[toLowerCase(book->getTitle())].end(), book),
        book_title_map[toLowerCase(book->getTitle())].end());
    book_isbn_map[toLowerCase(book->getISBN())].erase(
        std::remove(book_isbn_map[toLowerCase(book->getISBN())].begin(), book_isbn_map[toLowerCase(book->getISBN())].end(), book),
        book_isbn_map[toLowerCase(book->getISBN())].end());
}

void Database::deleteUser(int id) {
    auto user = findByID(users, id);
    users.erase(std::remove(users.begin(), users.end(), user), users.end());
    user_forename_map[toLowerCase(user->getForename())].erase(
        std::remove(user_forename_map[toLowerCase(user->getForename())].begin(), user_forename_map[toLowerCase(user->getForename())].end(), user),
        user_forename_map[toLowerCase(user->getForename())].end());
    user_surname_map[toLowerCase(user->getSurname())].erase(
        std::remove(user_surname_map[toLowerCase(user->getSurname())].begin(), user_surname_map[toLowerCase(user->getSurname())].end(), user),
        user_surname_map[toLowerCase(user->getSurname())].end());
    user_username_map[toLowerCase(user->getUsername())].erase(
        std::remove(user_username_map[toLowerCase(user->getUsername())].begin(), user_username_map[toLowerCase(user->getUsername())].end(), user),
        user_username_map[toLowerCase(user->getUsername())].end());
}

void Database::deleteTransaction(int id) {
    auto transaction = findByID(transactions, id);
    transactions.erase(std::remove(transactions.begin(), transactions.end(), transaction), transactions.end());
    transactions_by_book_id[transaction->getBook()->getID()].erase(
        std::remove(transactions_by_book_id[transaction->getBook()->getID()].begin(), transactions_by_book_id[transaction->getBook()->getID()].end(), transaction),
        transactions_by_book_id[transaction->getBook()->getID()].end());
    transactions_by_user_id[transaction->getUser()->getID()].erase(
        std::remove(transactions_by_user_id[transaction->getUser()->getID()].begin(), transactions_by_user_id[transaction->getUser()->getID()].end(), transaction),
        transactions_by_user_id[transaction->getUser()->getID()].end());
}

// Query operations with Levenshtein distance approximation
std::vector<std::shared_ptr<Book>> Database::queryBooks(const std::string& search_term, int threshold) {
    std::set<std::shared_ptr<Book>> results;

    std::string term = toLowerCase(search_term);  // Ensure case-insensitive comparison
    auto query = [&](const auto& map) {
        for (const auto& pair : map) {
            // Compare Levenshtein distance between the search term and each key
            int distance = levenshtein(term, pair.first);
            if (distance <= threshold) {
                for (const auto& book : pair.second) {
                    results.insert(book);
                }
            }
        }
    };

    query(book_author_map);
    query(book_title_map);
    query(book_isbn_map);

    return std::vector<std::shared_ptr<Book>>(results.begin(), results.end());
}

std::vector<std::shared_ptr<User>> Database::queryUsers(const std::string& search_term, int threshold) {
    std::set<std::shared_ptr<User>> results;

    std::string term = toLowerCase(search_term);  // Ensure case-insensitive comparison
    auto query = [&](const auto& map) {
        for (const auto& pair : map) {
            // Compare Levenshtein distance between the search term and each key
            int distance = levenshtein(term, pair.first);
            if (distance <= threshold) {
                for (const auto& user : pair.second) {
                    results.insert(user);
                }
            }
        }
    };

    query(user_forename_map);
    query(user_surname_map);
    query(user_username_map);

    return std::vector<std::shared_ptr<User>>(results.begin(), results.end());
}

std::vector<std::shared_ptr<Transaction>> Database::queryTransactionsByBookID(int id) {
    return transactions_by_book_id[id];
}

std::vector<std::shared_ptr<Transaction>> Database::queryTransactionsByUserID(int id) {
    return transactions_by_user_id[id];
}

// User authentication methods
std::shared_ptr<User> Database::authenticateUser(const std::string& username, const std::string& password) {
    if (user_username_map.find(toLowerCase(username)) != user_username_map.end()) {
        for (const auto& user : user_username_map[toLowerCase(username)]) {
            if (user->getPassword() == password) {
                return user;
            }
        }
    }
    throw std::invalid_argument("Invalid username or password");
}

bool Database::authenticateAdmin(const std::string& password) {
    return password == admin_password;
}