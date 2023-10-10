#pragma once

#include <cstddef>
#include <algorithm>

template <class Functor>
class ReverseUnaryFunctor {
public:
    ReverseUnaryFunctor(Functor functor) : functor_(functor) {
    }

    template <class T>
    bool operator()(const T& arg) const {
        return !functor_(arg);
    }

private:
    Functor functor_;
};

template <class Functor>
class ReverseBinaryFunctor {
public:
    ReverseBinaryFunctor(Functor functor) : functor_(functor) {
    }

    template <class T, class U>
    bool operator()(const T& arg1, const U& arg2) const {
        return functor_(arg2, arg1);
    }

private:
    Functor functor_;
};

auto MakeReverseUnaryFunctor(auto functor) {
    return ReverseUnaryFunctor{functor};
}

auto MakeReverseBinaryFunctor(auto functor) {
    return ReverseBinaryFunctor{functor};
}

template <class Iterator>
size_t ComparisonsCount(Iterator first, Iterator last) {
    size_t count = 0;

    auto comparator = [&](const auto& arg1, const auto& arg2) {
        ++count;
        return arg1 < arg2;
    };

    std::sort(first, last, comparator);

    return count;
}
