#pragma once

#include <utility>
#include <vector>
#include <stdexcept>
#include <cmath>

std::vector<std::pair<int64_t, int>> Factorize(int64_t x) {
    std::vector<std::pair<int64_t, int>> ans;
    for (int64_t i = 2; i <= sqrt(x); ++i) {
        if (x % i == 0) {
            int deg = 0;
            while (x % i == 0) {
                deg += 1;
                x /= i;
            }
            ans.emplace_back(i, deg);
        }
    }
    if (ans.empty() || x != 1) {
        ans.emplace_back(x, 1);
    }
    return ans;
}
