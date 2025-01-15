#include "Library.hpp"
#include "Database.hpp"
#include "Book.hpp"
#include "User.hpp"
#include "Transaction.hpp"

// Constructor
LibraryManager::LibraryManager() {
    // Load the database on construction
    loadDatabase();
}

// Destructor
LibraryManager::~LibraryManager() {
    // Save the database on destruction
    saveDatabase();
}