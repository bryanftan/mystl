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

    explicit vector(size_t capacity = 10)
            : capacity_(capacity)
            , buffer_(static_cast<T*>(::operator new(sizeof(T) * capacity))) {}

    ~vector() {
        for (size_t i {length_}; i > 0; --i) {
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
            for (size_t i {0}; i < rhs.length_; ++i) {
                push_back(rhs.buffer_[i]);
            }
        } catch(...) {
            for (size_t i {length_}; i > 0; --i) {
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

    // Mutating functions

    template <typename... Args>
    void emplace_back(Args&&... args) {
        resizeIfRequired();
        emplaceBackInternal(std::forward<Args>(args)...);
    }

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

    // Non-mutating functions

    size_t size() const { return length_; }
    bool empty() const { return length_ == 0; }

private:
    void resizeIfRequired() {
        if (length_ == capacity_) {
            const size_t newCapacity = 2.0 * capacity_;
            // copy and swap
            vector<T> temp(newCapacity);
            moveOrCopyAll<T>(temp);

        }
    }

    template <NoThrowMoveConstructible U>
    void moveOrCopyAll(vector& dest) {
        // do a move when resizing
        std::for_each(buffer_, buffer_ + length_, [&dest](T &item) {
            dest.moveBackInternal(std::move(item));
        });
    }

    template <typename U>
    void moveOrCopyAll(vector& dest) {
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

    template<typename... Args>
    void emplaceBackInternal(Args&&... args) {
        new (buffer_ + length_) T(std::forward<Args>(args)...);
        ++length_;
    }

};

} // namespace mystl