#pragma once

#include <cstddef>
#include <utility>
#include <vector>

namespace mystl {

template<typename T>
class vector {
private:
    std::size_t length_ {};
    std::size_t capacity_ {};
    T* buffer_;

public:

    explicit vector(const std::size_t capacity)
            : capacity_(capacity)
            , buffer_(static_cast<T*>(::operator new(sizeof(T) * capacity))) {

    }

    ~vector() noexcept {
        for (int i {length_}; i > 0; --i) {
            buffer_[i].~T();
        }
        ::operator delete(buffer_, std::nothrow);
    }

    // Copy Semantics

    vector(const vector& rhs)
        : capacity_(rhs.capacity_)
        , length_(0)
        , buffer_(static_cast<T*>(::operator new(sizeof(T) * rhs.capacity_))) {
        for (int i {0}; i < rhs.length_; ++i) {
            push_back(rhs.buffer_[i]);
        }
    }

    vector& operator=(const vector& rhs) {
        // copy and swap
        vector temp(rhs);
        temp.swap(*this);
        return *this;
    }

    // Move Semantics

    vector(vector&& rhs) noexcept
        : capacity_(0)
        , length_(0)
        , buffer_(nullptr) {
        rhs.swap(*this);
    }

    vector& operator=(vector&& rhs) noexcept {
        rhs.swap(*this);
        return *this;
    }

    // Member functions

    void push_back(const T& val) {
        new (buffer_ + length_) T(val);
        ++length_;
    }

private:
    void swap(vector<T>& rhs) noexcept {
        using std::swap;
        swap(capacity_, rhs.capacity_);
        swap(length_, rhs.length_);
        swap(buffer_, rhs.buffer_);
    }

};

} // namespace mystl