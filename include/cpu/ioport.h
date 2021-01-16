#pragma once
#include <types.h>

namespace io {
    void out8(u16 portn, u8 value);
    void out16(u16 portn, u16 value);
    void out32(u16 portn, u32 value);

    u8 in8(u16 portn);
    u16 in16(u16 portn);
    u32 in32(u16 portn);
}