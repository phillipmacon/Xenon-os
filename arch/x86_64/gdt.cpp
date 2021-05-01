#include <mem.h>
#include <print.h>
#include "gdt.h"

__attribute__((aligned(0x1000)))
GDTEntry gdt_entries[] = {
    {0, 0, 0, 0x00, 0x00, 0},
    {0, 0, 0, 0x9a, 0xa0, 0},
    {0, 0, 0, 0x92, 0xa0, 0},
};

GDTR gdtr;

extern "C" void loadGdt(u64 ptr);

namespace cpu::gdt {
    void init() {
        gdtr.limit = sizeof(gdt_entries) - 1;
        gdtr.offset = (u64)&gdt_entries;

        loadGdt((u64)&gdtr);
        printk("GDT Loaded\n");
    }
}