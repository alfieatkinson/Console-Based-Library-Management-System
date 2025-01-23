#include "Persistence.hpp"
#include <fstream>
#include <iostream>

void PersistenceManager::save(const Database& db) const {
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
    } else {
        std::cerr << "Unable to open file for saving: " << filename << std::endl;
    }
}