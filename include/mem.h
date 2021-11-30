#pragma once
#include <types.h>

namespace util {
    // TODO: Move these to a source file?
    inline void* memset(void* ptr, u8 value, size_t num) {
        u8* p = (u8*)ptr;
        for(size_t i = 0; i < num; i++)
            p[i] = value;

        return ptr;
    }

    inline void* memcpy(void* dst, const void* src, size_t num) {
        u8* d = (u8*)dst;
        const u8* s = (u8*)src;
        for(size_t i = 0; i < num; i++)
            d[i] = s[i];

        return dst;
    }
}

namespace mem::constants {
    constexpr u32 pageSize = 0x1000;
    constexpr u32 pageSize2MiB = 0x200000;

    constexpr addr kernelBase = 0xffff800000000000;
    constexpr addr kernelVma = 0xffffffff80200000;
}