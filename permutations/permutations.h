#pragma once

#include <vector>
#include <stdexcept>
#include <numeric>
#include <algorithm>

void Generate(std::vector<int>& arr, size_t index, std::vector<std::vector<int>>& permutations) {
    if (index == arr.size()) {
        permutations.push_back(arr);
        return;
    }
    for (size_t i = index; i < arr.size(); ++i) {
        std::swap(arr[index], arr[i]);
        Generate(arr, index + 1, permutations);
        std::swap(arr[index], arr[i]);
    }
}

std::vector<std::vector<int>> GeneratePermutations(size_t len) {
    std::vector<std::vector<int>> permutations;
    std::vector<int> initial;
    for (size_t i = 0; i < len; ++i) {
        initial.push_back(static_cast<int>(i));
    }
    Generate(initial, 0, permutations);
    std::sort(permutations.begin(), permutations.end());
    return permutations;
}
