#include "Database.hpp"
#include "Book.hpp"
#include "User.hpp"
#include "Transaction.hpp"
#include <algorithm>
#include <stdexcept>
#include <memory>
#include <cmath>
#include <iostream>

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