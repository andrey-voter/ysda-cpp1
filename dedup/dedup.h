#pragma once

#include <memory>
#include <vector>
#include <iostream>

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
    std::cout << items[0];
//    for (size_t i = 0; i < items.size(); ++i) {
//
//        res.emplace_back(std::make_shared<T>(*items[i]).get());
//    }
    return res;
}
