#pragma once

#include <algorithm>
#include <cstddef>
#include <new>
#include <utility>

namespace mystl {

template <typename T>
concept NoThrowMoveConstructible = std::is_nothrow_constructible_v<T>;

template<typename T>
class vector {
    static_assert(!std::is_same_v<bool, T>, "vector<bool> is not supported");

private:
    size_t length_ {};
    size_t capacity_ {};
    T* buffer_;

public:

    explicit vector(const size_t capacity = 10)
            : capacity_(capacity)
            , buffer_(static_cast<T*>(::operator new(sizeof(T) * capacity))) {}

    ~vector() {
        for (int i {length_}; i > 0; --i) {
            buffer_[i].~T();
        }
        ::operator delete(buffer_);
    }

    // Copy Semantics

    vector(const vector& rhs)
        : capacity_(rhs.capacity_)
        , length_(0)
        , buffer_(static_cast<T*>(::operator new(sizeof(T) * rhs.capacity_))) {
        try {
            for (int i {0}; i < rhs.length_; ++i) {
                push_back(rhs.buffer_[i]);
            }
        } catch(...) {
            for (int i {length_}; i > 0; --i) {
                buffer_[i].~T();
            }
            throw;
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
        resizeIfRequired();
        pushBackInternal(val);
    }

    void swap(vector& rhs) noexcept {
        using std::swap;
        swap(capacity_, rhs.capacity_);
        swap(length_, rhs.length_);
        swap(buffer_, rhs.buffer_);
    }

private:
    void resizeIfRequired() {
        if (length_ == capacity_) {
            const size_t newCapacity = 2.0 * capacity_;
            // copy and swap
            vector<T> temp(newCapacity);
            moveOrCopy<T>(temp);

        }
    }

    template <NoThrowMoveConstructible U>
    void moveOrCopy(vector& dest) {
        // do a move when resizing
        std::for_each(buffer_, buffer_ + length_, [&dest](T &item) {
            dest.moveBackInternal(std::move(item));
        });
    }

    template <typename U>
    void moveOrCopy(vector& dest) {
        // do a copy when resizing
        std::for_each(buffer_, buffer_ + length_, [&dest](T &item) {
            dest.pushBackInternal(item);
        });
    }

    void pushBackInternal(const T& val) {
        new (buffer_ + length_) T(val);
        ++length_;
    }

    void moveBackInternal(T&& val) {
        new (buffer_ + length_) T(std::move(val));
        ++length_;
    }

};

} // namespace mystl