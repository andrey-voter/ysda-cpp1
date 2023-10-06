#pragma once

#include <cstddef>
#include <vector>

class Stack {
public:
    void Push(int x) {
        data_.push_back(x);
    }

    bool Pop() {
        if (data_.empty()) {
            return false;
        }
        data_.pop_back();
        return true;
    }

    int Top() const {
        if (!data_.empty()) {
            return data_[data_.size() - 1];
        }
        return 0;
    }

    bool Empty() const {
        return data_.empty();
    }

    size_t Size() const {
        return data_.size();
    }

private:
    std::vector<int> data_;
};
