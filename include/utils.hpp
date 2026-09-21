#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <cctype>
#include <algorithm>

inline std::string normalize_text(const std::string& input) {
    std::string result = input;
    result.erase(std::remove_if(result.begin(), result.end(), ::ispunct), result.end());
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

#endif