#pragma once

#include <type_traits>
#include <concepts>
#include <exception>
#include <typeinfo>
#include <iostream>

template <class T>
concept NotAny = !std::same_as<std::remove_cvref_t<T>, class Any>;

class Any {
private:
    struct Base {
        virtual ~Base() {
        }
        virtual Base* GetCopy() = 0;
    };

    template <typename T>
    struct Derived : Base {
        T value;
        Derived(T value) : value(std::forward<T>(value)){};
        Base* GetCopy() override {
            return new Derived<T>(value);
        }
    };

    Base* ptr_ = nullptr;

public:
    Any() {
    }

    // T&& - universal (forwarding) reference
    // use std::forward inside this constructor
    template <NotAny T>
    Any(T&& value) : ptr_(new Derived<std::remove_cvref_t<T>>(std::forward<T>(value))) {

    }

    Any(const Any& other) {
        if (other.ptr_) {
            ptr_ = other.ptr_->GetCopy();
        }
    }

    Any(Any&& other) {
        delete ptr_;
        ptr_ = other.ptr_;
        other.ptr_ = nullptr;
    }

    Any& operator=(const Any& other) {
        if (&other != this) {
            auto cpy(other);
            delete ptr_;
            if (cpy.ptr_) {
                ptr_ = cpy.ptr_->GetCopy();
            } else {
                ptr_ = nullptr;
            }
        }
        return *this;
    }

    Any& operator=(Any&& other) {
        if (&other != this) {
            delete ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    ~Any() {
        Clear();
    }

    bool Empty() const {
        return ptr_ == nullptr;
    }

    void Clear() {
        delete ptr_;
        ptr_ = nullptr;
    }
    void Swap(Any& other) {
        std::swap(ptr_, other.ptr_);
    }

    template <class T>
    const T& GetValue() const {
        auto v = dynamic_cast<Derived<T>*>(ptr_);
        if (!v) {
            throw std::bad_cast();
        }
        return v->value;
    }
};
