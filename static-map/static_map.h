#pragma once

#include <vector>
#include <string>
#include <utility>
#include <algorithm>

class StaticMap {
public:
    explicit StaticMap(const std::vector<std::pair<std::string, std::string>>& items)
        : data_(items) {
        std::sort(data_.begin(), data_.end(),
                  [](const auto& a, const auto& b) { return a.first < b.first; });
    }

    bool Find(const std::string& key, std::string* value) const {
        auto it = std::lower_bound(data_.begin(), data_.end(), key,
                                   [](const auto& a, const auto& b) { return a.first < b; });
        if (it != data_.end() and it->first == key) {
            *value = it->second;
            return true;
        }
        return false;
    }

private:
    std::vector<std::pair<std::string, std::string>> data_;
};
