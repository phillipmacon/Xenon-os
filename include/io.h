#pragma once
#include <types.h>

namespace io {
    void out8(u16 portn, u8 value);
    void out16(u16 portn, u16 value);
    void out32(u16 portn, u32 value);
}