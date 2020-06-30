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

    u8 in8(u16 portn) {
        u8 value;
        asm volatile("inb %1, %0" : "=a" (value) : "Nd" (portn));
        return value;
    }

    u16 in16(u16 portn) {
        u16 value;
        asm volatile("inw %1, %0" : "=a" (value) : "Nd" (portn));
        return value;
    }

    u32 in32(u16 portn) {
        u32 value;
        asm volatile("inl %1, %0" : "=a" (value) : "Nd" (portn));
        return value;
    }
}