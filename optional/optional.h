#pragma once

#include <span>
#include <cstddef>
#include <stdexcept>

template <class T>
class alignas(T) Optional {
public:
    Optional() : data_() {
    }

    Optional(const T& value) {
        new (data_) T(value);
        has_value_ = true;
    }

    Optional(T&& value) {
        new (data_) T(std::move(value));
        has_value_ = true;
    }

    Optional(const Optional& other) {
        if (other.has_value_) {
            new (data_) T(*other);
            has_value_ = true;
        } else {
            has_value_ = false;
        }
    }

    Optional(Optional&& other) {
        if (other.has_value_) {
            new (data_) T(std::move(*other));
            has_value_ = true;
        } else {
            has_value_ = false;
        }
    }

    ~Optional() {
        if (has_value_) {
            reinterpret_cast<T*>(data_)->~T();
        }
    }

    Optional& operator=(const Optional& other) {
        if (other.has_value_) {
            T tmp(*other);
            if (has_value_) {
                reinterpret_cast<T*>(data_)->~T();
            }
            new (data_) T(tmp);
            has_value_ = true;
        } else {
            if (has_value_) {
                has_value_ = false;
                reinterpret_cast<T*>(data_)->~T();
            }
        }
        return *this;
    }

    Optional& operator=(Optional&& other) {
        if (other.has_value_) {
            T tmp(std::move(*other));
            if (has_value_) {
                reinterpret_cast<T*>(data_)->~T();
            }
            new (data_) T(std::move(tmp));
            has_value_ = true;
        } else {
            if (has_value_) {
                has_value_ = false;
                reinterpret_cast<T*>(data_)->~T();
            }
        }
        return *this;
    }

    T& operator*() & {
        return *reinterpret_cast<T*>(data_);
    }

    const T& operator*() const& {
        return *reinterpret_cast<const T*>(data_);
    }

    T&& operator*() && {
        return std::move(*reinterpret_cast<T*>(data_));
    }

    T* operator->() {
        return reinterpret_cast<T*>(data_);
    }

    const T* operator->() const {
        return reinterpret_cast<const T*>(data_);
    }

    T& Value() & {
        if (!has_value_) {
            throw std::runtime_error("shit happens");
        }
        return *reinterpret_cast<T*>(data_);
    }

    const T& Value() const& {
        if (!has_value_) {
            throw std::runtime_error("shit happens");
        }
        return *reinterpret_cast<const T*>(data_);
    }

    T&& Value() && {
        if (!has_value_) {
            throw std::runtime_error("shit happens");
        }
        return std::move(*reinterpret_cast<T*>(data_));
    }

    bool HasValue() const {
        return has_value_;
    }

    void Reset() {
        if (has_value_) {
            has_value_ = false;
            reinterpret_cast<T*>(data_)->~T();
        }
    }

private:
    std::byte data_[sizeof(T)];
    bool has_value_ = false;
};
