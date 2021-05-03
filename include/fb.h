#pragma once
#include <types.h>

// TODO: Move this file somewhere else

namespace video::fb {
    void init(void* ptr, size_t width, size_t height);
    void putc(char c);
    void puts(const char* s);
}