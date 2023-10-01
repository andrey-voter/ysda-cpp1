#pragma once

#include <string>
#include <stdexcept>
#include <vector>

std::vector<std::string> Split(const std::string& string, const std::string& delimiter = " ") {
    if (string.empty()) {
        return std::vector<std::string>();
    }
    std::vector<std::string> res;
    auto delimiter_pos = string.find(delimiter);
    size_t prev_pos = 0;
    while (std::string::npos != delimiter_pos) {
        if (string.substr(prev_pos, delimiter.size()) == delimiter) {
            res.push_back("");
            prev_pos = delimiter_pos + delimiter.size();
            delimiter_pos = string.find(delimiter, delimiter_pos + delimiter.size());
            continue;
        }
        res.push_back(string.substr(prev_pos, delimiter_pos - prev_pos));
        prev_pos = delimiter_pos + delimiter.size();
        delimiter_pos = string.find(delimiter, delimiter_pos + delimiter.size());
    }
    if (prev_pos < string.size()) {
        res.push_back(string.substr(prev_pos));
    } else {
        if (prev_pos == string.size()) {
            res.push_back("");
        }
    }
    return res;
}
