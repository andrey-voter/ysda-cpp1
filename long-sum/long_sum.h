#pragma once

#include <string>
#include <stdexcept>
#include <algorithm>
#include <iostream>

std::string LongSum(const std::string& a, const std::string& b) {
    std::string res;
    size_t a_size = a.size();
    size_t b_size = b.size();
    std::string a_copy(a);
    std::string b_copy(b);

    if (a_size < b_size) {
        std::string zeros = std::string(b_size - a_size, '0');
        std::reverse(a_copy.begin(), a_copy.end());
        a_copy += zeros;
        std::reverse(a_copy.begin(), a_copy.end());
    } else if (a_size > b_size) {
        std::string zeros = std::string(a_size - b_size, '0');
        std::reverse(b_copy.begin(), b_copy.end());
        b_copy += zeros;
        std::reverse(b_copy.begin(), b_copy.end());
    }

    int in_mind = 0;
    for (int i = a_copy.size() - 1; i >= 0; --i) {
        int val_1 = a_copy[i] - '0';
        int val_2 = b_copy[i] - '0';
        int s = val_1 + val_2 + in_mind;
        in_mind = 0;
        if (s >= 10) {
            in_mind = 1;
            s %= 10;
        }
        res.push_back(s + '0');
        if (!i && in_mind) {
            res.push_back(in_mind + '0');
        }
    }
    std::reverse(res.begin(), res.end());
    return res;
}
