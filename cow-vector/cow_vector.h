#pragma once

#include <string>
#include <cstddef>

struct State {
    int ref_count = 1;
    std::string* data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
};

class COWVector {
public:
    COWVector();
    ~COWVector();

    COWVector(const COWVector& other);
    COWVector& operator=(const COWVector& other);

    size_t Size() const;

    void Resize(size_t size);

    const std::string& Get(size_t at) const;
    const std::string& Back() const;

    void PushBack(const std::string& value);

    void Set(size_t at, const std::string& value);

    //    void Print() {
    //        std::cout << "----Printing vector-----" << std::endl;
    //        std::cout << "size: " << state_->size_ << " capacity: " << state_->capacity_
    //                  << " n_copies: " << state_->ref_count << std::endl;
    //        for (size_t i = 0; i < state_->size_; ++i) {
    //            std::cout << state_->data_[i] << std::endl;
    //        }
    //        std::cout << "------------------------" << std::endl;
    //    }

private:
    State* state_;
};

COWVector::COWVector() : state_(new State) {
    state_->ref_count = 1;
}

COWVector::~COWVector() {
    if (!--state_->ref_count) {
        delete[] state_->data_;
        delete state_;
    }
}

COWVector::COWVector(const COWVector& other) : state_(other.state_) {
    ++state_->ref_count;
}

COWVector& COWVector::operator=(const COWVector& other) {
    if (&other != this) {
        if (!--state_->ref_count) {
            delete[] state_->data_;
        }
        state_ = other.state_;
        ++state_->ref_count;
    }
    return *this;
}

size_t COWVector::Size() const {
    return state_->size_;
}

const std::string& COWVector::Get(size_t at) const {
    return state_->data_[at];
}

void COWVector::Resize(size_t size) {
    if (size == state_->size_) {
        return;
    }
    if (state_->ref_count > 1) {
        auto new_state = new State(*state_);
        new_state->ref_count = 1;
        --state_->ref_count;
        auto new_data = new std::string[state_->capacity_];
        for (size_t i = 0; i < state_->size_; ++i) {
            new_data[i] = state_->data_[i];
        }
        state_ = new_state;
        state_->data_ = new_data;
    }
    if (state_->size_ < size) {
        size_t num_to_add = size - state_->size_;
        while (num_to_add--) {
            PushBack("");
        }
    }
    state_->size_ = size;
    state_->capacity_ = size;
}

const std::string& COWVector::Back() const {
    return state_->data_[state_->size_ - 1];
}

void COWVector::PushBack(const std::string& value) {
    if (state_->ref_count > 1) {
        auto new_state = new State(*state_);
        new_state->ref_count = 1;
        --state_->ref_count;
        auto new_data = new std::string[state_->capacity_];
        for (size_t i = 0; i < state_->size_; ++i) {
            new_data[i] = state_->data_[i];
        }
        state_ = new_state;
        state_->data_ = new_data;
    }

    if (!state_->capacity_) {
        state_->capacity_ = state_->size_ = 1;
        state_->data_ = new std::string[1];
        state_->data_[0] = value;
        return;
    }

    if (state_->size_ == state_->capacity_) {
        state_->capacity_ *= 2;
        std::string* new_data = new std::string[state_->capacity_];
        size_t i = 0;
        for (; i < state_->size_; ++i) {
            new_data[i] = state_->data_[i];
        }
        new_data[i] = value;
        delete[] state_->data_;
        state_->data_ = new_data;
    } else {
        state_->data_[state_->size_] = value;
    }
    ++state_->size_;
}

void COWVector::Set(size_t at, const std::string& value) {
    if (state_->ref_count > 1) {
        auto new_state = new State(*state_);
        new_state->ref_count = 1;
        --state_->ref_count;
        auto new_data = new std::string[state_->capacity_];
        for (size_t i = 0; i < state_->size_; ++i) {
            new_data[i] = state_->data_[i];
        }
        state_ = new_state;
        state_->data_ = new_data;
    }
    state_->data_[at] = value;
}