#pragma once
#include <types.h>

namespace util {
    // TODO: Keep these functions here, or move them to global scope?
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

    constexpr u64 kbToBytes(u64 kb) { return kb * 1024; }
    constexpr u64 mbToBytes(u64 mb) { return kbToBytes(mb * 1024); }
    constexpr u64 gbToBytes(u64 gb) { return mbToBytes(gb * 1024); }
    constexpr u64 tbToBytes(u64 gb) { return gbToBytes(gb * 1024); }

    constexpr u64 bytesToKb(u64 b) { return b / 1024; }
    constexpr u64 bytesToMb(u64 b) { return bytesToKb(b / 1024); }
    constexpr u64 bytesToGb(u64 b) { return bytesToMb(b / 1024); }
    constexpr u64 bytesToTb(u64 b) { return bytesToGb(b / 1024); }
}

namespace mem::physmem {
    void initialise(u64 memSize);
}