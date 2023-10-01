#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <algorithm>
#include <map>
#include <set>

int64_t CountPairs(const std::vector<int>& data, int x) {
    std::map<int64_t, int64_t> freq;
    int64_t ans = 0;
    for (size_t i = 0; i < data.size(); ++i) {
        int64_t complement = x - data[i];
        if (freq.contains(complement)) {
            ans += freq[complement];
        }
        freq[data[i]]++;
    }
    return ans;
}
