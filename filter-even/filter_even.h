#pragma once

#include <vector>
#include <stdexcept>
#include <iostream>

void FilterEven(std::vector<int>* data) {
    if (data->empty()) {
        return;
    }
    int even_pos = 0;
    for (size_t i = 0; i < data->size(); ++i) {
        if ((*data)[i] % 2 == 0) {
            (*data)[even_pos] = (*data)[i];
            even_pos++;
        }
    }
    data->resize(even_pos);
}
