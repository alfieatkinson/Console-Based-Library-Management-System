#ifndef PERSISTENCE_HPP
#define PERSISTENCE_HPP

#include "Database.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <mutex>

// Forward declaration
class Database;

class PersistenceManager {
private:
    std::string filename;
    mutable std::mutex mtx;

public:
    PersistenceManager(const std::string& filename) : filename(filename) {}

    bool save(const Database& db) const;
    bool load(Database& db) const;
};

#endif // PERSISTENCE_HPP