#pragma once

#include <string>
#include <unordered_map>
#include <stdexcept>

std::unordered_map<int, std::string> ReverseMap(const std::unordered_map<std::string, int>& map) {
    std::unordered_map<int, std::string> res;
    for (const auto& [string, number] : map) {
        res[number] = string;
    }
    return res;
}
