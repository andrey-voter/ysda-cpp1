#pragma once

#include <string>
#include <stdexcept>
#include <unordered_set>

char my_tolower(char ch) {
    return static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
}

bool my_isalpha(char ch) {
    return std::isalpha(static_cast<unsigned char>(ch));
}

int DifferentWordsCount(const std::string& string) {
    std::string current;
    std::unordered_set<std::string> words;
    for(const char c: string) {
        if (my_isalpha(c)) {
            current.push_back(my_tolower(c));
        }
        else {
            if (!current.empty()) {
                words.insert(current);
                current = "";
            }
        }
    }
    if (!current.empty()) {
        words.insert(current);
    }
    return words.size();
}
