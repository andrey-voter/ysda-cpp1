#pragma once

#include <cstddef>
#include <initializer_list>
#include <utility>
#include <iostream>

class Deque {
public:

    void DequeStats() {
        std::cout << "------------------Deque stats------------------" << std::endl;
        std::cout << "num of chunks: " << num_of_chunks_ << std::endl;
        std::cout << "size: " << size_ << std::endl;
        std::cout << "left_chunk: " << left_chunk_ << std::endl;
        std::cout << "left: " << left_ << std::endl;
        std::cout << "right_chunk: " << right_chunk_ << std::endl;
        std::cout << "right: " << right_ << std::endl;
        std::cout << "chunk_size_: " << chunk_size_ << std::endl;
        std::cout << "chunks: " << chunks_ << std::endl;
        std::cout << "-----------------------------------------------" << std::endl;
    }


    Deque() = default;

    Deque(const Deque& other)
        : left_chunk_(other.left_chunk_),
          right_chunk_(other.right_chunk_),
          chunk_size_(other.chunk_size_),
          num_of_chunks_(other.num_of_chunks_),
          size_(other.size_),
          left_(other.left_),
          right_(other.right_) {

//        std::cout << " copy cons " << std::endl;

        DelData();
        chunks_ = new int*[num_of_chunks_];

        for (size_t i = 0; i < num_of_chunks_; ++i) {
            chunks_[i] = new int[chunk_size_];
        }

        for (size_t i = 0; i < num_of_chunks_; ++i) {
            for (size_t j = 0; j < chunk_size_; ++j) {
                chunks_[i][j] = other.chunks_[i][j];
            }
        }
    }

    Deque(Deque&& other)
        : left_chunk_(other.left_chunk_),
          right_chunk_(other.right_chunk_),
          chunk_size_(other.chunk_size_),
          num_of_chunks_(other.num_of_chunks_),
          size_(other.size_),
          left_(other.left_),
          right_(other.right_) {

//        std::cout << " move cons " << std::endl;
        DelData();
        chunks_ = other.chunks_;
        other.chunks_ = nullptr;
    }

    explicit Deque(size_t size) : size_(size) {
//        std::cout << " cons from size_t " << std::endl;
        //        memory allocation
        Allocate();

        //        initialization
        for (size_t i = 0; i < num_of_chunks_; ++i) {
            for (size_t j = 0; j < chunk_size_; ++j) {
                chunks_[i][j] = 0;
            }
        }
    }

    Deque(std::initializer_list<int> list) : size_(list.size()) {
//        std::cout << " init list cons " << std::endl;
        //        memory allocation
        Allocate();

        //        initialization
        size_t chunk_index = 0;
        size_t element_index = 0;
        for (const int elem : list) {
            chunks_[chunk_index][element_index] = elem;
            ++element_index;
            if (element_index % chunk_size_ == 0) {
                ++chunk_index;
                element_index = 0;
            }
        }

        left_chunk_ = 0;
        right_chunk_ = num_of_chunks_ - 1;
        right_ = element_index;
    }

    ~Deque() {
        DelData();
    }

    Deque& operator=(const Deque& other) {
        if (&other != this) {
            DelData();
            left_chunk_ = (other.left_chunk_);
            right_chunk_ = (other.right_chunk_);
            chunk_size_ = (other.chunk_size_);
            num_of_chunks_ = (other.num_of_chunks_);
            size_ = (other.size_);
            left_ = (other.left_);
            right_ = (other.right_);

            chunks_ = new int*[num_of_chunks_];

            for (size_t i = 0; i < num_of_chunks_; ++i) {
                chunks_[i] = new int[chunk_size_];
            }

            for (size_t i = 0; i < num_of_chunks_; ++i) {
                for (size_t j = 0; j < chunk_size_; ++j) {
                    chunks_[i][j] = other.chunks_[i][j];
                }
            }
        }
        return *this;
    }
    Deque& operator=(Deque&& other) {
        if (&other != this) {
            DelData();
            left_chunk_ = (other.left_chunk_);
            right_chunk_ = (other.right_chunk_);
            chunk_size_ = (other.chunk_size_);
            num_of_chunks_ = (other.num_of_chunks_);
            size_ = (other.size_);
            left_ = (other.left_);
            right_ = (other.right_);

            chunks_ = other.chunks_;
            other.chunks_ = nullptr;
        }
        return *this;
    }

    void Swap(Deque& other) {
        std::swap(chunks_, other.chunks_);
        std::swap(left_chunk_, other.left_chunk_);
        std::swap(right_chunk_, other.right_chunk_);
        std::swap(left_, other.left_);
        std::swap(right_, other.right_);
        std::swap(size_, other.size_);
        std::swap(chunk_size_, other.chunk_size_);
        std::swap(num_of_chunks_, other.num_of_chunks_);
    }

