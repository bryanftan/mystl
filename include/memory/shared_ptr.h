#include <utility>
#include <atomic>

#pragma once

namespace mystl {

template <typename T>
class shared_ptr {
private:
    T *data_;
    std::atomic<unsigned int> *count_;

public:
    explicit shared_ptr(T *data)
            : data_(data)
            , count_(new std::atomic<unsigned int>(1)) {}

    ~shared_ptr() {
        (*count_)--;
        if (!(*count_))
            delete data_;
    }

    // Copy Semantics

    shared_ptr(const shared_ptr &rhs)
            : data_(rhs.data_)
            , count_(rhs.count_) {
        ++(*count_);
    }

    shared_ptr &operator=(const shared_ptr &rhs) {
        // Copy and swap
        auto temp {rhs};
        swap(temp);
        return *this;
    }

    // Move Semantics

    shared_ptr(shared_ptr &&rhs)
            : data_(nullptr)
            , count_(nullptr) {
        swap(rhs);
    }

    shared_ptr &operator=(shared_ptr &&rhs) {
        rhs.swap(*this);
        return *this;
    }

    // Member Functions

    T *get() const noexcept {
        return data_;
    }

    T *operator->() const noexcept {
        return data_;
    }

    T operator*() const {
        return *data_;
    }

    explicit operator bool() {
        return data_;
    }

    void swap(shared_ptr &rhs) noexcept {
        using std::swap;
        swap(data_, rhs.data_);
        swap(count_, rhs.count_);
    }
};

} // namspace mystl