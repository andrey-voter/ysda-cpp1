#pragma once

#include <memory>
#include <vector>
#include <iostream>
#include <unordered_map>

template <class T>
std::vector<std::unique_ptr<T>> Duplicate(const std::vector<std::shared_ptr<T>>& items) {
    std::vector<std::unique_ptr<T>> res;
    for (size_t i = 0; i < items.size(); ++i) {
        res.emplace_back(std::make_unique<T>(*items[i]));
    }
    return res;
}

template <class T>
std::vector<std::shared_ptr<T>> DeDuplicate(const std::vector<std::unique_ptr<T>>& items) {
    std::vector<std::shared_ptr<T>> res;
    std::unordered_map<T, std::shared_ptr<T>> t_to_ptr;

    for (size_t i = 0; i < items.size(); ++i) {
        if (t_to_ptr.contains(*items[i])) {
            res.push_back(t_to_ptr[*items[i]]);
        } else {
            t_to_ptr.insert({*items[i], std::make_shared<T>(*items[i])});
            res.push_back(t_to_ptr[*items[i]]);
        }
    }
    return res;
}
