#pragma once

#include <stdexcept>
#include <vector>
#include <cstddef>

void Reverse(std::vector<int>& data, int start, int end) {
    while (end > start) {
        std::swap(data[start], data[end]);
        end--;
        start++;
    }
}

void Rotate(std::vector<int>* data, size_t shift) {
    if (data->empty() or shift == 0) {
        return;
    }
    Reverse(*data, 0, shift - 1);
    Reverse(*data, shift, data->size() - 1);
    Reverse(*data, 0, data->size() - 1);
}
