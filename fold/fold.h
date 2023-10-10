#pragma once

#include <vector>
#include <stddef.h>

#define UNUSED(x) (void)(x)

struct Sum {
    template <typename T>
    T operator()(const T& a, const T& b) const {
        return a + b;
    }
};

struct Prod {
    template <typename T>
    T operator()(const T& a, const T& b) const {
        return a * b;
    }
};

struct Concat {
    template <typename T>
    std::vector<T> operator()(const std::vector<T>& a, const std::vector<T>& b) const {
        std::vector<T> res;
        res.reserve(a.size() + b.size());
        res.insert(res.end(), a.begin(), a.end());
        res.insert(res.end(), b.begin(), b.end());
        return res;
    }
};

class Length {

public:
    Length(size_t* len) : len_(len) {
    }
    template <typename T>
    T operator()(const T& a, const T& b) const {
        UNUSED(b);
        (*len_)++;
        return a;
    }

private:
    size_t* len_;
};

template <class Iterator, class T, class BinaryOp>
T Fold(Iterator first, Iterator last, T init, BinaryOp func) {
    T ans = init;
    while (first != last) {
        ans = func(ans, (*first));
        ++first;
    }
    return ans;
}
