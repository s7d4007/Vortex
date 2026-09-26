#ifndef UTILS_HPP
#define UTILS_HPP
#include<vector>
#include <unordered_set>
#include <string>
#include <cctype>
#include <algorithm>

inline std::string normalize_text(const std::string& input) {
    std::string result = input;
    result.erase(std::remove_if(result.begin(), result.end(), ::ispunct), result.end());
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

inline bool is_stop_word(const std::string& word) {
    static const std::unordered_set<std::string> stop_words = {
        "a", "an", "and", "are", "as", "at", "be", "but", "by",
        "for", "if", "in", "into", "is", "it", "no", "not", "of",
        "on", "or", "such", "that", "the", "their", "then", "there",
        "these", "they", "this", "to", "was", "will", "with"
    };
    return stop_words.find(word) != stop_words.end();
}

inline int levenshtein_distance(const std::string& s1, const std::string& s2) {
    int m = static_cast<int>(s1.size());
    int n = static_cast<int>(s2.size());
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));
    
    for (int i = 0; i <= m; ++i) dp[i][0] = i;
    for (int j = 0; j <= n; ++j) dp[0][j] = j;
    
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1 + std::min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
            }
        }
    }
    return dp[m][n];
}

#endif