#include <io.h>

namespace io {
    void out8(u16 portn, u8 value) {
        asm volatile("outb %0, %1" : : "a" (value), "Nd" (portn));
    }

    void out16(u16 portn, u16 value) {
        asm volatile("outw %0, %1" : : "a" (value), "Nd" (portn));
    }

    void out32(u16 portn, u32 value) {
        asm volatile("outl %0, %1" : : "a" (value), "Nd" (portn));
    }
}