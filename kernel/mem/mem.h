#pragma once
#include <types.h>

namespace util {
    class Bitmap {
    private:
        u64* bits;
        size_t data_size;

    public:
        // Size is in amount of u64's
        Bitmap(u64* data, size_t size) : bits(data), data_size(size) {}

        void set(size_t index);
        void set(size_t index, size_t count);
        void unset(size_t index);
        u8 get(size_t index);

        void resize(size_t size);
        void clear();

        size_t getBlock(size_t count);

        // Gets the first zero bit offset
        size_t getNextZero();

        size_t bitcount() { return data_size * 64; }
    };

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

    inline u64 kbToBytes(u64 kb) { return kb << 10; }
    inline u64 mbToBytes(u64 mb) { return mb << 20; }
    inline u64 gbToBytes(u64 gb) { return gb << 30; }
    inline u64 tbToBytes(u64 tb) { return tb << 40; }

    inline u64 bytesToKb(u64 b) { return b >> 10; }
    inline u64 bytesToMb(u64 b) { return b >> 20; }
    inline u64 bytesToGb(u64 b) { return b >> 30; }
    inline u64 bytesToTb(u64 b) { return b >> 40; }
}

namespace mem::constants {
    constexpr u32 pageSize = 0x1000;
}

namespace mem::physmem {
    void initialise(u64 memSize);
}

namespace mem::virtmem {
    void initialise();
}