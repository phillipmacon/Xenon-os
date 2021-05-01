#pragma once
#include <types.h>

enum class IdtType : u8 {
    InterruptGate = 0b10001110,
    CallGate      = 0b10001100,
    TrapGate      = 0b10001111,
};

struct [[gnu::packed]] IDTEntry {
    u16 offset_low;
    u16 selector;
    u8 zero1;
    IdtType type;
    u16 offset_mid;
    u32 offset_high;
    u32 zero2;

    void setOffset(u64 offset) {
        offset_low  = (u16)((offset >> 0 ) & 0xffff);
        offset_mid  = (u16)((offset >> 16) & 0xffff);
        offset_high = (u32)((offset >> 32) & 0xffffffff);
    }
};

static_assert(sizeof(IDTEntry) == 16,  "Type IDTEntry does not have the right size");

struct [[gnu::packed]] IDTR {
    u16 limit;
    u64 offset;
};