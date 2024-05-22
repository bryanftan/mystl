#pragma once

#include <cstddef>
#include <vector>

namespace mystl {

template<typename T>
class vector {
private:
    std::size_t _length {};
    std::size_t _capacity {};
    T* _buffer;

public:

    explicit vector(const std::size_t capacity)
            : _capacity(capacity)
            , _buffer(static_cast<T*>(::operator new(sizeof(T) * capacity))) {}

    ~vector() {
        for (int i {_length}; i > 0; --i) {
            _buffer[i].~T();
        }
        ::operator delete(_buffer);
    }

    // Copy Semantics

    vector(const vector<T>& rhs)
        : _capacity(rhs._capacity),
        _length(0),
        _buffer(static_cast<T*>(::operator new(sizeof(T) * rhs._capacity))) {
        for (int i {0}; i < rhs._length; ++i) {
            push_back(rhs._buffer[i]);
        }
    }

    vector<T>& operator=(const vector<T>& rhs) {
        // copy and swap
        return *this;
    }

    // Move Semantics

    vector(T&& rhs) {}

    vector operator=(T&& rhs) {}

    // Member functions

    void push_back(const T& val) {
        new (_buffer + _length) T(val);
        ++_length;
    }

};

} // namespace mystl