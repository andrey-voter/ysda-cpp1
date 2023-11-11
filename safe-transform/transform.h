#pragma once

#include <vector>
#include <exception>
#include <utility>

template <class It, class Pred, class Func>
void TransformIf(It begin, It end, Pred p, Func f) {
//    std::vector<typename std::iterator_traits<It>::value_type> log;
    std::vector<std::pair<It, typename std::iterator_traits<It>::value_type>> log;
    try {
        for (auto it = begin; it != end; ++it) {
            if (p(*it)) {
                try {
                    log.emplace_back(it, *it);
                }
                catch (...) {}
                f(*it);
            }
        }
    } catch (...) {
        for (auto& pair : log) {
            *pair.first = pair.second;
        }
        throw;
    }
}
