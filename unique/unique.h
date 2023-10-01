#pragma once

#include <vector>
#include <stdexcept>

std::vector<int> Unique(const std::vector<int>& data) {
    if (data.empty()) {
        return std::vector<int>();
    }
    std::vector<int> res;
    res.push_back(data[0]);
    for (size_t i = 1; i < data.size(); ++i) {
        if (data[i - 1] != data[i]) {
            res.push_back(data[i]);
        }
    }
    return res;
}
