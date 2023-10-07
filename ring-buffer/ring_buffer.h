#pragma once

#include <cstddef>
#include <vector>

class RingBuffer {
public:
    explicit RingBuffer(size_t capacity) : capacity_(capacity) {
    }

    size_t Size() const {
        return data_.size();
    }

    bool Empty() const {
        return data_.empty();
    }

    bool TryPush(int element) {
        if (data_.size() < capacity_) {
            data_.push_back(element);
            return true;
        }
        return false;
    }

    bool TryPop(int* element) {
        if (!data_.empty()) {
            *element = data_[0];
            data_.erase(data_.begin());
            return true;
        }
        return false;
    }

private:
    std::vector<int> data_;
    size_t capacity_;
};
