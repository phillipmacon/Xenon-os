#pragma once
#include <types.h>

namespace util {
    inline void* memset(void* ptr, u8 value, size_t num);
    inline void* memcpy(void* dst, const void* src, size_t num);
}

namespace mem::constants {
    constexpr u32 pageSize = 0x1000;
    constexpr u32 pageSize2MiB = 0x200000;

    constexpr addr kernelBase = 0xffff800000000000;
    constexpr addr kernelVma = 0xffffffff80200000;
}