#include "Utils.hpp"

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

// Helper function to convert a string to lower case
std::string toLowerCase(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}