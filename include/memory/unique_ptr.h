#pragma once

#include <utility>

namespace mystl {

template<typename T>
class unique_ptr {
    T* data_;
public:
    explicit unique_ptr(T* data)
        : data_(data) {}

    ~unique_ptr() {
        delete data_;
    }

    // Copy Semantics

    unique_ptr(const unique_ptr& rhs) = delete;

    unique_ptr& operator=(const unique_ptr& rhs) = delete;

    // Move Semantics

    unique_ptr(unique_ptr&& rhs) noexcept : data_(nullptr) {
        rhs.swap(*this);
    }

    unique_ptr& operator=(unique_ptr&& rhs) noexcept {
        rhs.swap(*this);
        return *this;
    }

    // Member Functions

    T* release() noexcept {
        T* resp { nullptr };
        std::swap(resp, data_);
        return resp;
    }

    void swap(unique_ptr& other) noexcept {
        std::swap(data_, other.data_);
    }

    T* get() const {
        return data_;
    }

    T* operator->() const {
        return data_;
    }

    T& operator*() const {
        return *data_;
    }

    explicit operator bool() const {
        return data_;
    }
};

template<typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args) {
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

} // namespace mystl