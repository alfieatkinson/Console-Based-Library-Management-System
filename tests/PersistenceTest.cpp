#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <memory>

#include "Persistence.hpp"
#include "Database.hpp"
#include "Book.hpp"
#include "User.hpp"
#include "Transaction.hpp"

TEST_CASE("PersistenceManager saves and loads data correctly") {
    // Create a temporary file for testing
    std::string filename = "test.json";

    // Create a PersistenceManager instance
    PersistenceManager pm(filename);

    // Create a Database instance and add some sample data
    Database db;
    db.createBook("The Great Gatsby", "F. Scott Fitzgerald", "9780743273565", 1925);
    db.createUser("jdoe", "John", "Doe", "jdoe@example.com", "+1234567890", "password123");
    db.createTransaction("borrow", db.readBook(1), db.readUser(1));

    // Save the database to a file
    pm.save(db);

    // Load the database from the file to a new Database instance
    Database db2;
    pm.load(db2);

    // Check if the loaded data matches the original data
    SECTION("Saved and loaded vector sizes match") {
        REQUIRE(db.getBooks().size() == db2.getBooks().size());
        REQUIRE(db.getUsers().size() == db2.getUsers().size());
        REQUIRE(db.getTransactions().size() == db2.getTransactions().size());
    }

    SECTION("Saved and loaded book data match") {
        auto book1 = db.readBook(1);
        auto book2 = db2.readBook(1);
        REQUIRE(book1->getID() == book2->getID());
        REQUIRE(book1->getTitle() == book2->getTitle());
        REQUIRE(book1->getAuthor() == book2->getAuthor());
        REQUIRE(book1->getISBN() == book2->getISBN());
        REQUIRE(book1->getYearPublished() == book2->getYearPublished());
        REQUIRE(book1->isAvailable() == book2->isAvailable());
    }

    SECTION("Saved and loaded user data match") {
        auto user1 = db.readUser(1);
        auto user2 = db2.readUser(1);
        REQUIRE(user1->getID() == user2->getID());
        REQUIRE(user1->getUsername() == user2->getUsername());
        REQUIRE(user1->getForename() == user2->getForename());
        REQUIRE(user1->getSurname() == user2->getSurname());
        REQUIRE(user1->getEmail() == user2->getEmail());
        REQUIRE(user1->getPhoneNumber() == user2->getPhoneNumber());
        REQUIRE(user1->getPassword() == user2->getPassword());
    }

    SECTION("Saved and loaded transaction data match") {
        auto transaction1 = db.readTransaction(1);
        auto transaction2 = db2.readTransaction(1);
        REQUIRE(transaction1->getID() == transaction2->getID());
        REQUIRE(transaction1->getType() == transaction2->getType());
        REQUIRE(transaction1->getStatus() == transaction2->getStatus());
        REQUIRE(transaction1->getBook()->getID() == transaction2->getBook()->getID());
        REQUIRE(transaction1->getUser()->getID() == transaction2->getUser()->getID());
        REQUIRE(transaction1->getDatetime() == transaction2->getDatetime());
    }
}