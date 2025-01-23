#include "Persistence.hpp"
#include <fstream>
#include <iostream>

// Helper function to convert a string to lower case
std::string toLowerCase(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

// Method to save the database to a file
bool PersistenceManager::save(const Database& db) const {
    std::lock_guard<std::mutex> lock(mtx);

    nlohmann::json j;

    // Serialize books
    for (const auto& book : db.getBooks()) {
        j["books"].push_back({
            {"book_id", book->getID()},
            {"title", book->getTitle()},
            {"author", book->getAuthor()},
            {"isbn", book->getISBN()},
            {"year_published", book->getYearPublished()},
            {"available", book->isAvailable()}
        });
    }

    // Serialize users
    for (const auto& user : db.getUsers()) {
        j["users"].push_back({
            {"user_id", user->getID()},
            {"username", user->getUsername()},
            {"forename", user->getForename()},
            {"surname", user->getSurname()},
            {"email", user->getEmail()},
            {"phone_number", user->getPhoneNumber()},
            {"password", user->getPassword()}
        });
    }

    // Serialize transactions
    for (const auto& transaction : db.getTransactions()) {
        j["transactions"].push_back({
            {"transaction_id", transaction->getID()},
            {"type", transaction->getType()},
            {"status", transaction->getStatus()},
            {"book_id", transaction->getBook()->getID()},
            {"user_id", transaction->getUser()->getID()},
            {"datetime", transaction->getDatetime()}
        });
    }

    // Serialize ID counters
    j["book_id_counter"] = db.getBookIDCounter();
    j["user_id_counter"] = db.getUserIDCounter();
    j["transaction_id_counter"] = db.getTransactionIDCounter();

    // Write to file
    std::ofstream file(filename);
    if (file.is_open()) {
        file << j.dump(4);
        file.close();
        std::cout << "Data saved to " << filename << std::endl;
        return true;
    } else {
        std::cerr << "Unable to open file for saving: " << filename << std::endl;
        return false;
    }
}

// Method to load the database from a file
bool PersistenceManager::load(Database& db) const {
    std::lock_guard<std::mutex> lock(mtx);

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file for loading: " << filename << std::endl;
        return false;
    }

    nlohmann::json j;
    file >> j;
    file.close();

    // Check if JSON data is empty
    if (j.empty()) {
        std::cout << "No data to load. Populating database with test data." << std::endl;
        return false;
    }

    // Deserialize books
    for (const auto& item : j["books"]) {
        auto book = std::make_shared<Book>(
            item["book_id"].get<int>(),
            item["title"].get<std::string>(),
            item["author"].get<std::string>(),
            item["isbn"].get<std::string>(),
            item["year_published"].get<int>(),
            item["available"].get<bool>()
        );
        db.books.push_back(book);

        // Populate maps
        db.book_author_map[toLowerCase(book->getAuthor())].push_back(book);
        db.book_title_map[toLowerCase(book->getTitle())].push_back(book);
        db.book_isbn_map[toLowerCase(book->getISBN())].push_back(book);
    }

    // Deserialize users
    for (const auto& item : j["users"]) {
        auto user = std::make_shared<User>(
            item["user_id"].get<int>(),
            item["username"].get<std::string>(),
            item["forename"].get<std::string>(),
            item["surname"].get<std::string>(),
            item["email"].get<std::string>(),
            item["phone_number"].get<std::string>(),
            item["password"].get<std::string>()
        );
        db.users.push_back(user);

        // Populate maps
        db.user_forename_map[toLowerCase(user->getForename())].push_back(user);
        db.user_surname_map[toLowerCase(user->getSurname())].push_back(user);
        db.user_username_map[toLowerCase(user->getUsername())].push_back(user);
    }

    // Deserialize transactions
    for (const auto& item : j["transactions"]) {
        auto book = db.readBook(item["book_id"].get<int>());
        auto user = db.readUser(item["user_id"].get<int>());
        auto transaction = std::make_shared<Transaction>(
            item["transaction_id"].get<int>(),
            item["type"].get<std::string>(),
            book,
            user
        );
        transaction->setStatus(item["status"].get<std::string>());
        transaction->setDatetime(item["datetime"].get<std::string>());
        db.transactions.push_back(transaction);

        // Populate maps
        db.transactions_by_book_id[book->getID()].push_back(transaction);
        db.transactions_by_user_id[user->getID()].push_back(transaction);
    }

    // Deserialize ID counters
    db.setBookIDCounter(j["book_id_counter"].get<int>());
    db.setUserIDCounter(j["user_id_counter"].get<int>());
    db.setTransactionIDCounter(j["transaction_id_counter"].get<int>());

    std::cout << "Data loaded from " << filename << std::endl;
    return true;
}