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

public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = size_t;

private:
    size_type length_ {};
    size_type capacity_ {};
    pointer buffer_;

public:

    explicit vector(size_type capacity = 10)
            : capacity_(capacity)
            , buffer_(static_cast<pointer>(::operator new(sizeof(value_type) * capacity))) {}

    ~vector() {
        for (size_type i {length_}; i > 0; --i) {
            buffer_[i].~T();
        }
        ::operator delete(buffer_);
    }

    // Copy Semantics

    vector(const vector& rhs)
        : capacity_(rhs.capacity_)
        , length_(0)
        , buffer_(static_cast<pointer>(::operator new(sizeof(value_type) * rhs.capacity_))) {
        try {
            for (size_type i {0}; i < rhs.length_; ++i) {
                push_back(rhs.buffer_[i]);
            }
        } catch(...) {
            for (size_type i {length_}; i > 0; --i) {
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

    void push_back(const_reference val) {
        resizeIfRequired();
        pushBackInternal(val);
    }

    void push_back(value_type&& val) {
        resizeIfRequired();
        moveBackInternal(std::move(val));
    }

    void pop_back() {
        --length_;
        buffer_[length_].~T();
    }

    void swap(vector& rhs) noexcept {
        using std::swap;
        swap(capacity_, rhs.capacity_);
        swap(length_, rhs.length_);
        swap(buffer_, rhs.buffer_);
    }

    // Element access

    reference operator[](size_type idx) {
        return buffer_[idx];
    }

    // Non-mutating functions

    size_type size() const noexcept {
        return length_;
    }

    size_type capacity() const noexcept {
        return capacity_;
    }

    bool empty() const noexcept {
        return length_ == 0;
    }

private:
    void resizeIfRequired() {
        if (length_ == capacity_) {
            const size_type newCapacity = 2.0 * capacity_;
            // copy and swap
            vector<value_type> temp(newCapacity);
            moveOrCopyAll<value_type>(temp);
        }
    }

    template <NoThrowMoveConstructible U>
    void moveOrCopyAll(vector& dest) {
        // do a move when resizing
        std::for_each(buffer_, buffer_ + length_, [&dest](reference item) {
            dest.moveBackInternal(std::move(item));
        });
    }

    template <typename U>
    void moveOrCopyAll(vector& dest) {
        // do a copy when resizing
        std::for_each(buffer_, buffer_ + length_, [&dest](reference item) {
            dest.pushBackInternal(item);
        });
    }

    void pushBackInternal(const_reference val) {
        new (buffer_ + length_) T(val);
        ++length_;
    }

    void moveBackInternal(value_type && val) {
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