#pragma once

#include <stdexcept>

template <class Iterator, class Predicate>
Iterator Partition(Iterator first, Iterator last, Predicate pred) {
    if (first == last) {
        return first;
    }
    Iterator insert_pos = first;
    while (first != last) {
        if (pred(*first)) {
            std::swap((*insert_pos), (*first));
            ++insert_pos;
        }
        ++first;
    }
    return insert_pos;
}
