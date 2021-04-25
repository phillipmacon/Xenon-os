#include <cpu/cpuinfo.h>
#include <cpu/paging.h>
#include <print.h>
#include <types.h>

#include "mem/mem.h"
#include "xenonlogo.binh"

typedef void (*constructor)();
extern constructor __CTOR_LIST__;
extern constructor __CTOR_END__;
void callConstructors()
{
    for(constructor* i = &__CTOR_LIST__; i != &__CTOR_END__; i++)
        (*i)();
}

extern "C" void kmain(void* multiboot_structure, u32 multiboot_magic)
{
    if(multiboot_magic != 0x36d76289) {
        printk("Multiboot magic invalid! Halting.\n");
        while(1) { __asm("hlt"); }
    }

    printk("Initialising kernel\n");
    
    callConstructors();

    cpu::info::printProcessorInfo();

    // Find the framebuffer tag
    u32* mb = (u32*)multiboot_structure;
    mb += 2;

    struct MemMapEntry {
        u64 base_addr;
        u64 size;
        u32 type;
        u32 reserved;
    }* memory_map;
    u32 memory_map_length{};

    addr fbAddr{};
    u32 fbWidth, fbHeight, fbPitch;
    u32 type = *(u32*)mb;
    while(type != 0) {
        // printk("Found tag: 0x%x\n", type);

        // Memory Map
        if(type == 6) {
            u32* mbSave = mb;
            if(mbSave[3] != 0) {
                printk("Memory map has an unsupported entry version!\n");
                while(1) { __asm("hlt"); }
            }

            memory_map = reinterpret_cast<MemMapEntry*>(&mbSave[4]);
            memory_map_length = (mbSave[1] - 16) / sizeof(MemMapEntry);
            printk("Found memory map, %i entries\n", memory_map_length);
        }

        // Framebuffer
        if(type == 8) {
            u32* mbSave = mb + 2;
            fbAddr = *(u64*)mbSave;
            fbPitch = mbSave[2];
            fbWidth = mbSave[3];
            fbHeight = mbSave[4];

            printk("Found fbaddr 0x%x\n", fbAddr);
        }

        u32 mbTagSize = *(mb + 1);
        mbTagSize = (mbTagSize + (8 - 1)) & -8;
        mb = (u32*)((addr)mb + mbTagSize);

        type = *mb;
    }

    if(memory_map == nullptr || memory_map_length == 0) {
        printk("Memory map not found! Halting.\n");
        while(1) { __asm("hlt"); }
    }

    if(fbAddr == 0) {
        printk("Framebuffer not found! Halting.\n");
        while(1) { __asm("hlt"); }  
    }

    size_t memory_size{}, memory_size_available{};
    for(u32 i = 0; i < memory_map_length; i++) {
        MemMapEntry e = memory_map[i];
        const static char* map_types[] = {"Reserved", "Available", "Reserved", "ACPI", "PreserveOnHibernate"};
        printk("MM Entry #%i - Base: 0x%x Size: 0x%x (%iKB) Type: %s\n", i, e.base_addr, e.size, util::bytesToKb(e.size), map_types[e.type]);

        if(e.type == 1)
            memory_size_available += e.size;

        memory_size += e.size;
    }

    mem::physmem::initialise(memory_size);
    mem::virtmem::initialise();

    printk("%iMB memory detected, %iMB available\n", util::bytesToMb(memory_size)+1, util::bytesToMb(memory_size_available)+1);
    printk("%iKB for bitmap\n", util::bytesToKb(memory_size / 4096 / 8)+1);

    // Called "Crap" for a reason
    addr fbVAddr = cpu::paging::mapPagesCrap(fbAddr, fbHeight * fbPitch, 0, 0, 1, 0);
    u32* fb = (u32*)fbVAddr;

    u32 midX = fbWidth / 2, midY = fbHeight / 2;
    u32 midLogo = (midY - 190) * fbWidth + (midX - 190);

    for(u32 y = 0; y < 380; y++) {
        for(u32 x = 0; x < 380; x++) {
            u8 p = _xenonLogo[y * 380 + x];
            fb[midLogo + y * fbWidth + x] = (p << 16) | (p << 8) | p;
        }
    }

    while(1) { __asm("hlt"); }
}
