#pragma once
#include <types.h>

namespace util {
    template <typename T>
    class Array {
    private:
        T* data;
        size_t length;
    public:
        Array() : data(nullptr), length(0) {}
        Array(T* _data, size_t _length) : data(_data), length(_length) {}
        
        // TODO: Use optionals to prevent NPD?
        T& operator[](size_t idx) { return data[idx]; }

        T* begin() { return data; }
        T* end() { return data + length; }
    };
}