#pragma once

#include <vector>
#include <stdexcept>

std::vector<int> Range(int from, int to, int step = 1) {
    if (step == 0 || (from == to) || ((from < to) && (step < 0)) || ((from > to) && (step > 0))) {
        return std::vector<int>();
    }
    auto res = std::vector<int>();
    if (from > to) {
        for (int i = from; i > to; i += step) {
            res.push_back(i);
        }
    } else {
        for (int i = from; i < to; i += step) {
            res.push_back(i);
        }
    }
    return res;
}
