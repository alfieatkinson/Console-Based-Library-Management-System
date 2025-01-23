#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <stdexcept>

// Levenshtein distance for approximate string matching
int levenshtein(const std::string& s1, const std::string& s2);

// Helper function to convert a string to lower case
std::string toLowerCase(const std::string& input);

#endif // UTILS_HPP