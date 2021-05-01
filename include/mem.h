#include <types.h>

namespace util {
    inline void* memset(void* ptr, u8 value, size_t num);
    inline void* memcpy(void* dst, const void* src, size_t num);
}