    void PushBack(int value) {
        if (!chunks_) {
            Allocate();
        }

        if (right_ == chunk_size_) {
            if (right_chunk_ + 1 == num_of_chunks_) {
                Reallocate();
            } else {
                ++right_chunk_;
                right_ = 0;
            }
        }
        chunks_[right_chunk_][right_++] = value;
        ++size_;
    }

    void PopBack() {
        if (!chunks_ or !size_) {
            return;
        }
        if (right_ != 0) {
            --right_;
        } else {
            --right_chunk_;
            right_ = chunk_size_ - 1;
        }
        --size_;
    }

    void PushFront(int value) {
        if (!chunks_) {
            Allocate();
        }

        if (left_ == 0) {
//            DequeStats();
            if (left_chunk_ == 0) {
                Reallocate(true);
            } else {
                --left_chunk_;
                left_ = chunk_size_ - 1;
            }
        }
//        DequeStats();
        chunks_[left_chunk_][left_--] = value;
        ++size_;
    }

    void PopFront() {
        if (!chunks_ or !size_) {
            return;
        }
        if (left_ != chunk_size_ - 1) {
            ++left_;
        } else {
            ++left_chunk_;
            left_ = 0;
        }
        --size_;
    }

    int& operator[](size_t index) {
        if (left_chunk_ == right_chunk_) {
            if (index < (chunk_size_ - 1 - left_)) {
                return chunks_[left_chunk_][left_ + index];
            }
        }

        if (index < (chunk_size_ - 1 - left_)) {
            return chunks_[left_chunk_][left_ + index + 1];
        }
        index -= (chunk_size_ - 1 - left_);
        int chunk_index = left_chunk_ + 1 + index / chunk_size_;
        int index_in_chunk = index % chunk_size_;
        return chunks_[chunk_index][index_in_chunk];
    }

    const int& operator[](size_t index) const {
        if (left_chunk_ == right_chunk_) {
            if (index < (chunk_size_ - 1 - left_)) {
                return chunks_[left_chunk_][left_ + index];
            }
        }

        if (index < (chunk_size_ - 1 - left_)) {
            return chunks_[left_chunk_][left_ + index + 1];
        }
        index -= (chunk_size_ - 1 - left_);
        int chunk_index = left_chunk_ + 1 + index / chunk_size_;
        int index_in_chunk = index % chunk_size_;
        return chunks_[chunk_index][index_in_chunk];
    }

    size_t Size() const {
        return size_;
    }

    void Clear() {
        DelData();

        chunks_ = nullptr;
        left_chunk_ = 0;
        right_chunk_ = 0;

        num_of_chunks_ = 0;

        size_ = 0;
        left_ = chunk_size_ - 1;
        right_ = 0;
    }

    void DelData() {
        if (chunks_) {
            for (size_t i = 0; i < num_of_chunks_; ++i) {
                delete[] chunks_[i];
            }
        }
        delete[] chunks_;
    }

    void Allocate() {
        num_of_chunks_ = size_ / chunk_size_ + 1;
        chunks_ = new int*[num_of_chunks_];
        for (size_t i = 0; i < num_of_chunks_; ++i) {
            chunks_[i] = new int[chunk_size_];
        }

        if (num_of_chunks_ == 1) {
            left_chunk_ = right_chunk_ = left_ = right_ = 0;
        } else {
            left_chunk_ = num_of_chunks_ / 2 - 1;
            right_chunk_ = num_of_chunks_ / 2;

            left_ = chunk_size_ - 1;
            right_ = 0;
        }
    }

    void Reallocate(bool called_from_push_front = false) {
//        std::cout << "reallocate " << std::endl;
        int** new_chunks = new int*[3 * num_of_chunks_];

        for (size_t i = 0; i < num_of_chunks_; ++i) {
            new_chunks[i] = new int[chunk_size_];
        }

        for (size_t i = 2 * num_of_chunks_; i < 3 * num_of_chunks_; ++i) {
            new_chunks[i] = new int[chunk_size_];
        }


        size_t old_chunk_index = 0;
        for (size_t chunk_index = num_of_chunks_; chunk_index < 2 * num_of_chunks_; ++chunk_index) {
            new_chunks[chunk_index] = chunks_[old_chunk_index++];
        }

        left_chunk_ = num_of_chunks_ - 1;
        right_chunk_ = 2 * num_of_chunks_;

        if (called_from_push_front) {
            --right_chunk_;
            left_ = chunk_size_ - 1;
        } else {
            right_ = 0;
        }

        delete[] chunks_;
        chunks_ = new_chunks;
        num_of_chunks_ *= 3;
    }

private:
    int** chunks_ = nullptr;
    size_t left_chunk_ = 0;
    size_t right_chunk_ = 0;

    size_t chunk_size_ = 128;
    size_t num_of_chunks_ = 0;

    size_t size_ = 0;
    size_t left_ = 0;
    size_t right_ = 0;
};
