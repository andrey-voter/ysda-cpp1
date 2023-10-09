#pragma once

#include <stdexcept>

template <class Iterator, class T>
Iterator FindLast(Iterator first, Iterator last, const T& value) {
    Iterator ans = last;
    while (last != first) {
        --last;
        if ((*last) == value) {
            ans = last;
            break;
        }
    }
    return ans;
}
