#pragma once
#include <types.h>

namespace cpu::paging {
    namespace constants {
        constexpr u32 pageSize = 0x1000;
        constexpr u32 pageSize2MiB = 0x200000;
        constexpr u8 pageShiftBits = 12;

        constexpr addr kernelVma = 0xffffffff80000000;
    };

    struct PageTableEntry {
        bool present : 1;
        bool writable : 1;
        bool user : 1;
        bool writeThrough : 1;
        bool cacheDisabled : 1;
        bool accessed : 1;
        bool dirty : 1;
        bool pageSize : 1;
        bool global : 1;

        u8 available : 3;
        addr address : 52;
    };

    struct PageDirectoryEntry {
        bool present : 1;
        bool writable : 1;
        bool user : 1;
        bool writeThrough : 1;
        bool cacheDisabled : 1;
        bool accessed : 1;
        bool : 1;
        bool global : 1;
        bool : 1;

        u8 available : 3;
        addr address : 52;
    };

    #pragma pack(push, 1)
    union VirtualAddr { 
        struct {    
            u16 offset : 12;
            u16 l1 : 9;
            u16 l2 : 9;
            u16 l3 : 9;
            u16 l4 : 9;
            u32 signExtend : 16;
        };

        addr x;
    };
    #pragma pack(pop)

    struct PageDirectoryL4 {
        PageDirectoryEntry entries[512];
    } __attribute__((aligned(0x1000)));

    struct PageDirectoryL3 {
        PageDirectoryEntry entries[512];
    } __attribute__((aligned(0x1000)));

    struct PageDirectoryL2 {
        PageDirectoryEntry entries[512];
    } __attribute__((aligned(0x1000)));

    struct PageDirectoryL1 {
        PageTableEntry entries[512];
    } __attribute__((aligned(0x1000)));
}