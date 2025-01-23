#ifndef PERSISTENCE_HPP
#define PERSISTENCE_HPP

#include "Database.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <mutex>

class PersistenceManager {
private:
    std::string filename;
    mutable std::mutex mtx;

public:
    PersistenceManager(const std::string& filename) : filename(filename) {}

    void save(const Database& db) const;
    void load(Database& db) const;
};

#endif // PERSISTENCE_HPP