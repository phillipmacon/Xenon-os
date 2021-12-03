#pragma once
#include <types.h>

struct GDTEntry {
    u16 base_low;
    u16 limit_low;
    u8 base_high;
    u8 limit_high_flags;
    u8 access;
    u8 base_mid;
};

static_assert(sizeof(GDTEntry) == 8,  "Type GDTEntry does not have the right size");

struct GDTR {
    u16 limit;
    u64 offset;
} __attribute((packed));

namespace cpu::gdt {
    void init();
}