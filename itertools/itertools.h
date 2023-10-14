#pragma once

#include <cstddef>
#include <utility>
#include <cstdint>
#include <iterator>
#include <iostream>

template <class Iterator>
class Sequence {
public:
    Sequence(Iterator begin, Iterator end) : begin_{begin}, end_{end} {
    }

    Iterator begin() const {
        return begin_;
    }

    Iterator end() const {
        return end_;
    }

private:
    Iterator begin_, end_;
};

template <class Iterator>
class RepeatIterator {
public:
    RepeatIterator(Iterator begin, Iterator end, size_t count = 0)
        : begin_{begin}, it_{begin}, end_{end}, count_{count} {
    }

    RepeatIterator& operator++() {
        if (!(++it_ != end_)) {
            ++count_;
            it_ = begin_;
        }
        return *this;
    }

    auto operator*() const {
        return *it_;
    }

    bool operator==(const RepeatIterator& rhs) const = default;

private:
    Iterator begin_, it_, end_;
    size_t count_;
};

auto Repeat(const auto& sequence, size_t n) {
    auto begin = sequence.begin();
    auto end = sequence.end();
    if (n && (begin != end)) {
        return Sequence{RepeatIterator{begin, end}, RepeatIterator{begin, end, n}};
    } else {
        return Sequence{RepeatIterator{end, end}, RepeatIterator{end, end}};
    }
}

template <class T>
class RangeIterator {
public:
    RangeIterator(T from, T to, T step) : current_(from), end_(to), step_(step) {
    }

    RangeIterator(T from, T to) : current_(from), end_(to), step_(static_cast<T>(1)) {
    }

    RangeIterator(T to) : current_(static_cast<T>(0)), end_(to), step_(static_cast<T>(1)) {
    }

    RangeIterator& operator++() {
        current_ += step_;
        return *this;
    }

    auto operator*() const {
        return current_;
    }

    bool operator!=(const RangeIterator& rhs) const {
        return current_ < rhs.current_;
    }

private:
    T current_, end_, step_;
};

auto Range(const auto& from, const auto& to, const auto& step) {
    return Sequence(RangeIterator(from, to, step), RangeIterator(to, to, step));
}

auto Range(const auto& to) {
    return Sequence(RangeIterator(to), RangeIterator(to, to));
}

auto Range(const auto& from, const auto& to) {
    return Sequence(RangeIterator(from, to), RangeIterator(to, to));
}

template <class Iterator1, class Iterator2>
class ZipIterator {
public:
    ZipIterator(Iterator1 first1, Iterator1 last1, Iterator2 first2, Iterator2 last2)
        : first1_(first1), last1_(last1), first2_(first2), last2_(last2) {
    }

    ZipIterator& operator++() {
        ++first1_;
        ++first2_;
        return *this;
    }

    auto operator*() const {
        return std::make_pair(*first1_, *first2_);
    }

    bool operator!=(const ZipIterator& rhs) const {
        return (first1_ != rhs.first1_) and (first2_ != rhs.first2_);
    }

private:
    Iterator1 first1_, last1_;
    Iterator2 first2_, last2_;
};

auto Zip(const auto& sequence1, const auto& sequence2) {
    auto begin1 = sequence1.begin();
    auto begin2 = sequence2.begin();
    auto end1 = sequence1.end();
    auto end2 = sequence2.end();
    return Sequence{ZipIterator{begin1, end1, begin2, end2}, ZipIterator{end1, end1, end2, end2}};
}

template <class Iterator>
class GroupIterator {
public:
    GroupIterator(Iterator current, Iterator end)
        : current_{current}, end_{end}, current_group_(current_) {
        if (current_ != end_) {
            while (current_ != end_ and *current_ == *current_group_) {
                ++current_;
            }
        }
    }

    GroupIterator& operator++() {
        current_group_ = current_;
        while (current_ != end_ && *current_ == *current_group_) {
            ++current_;
        }
        return *this;
    }

    auto operator*() const {
        return Sequence(current_group_, current_);
    }

    bool operator!=(const GroupIterator& rhs) const {
        return current_group_ != rhs.current_group_;
    }

private:
    Iterator current_, end_, current_group_;
};

auto Group(const auto& sequence) {
    return Sequence(GroupIterator(sequence.begin(), sequence.end()),
                    GroupIterator(sequence.end(), sequence.end()));
}
