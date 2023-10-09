#pragma once

#include <stdexcept>

template <class Iterator, class T>
Iterator FindLast(Iterator first, Iterator last, const T& value) {
    Iterator ans = last;
    while (first != last) {
        if ((*first) == value) {
            ans = first;
        }
        first++;
    }
    return ans;
}
