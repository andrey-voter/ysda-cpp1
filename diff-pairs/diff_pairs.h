#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <algorithm>
#include <map>
#include <set>

int64_t CountPairs(const std::vector<int>& data, int x) {
    std::map<int, int> freq;
    int ans = 0;
    for (size_t i = 0; i < data.size(); ++i) {
        int complement = x - data[i];
        if (freq.contains(complement)) {
            ans += freq[complement];
        }
        freq[data[i]]++;
    }
    return ans;
}
