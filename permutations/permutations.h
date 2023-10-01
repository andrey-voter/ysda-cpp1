#pragma once

#include <vector>
#include <stdexcept>
#include <numeric>

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

bool CompareVectors(const std::vector<int>& a, const std::vector<int>& b) {
    int size = std::min(a.size(), b.size());
    for (int i = 0; i < size; ++i) {
        if (a[i] < b[i]) {
            return true;
        }
        if (a[i] > b[i]) {
            return false;
        }
    }
    return a.size() < b.size();
}

void CustomSort(std::vector<std::vector<int>>& vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
        for (size_t j = i + 1; j < vec.size(); ++j) {
            if (CompareVectors(vec[j], vec[i])) {
                std::swap(vec[i], vec[j]);
            }
        }
    }
}

std::vector<std::vector<int>> GeneratePermutations(size_t len) {
    std::vector<std::vector<int>> permutations;
    std::vector<int> initial;
    for (size_t i = 0; i < len; ++i) {
        initial.push_back(static_cast<int>(i));
    }
    Generate(initial, 0, permutations);
    CustomSort(permutations);
    return permutations;
}
