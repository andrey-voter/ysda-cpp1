#pragma once

#include <iterator>
#include <cstddef>
#include <algorithm>

class Vector {
public:
    class Iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = int;
        using difference_type = ptrdiff_t;
        using pointer = int*;
        using reference = int&;

        auto operator<=>(const Iterator&) const = default;

        Iterator() : it_(nullptr) {
        }

        Iterator(pointer p) : it_(p) {
        }

        Iterator& operator++() {
            ++it_;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++it_;
            return temp;
        }

        Iterator& operator--() {
            --it_;
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            --it_;
            return temp;
        }

        reference operator*() const {
            return *it_;
        }

        reference operator[](difference_type n) const {
            return it_[n];
        }

        Iterator operator-(ptrdiff_t n) const {
            return Iterator(it_ - n);
        }

        ptrdiff_t operator-(const Iterator& rhs) const {
            return it_ - rhs.it_;
        }

        Iterator operator+(ptrdiff_t n) const {
            return Iterator(it_ + n);
        }

        Iterator& operator+=(ptrdiff_t n) {
            it_ += n;
            return *this;
        }

        Iterator& operator-=(ptrdiff_t n) {
            it_ -= n;
            return *this;
        }

        pointer operator->() {
            return it_;
        }

        friend Iterator operator+(Iterator::difference_type, Iterator);
        friend Iterator operator-(Iterator::difference_type, Iterator);

    private:
        pointer it_;
    };

    Vector() {
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    }

    Vector(int size) : size_(size), capacity_(size) {
        data_ = new int[capacity_]{};
    }

    Vector(std::initializer_list<int> initializer_list)
        : size_(initializer_list.size()), capacity_(initializer_list.size()) {
        data_ = new int[capacity_];
        std::copy(initializer_list.begin(), initializer_list.end(), data_);
    }

    Vector(const Vector& other) : size_(other.size_), capacity_(other.capacity_) {
        delete[] data_;
        if (other.data_) {
            data_ = new int[capacity_];
            for (size_t i = 0; i < other.size_; ++i) {
                data_[i] = other.data_[i];
            }
        }
    }

    Vector(Vector&& moved) : size_(moved.size_), capacity_(moved.capacity_) {
        delete[] data_;
        data_ = moved.data_;
        moved.data_ = nullptr;
        moved.size_ = 0;
        moved.capacity_ = 0;
    }

    Vector& operator=(const Vector& other) {
        if (&other != this) {
            delete[] data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = new int[capacity_];
            for (size_t i = 0; i < other.size_; ++i) {
                data_[i] = other.data_[i];
            }
        }
        return *this;
    }

    Vector& operator=(Vector&& moved) {
        if (&moved != this) {
            delete[] data_;
            size_ = moved.size_;
            capacity_ = moved.capacity_;
            data_ = moved.data_;
            moved.data_ = nullptr;
            moved.size_ = 0;
            moved.capacity_ = 0;
        }
        return *this;
    }

    ~Vector() {

        if (data_) {
            delete[] data_;
        }
    }

    void Swap(Vector& other) {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    int operator[](size_t index) const {
        return data_[index];
    }

    int& operator[](size_t index) {
        return data_[index];
    }

    size_t Size() const {
        return size_;
    }

    size_t Capacity() const {
        return capacity_;
    }

    void PushBack(int new_elem) {
        if (!capacity_) {
            capacity_ = size_ = 1;
            data_ = new int[1];
            data_[0] = new_elem;
            return;
        }
        if (size_ == capacity_) {
            capacity_ *= 2;
            int* new_data = new int[capacity_];
            size_t i = 0;
            for (; i < size_; ++i) {
                new_data[i] = data_[i];
            }
            new_data[i] = new_elem;
            delete[] data_;
            ++size_;
            data_ = new_data;
        } else {
            data_[size_] = new_elem;
            ++size_;
        }
    }

    void PopBack() {
        --size_;
    }

    void Clear() {
        size_ = 0;
    }

    void Reserve(size_t new_capacity) {
        if (capacity_ >= new_capacity) {
            return;
        }
        capacity_ = new_capacity;
        int* new_data = new int[capacity_];
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = data_[i];
        }
        delete[] data_;
        data_ = new_data;
    }

    Iterator begin() {
        return Iterator(data_);
    }

    Iterator end() {
        return Iterator(data_ + size_);
    }

private:
    int* data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
};

Vector::Iterator operator+(Vector::Iterator::difference_type n, Vector::Iterator iterator) {
    return Vector::Iterator(iterator.it_ + n);
}

Vector::Iterator operator-(Vector::Iterator::difference_type n, Vector::Iterator iterator) {
    return Vector::Iterator(iterator.it_ - n);
}
