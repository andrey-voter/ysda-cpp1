#pragma once

#include <string>

class WeakPtr;

struct Block {
    int strong = 0;
    int weak = 0;
};

class SharedPtr {
public:
    friend WeakPtr;

    SharedPtr() = default;

    SharedPtr(const WeakPtr& ptr);

    SharedPtr(std::string* ptr) : obj_(ptr), counter_(new Block{1, 0}) {
    }

    SharedPtr(const SharedPtr& other) : obj_(other.obj_), counter_(other.counter_) {
        if (counter_) {
            ++(counter_->strong);
        }
    }

    SharedPtr(SharedPtr&& other) : obj_(other.obj_), counter_(other.counter_) {
        other.counter_ = nullptr;
        other.obj_ = nullptr;
    }

    SharedPtr& operator=(SharedPtr other) {
        std::swap(obj_, other.obj_);
        std::swap(counter_, other.counter_);
        return *this;
    }

    ~SharedPtr() {
        if (!counter_) {
            return;
        }

        if (--(counter_->strong) == 0) {
            delete obj_;
            if (counter_->weak == 0) {
                delete counter_;
            }
        }
    }

    void Reset(std::string* ptr) {
        if (ptr == obj_) {
            return;
        }
        if (counter_) {
            if (--counter_->strong == 0) {
                delete counter_;
                delete obj_;
            }
        }
        obj_ = ptr;
        counter_ = new Block{1, 0};
    }

    std::string& operator*() const {
        return *obj_;
    }

    std::string* operator->() const {
        return obj_;
    }

    std::string* Get() {
        return obj_;
    }

private:
    std::string* obj_ = nullptr;
    Block* counter_ = nullptr;
};

class WeakPtr {
public:
    friend SharedPtr;

    WeakPtr(const SharedPtr& ptr) : obj_(ptr.obj_), counter_(ptr.counter_) {
        if (counter_) {
            ++counter_->weak;
        }
    }

    WeakPtr() = default;

    WeakPtr(WeakPtr&& moved) : obj_(moved.obj_), counter_(moved.counter_) {
        moved.counter_ = nullptr;
        moved.obj_ = nullptr;
    }

    WeakPtr(const WeakPtr& ptr) : obj_(ptr.obj_), counter_(ptr.counter_) {
        if (counter_) {
            ++counter_->weak;
        }
    }

    WeakPtr& operator=(WeakPtr other) {
        std::swap(obj_, other.obj_);
        std::swap(counter_, other.counter_);
        return *this;
    }

    bool Alive() const {
        if (counter_) {
            return counter_->strong > 0;
        }
        return false;
    }

    bool IsExpired() {
        return !Alive();
    }

    SharedPtr Lock() {
        SharedPtr res;
        if (Alive()) {
            res.obj_ = obj_;
            res.counter_ = counter_;
            ++counter_->strong;
        }
        return res;
    }

    std::string* Get() const {
        return obj_;
    }

    std::string* operator->() const {
        return obj_;
    }

    ~WeakPtr() {
        if (counter_ && --counter_->weak == 0 && counter_->strong == 0) {
            delete counter_;
        }
    }

private:
    std::string* obj_ = nullptr;
    Block* counter_ = nullptr;
};

SharedPtr::SharedPtr(const WeakPtr& ptr) {
    if (ptr.Alive()) {
        obj_ = ptr.obj_;
        counter_ = ptr.counter_;
        ++counter_->strong;
    }
}
