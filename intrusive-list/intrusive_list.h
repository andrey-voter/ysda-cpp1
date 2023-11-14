#pragma once

#include <algorithm>
#include <cstddef>

class ListHook {
public:
    template <class T>
    friend class List;

    bool IsLinked() const {
        return prev_ != nullptr && next_ != nullptr;
    }

    void Unlink() {
        prev_->next_ = next_;
        next_->prev_ = prev_;
        prev_ = nullptr;
        next_ = nullptr;
    }

    ListHook(const ListHook&) = delete;
    ListHook& operator=(const ListHook&) = delete;

protected:
    ListHook() = default;

    // Must unlink element from list
    ~ListHook() {
        if (prev_) {
            prev_->next_ = next_;
        }
        if (next_) {
            next_->prev_ = prev_;
        }
        prev_ = next_ = nullptr;
    }

private:
    ListHook* prev_ = nullptr;
    ListHook* next_ = nullptr;
};

template <class T>
class List {
public:
    class Iterator {
    public:
        friend class List<T>;
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator() = default;
        Iterator(const Iterator&) = default;
        Iterator(T* ptr) : ptr_(ptr) {
        }

        Iterator& operator=(const Iterator&) = default;

        Iterator& operator++() {
            ptr_ = Get(ptr_->next_);
            return *this;
        }

        Iterator operator++(int) {
            Iterator it = *this;
            ++*this;
            return it;
        }

        Iterator& operator--() {
            ptr_ = Get(ptr_->prev_);
            return *this;
        }

        Iterator operator--(int) {
            Iterator it = *this;
            --*this;
            return it;
        }

        T& operator*() const {
            return *ptr_;
        }

        T* operator->() const {
            return ptr_;
        }

        friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
            return lhs.ptr_ == rhs.ptr_;
        }

    private:
        T* ptr_;
    };

    class ConstIterator {
    public:
        friend class List<T>;
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = const T;
        using difference_type = ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        ConstIterator() = default;
        ConstIterator(const ConstIterator&) = default;
        ConstIterator(const T* ptr) : ptr_(ptr) {
        }
        ConstIterator& operator=(const ConstIterator&) = default;

        ConstIterator& operator++() {
            ptr_ = Get(ptr_->next_);
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator it = *this;
            ++*this;
            return it;
        }

        ConstIterator& operator--() {
            ptr_ = Get(ptr_->prev_);
            return *this;
        }

        ConstIterator operator--(int) {
            ConstIterator it = *this;
            --*this;
            return it;
        }

        const T& operator*() const {
            return *ptr_;
        }

        const T* operator->() const {
            return ptr_;
        }

        friend bool operator==(const ConstIterator& lhs, const ConstIterator& rhs) {
            return lhs.ptr_ == rhs.ptr_;
        }

    private:
        const T* ptr_;
    };

    List() {
        dummy_.prev_ = dummy_.next_ = &dummy_;
    }
    List(const List&) = delete;
    List(List&& other) {
        dummy_.prev_ = other.dummy_.prev_;
        dummy_.next_ = other.dummy_.next_;
        dummy_.prev_->next_ = &dummy_;
        dummy_.next_->prev_ = &dummy_;
        other.dummy_.prev_ = &other.dummy_;
        other.dummy_.next_ = &other.dummy_;
    }

    // must unlink all elements from list
    void UnlinkAll() {
        while (!IsEmpty()) {
            PopBack();
        }
    }

    ~List() {
        UnlinkAll();
    }

    List& operator=(const List&) = delete;
    List& operator=(List&& other) {
        if (&other != this) {
            UnlinkAll();
            dummy_.prev_ = other.dummy_.prev_;
            dummy_.next_ = other.dummy_.next_;
            dummy_.prev_->next_ = &dummy_;
            dummy_.next_->prev_ = &dummy_;
            other.dummy_.prev_ = &other.dummy_;
            other.dummy_.next_ = &other.dummy_;
        }
        return *this;
    }

    bool IsEmpty() const {
        return dummy_.next_ == &dummy_ || dummy_.next_ == nullptr;
    }
    // this method is allowed to be O(n)
    size_t Size() const {
        size_t size = 0;
        for (auto it = Begin(); it != End(); ++it) {
            ++size;
        }
        return size;
    }

    // note that IntrusiveList doesn't own elements,
    // and never copies or moves T
    void PushBack(T* elem) {
        elem->prev_ = dummy_.prev_;
        elem->next_ = &dummy_;
        dummy_.prev_->next_ = elem;
        dummy_.prev_ = elem;
    }
    void PushFront(T* elem) {
        elem->prev_ = &dummy_;
        elem->next_ = dummy_.next_;
        dummy_.next_->prev_ = elem;
        dummy_.next_ = elem;
    }

    T& Front() {
        return *Begin();
    }
    const T& Front() const {
        return *Begin();
    }

    T& Back() {
        return *std::prev(End());
    }
    const T& Back() const {
        return *std::prev(End());
    }

    void PopBack() {
        ListHook* deleted = dummy_.prev_;
        if (dummy_.prev_) {
            if (dummy_.prev_->prev_) {
                dummy_.prev_->prev_->next_ = &dummy_;
            }
            dummy_.prev_ = dummy_.prev_->prev_;
            deleted->next_ = deleted->prev_ = nullptr;
        } else {
            dummy_.next_ = &dummy_;
        }
    }
    void PopFront() {
        ListHook* deleted = dummy_.next_;
        dummy_.next_->next_->prev_ = &dummy_;
        dummy_.next_ = dummy_.next_->next_;
        deleted->next_ = deleted->prev_ = nullptr;
    }

    Iterator Begin() {
        return IteratorTo(reinterpret_cast<T*>(dummy_.next_));
    }
    Iterator End() {
        return IteratorTo(reinterpret_cast<T*>(&dummy_));
    }

    ConstIterator Begin() const {
        return ConstIteratorTo(reinterpret_cast<const T*>(dummy_.next_));
    }
    ConstIterator End() const {
        return ConstIteratorTo(reinterpret_cast<const T*>(&dummy_));
    }
    ConstIterator ConstIteratorTo(const T* element) const {
        return ConstIterator{element};
    }

    // complexity of this function must be O(1)
    Iterator IteratorTo(T* element) {
        return Iterator{element};
    }

private:
    ListHook dummy_;
    static T* Get(ListHook* hook) {
        return static_cast<T*>(static_cast<void*>(hook));
    }
    static const T* Get(const ListHook* hook) {
        return static_cast<const T*>(static_cast<const void*>(hook));
    }
};

template <class T>
List<T>::Iterator begin(List<T>& list) {
    return list.Begin();
}

template <class T>
List<T>::Iterator end(List<T>& list) {
    return list.End();
}

template <class T>
List<T>::ConstIterator begin(const List<T>& list) {
    return list.Begin();
}

template <class T>
List<T>::ConstIterator end(const List<T>& list) {
    return list.End();
}
