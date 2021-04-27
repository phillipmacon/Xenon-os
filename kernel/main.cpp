#include <cpu/cpuinfo.h>
#include <cpu/paging.h>
#include <print.h>
#include <types.h>
#include <stivale/stivale2.h>

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

// extern "C" void kmain(void* multiboot_structure, u32 multiboot_magic)
extern "C" void kmain(stivale2_struct* boot_info)
{
    // if(multiboot_magic != 0x36d76289) {
    //     printk("Multiboot magic invalid! Halting.\n");
    //     while(1) { __asm("hlt"); }
    // }
    
    printk("Bootloader brand string: %s\n", boot_info->bootloader_brand);
    printk("Bootloader version string: %s\n", boot_info->bootloader_version);

    printk("Initialising kernel\n");
    
    callConstructors();

    cpu::info::printProcessorInfo();

    u32* fb{};
    u32 fbWidth{}, fbHeight{};

    size_t memsize{}, memsize_usable{};

    // TODO: Move this to arch (or arch-shared for aarch64)
    stivale2_tag* tag = (stivale2_tag*)boot_info->tags;
    int i = 0;
    while(tag) {
        switch(tag->identifier) {
        case STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID: {
            printk("Found framebuffer tag\n");
            stivale2_struct_tag_framebuffer* fbtag = reinterpret_cast<stivale2_struct_tag_framebuffer*>(tag);
            fb = (u32*)fbtag->framebuffer_addr;
            fbWidth = fbtag->framebuffer_width;
            fbHeight = fbtag->framebuffer_height;

            printk("FB addr: 0x%x\n", fbtag->framebuffer_addr);
            break;
        }
        case STIVALE2_STRUCT_TAG_MEMMAP_ID: {
            printk("Found memory map\n");
            stivale2_struct_tag_memmap* memmaptag = reinterpret_cast<stivale2_struct_tag_memmap*>(tag);
            stivale2_mmap_entry* memmap = memmaptag->memmap;
            for(u64 i = 0; i < memmaptag->entries; i++) {
                printk("\tMemmap #%i: Base 0x%x Size 0x%x Type 0x%x\n", i, memmap[i].base, memmap[i].length, memmap[i].type);
                memsize += memmap[i].length;
                if(memmap[i].type == STIVALE2_MMAP_USABLE)
                    memsize_usable += memmap[i].length;
            }
            break;
        }
        // case STIVALE2_STRUCT_TAG_RSDP_ID: {
        //     stivale2_struct_tag_rsdp* rsdptag = reinterpret_cast<stivale2_struct_tag_rsdp*>(tag);
        //     printk("RSDP @ 0x%x\n", rsdptag->rsdp);
        //     break;
        // }
        }

        tag = (stivale2_tag*)tag->next;
        ++i;
    }

    printk("%iMB memory detected, %iMB available\n", util::bytesToMb(memsize), util::bytesToMb(memsize_usable));
    printk("%iKB for bitmap\n", util::bytesToKb(memsize / 4096 / 8));

    // mem::physmem::initialise(memsize);
    // mem::virtmem::initialise();
    
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
