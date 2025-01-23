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