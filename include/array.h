#pragma once
#include <types.h>

namespace util {
    template <typename T>
    class Array {
    private:
        T* data;
        size_t length;
    public:
        Array() : length(0), data(nullptr) {}
        Array(T* _data, size_t _length) : length(_length), data(_data) {}
        
        T& operator[](size_t idx) { return data[idx]; }

        T* begin() { return data; }
        T* end() { return data + length; }
    };
}