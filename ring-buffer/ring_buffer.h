#pragma once

#include <cstddef>
#include <vector>

class RingBuffer {
public:
    explicit RingBuffer(size_t capacity)
        : data_(std::vector<int>(capacity)), capacity_(capacity), head_(0), tail_(0), size_(0) {
    }

    size_t Size() const {
        return size_;
    }

    bool Empty() const {
        return size_ == 0;
    }

    bool TryPush(int element) {
        if (size_ < capacity_) {
            data_[tail_] = element;
            tail_ = (tail_ + 1) % capacity_;
            size_++;
            return true;
        }
        return false;
    }

    bool TryPop(int* element) {
        if (!Empty()) {
            *element = data_[head_];
            head_ = (head_ + 1) % capacity_;
            size_--;
            return true;
        }
        return false;
    }

private:
    std::vector<int> data_;
    int capacity_;
    int head_;
    int tail_;
    int size_;
};
