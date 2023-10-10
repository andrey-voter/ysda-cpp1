#pragma once

#include <stdexcept>

template <class Iterator>
Iterator LocalMax(Iterator first, Iterator last) {
    if (first == last or std::next(first) == last) {
        return first;
    }
    if ((*(std::next(first))) < (*first)) {
        return first;
    }
    Iterator prev = first;
    first++;
    Iterator ans = first;
    while (ans != last) {
        Iterator next = std::next(ans);
        if (next != last) {
            if (((*prev) < (*ans)) and ((*next) < (*ans))) {
                return ans;
            }
        } else {
            if ((*prev) < (*ans)) {
                return ans;
            } else {
                return last;
            }
        }
        ans++;
        prev++;
    }
    return ans;
}
