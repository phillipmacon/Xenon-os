#include <cpu/cpuinfo.h>
#include <cpu/paging.h>
#include <print.h>
#include <types.h>
#include <stivale/stivale2.h>
#include <array.h>

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

u32 hueTable[] = {
    0xff0000,
    0xff5f00,
    0xffbf00,
    0xdfff00,
    0x7fff00,
    0x1fff00,
    0x00ff3f,
    0x00ff9f,
    0x00ffff,
    0x009fff,
    0x003fff,
    0x1f00ff,
    0x7f00ff,
    0xdf00ff,
    0xff00bf,
    0xff005f,
};

u32 fbBack[1024 * 768];

extern "C" void kmain(stivale2_struct* boot_info)
{
    printk("Bootloader brand string: %s\n", boot_info->bootloader_brand);
    printk("Bootloader version string: %s\n", boot_info->bootloader_version);

    printk("Initialising kernel\n");
    
    // callConstructors();

    cpu::info::printProcessorInfo();

    u32* fb{};
    u32 fbWidth{}, fbHeight{};

    size_t memsize{}, memsize_usable{};

    // TODO: Move this to arch (or arch-shared for aarch64)
    stivale2_tag* tag = (stivale2_tag*)boot_info->tags;
    util::Array<stivale2_mmap_entry> memmap;
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

            int i = 0;
            memmap = util::Array<stivale2_mmap_entry>(memmaptag->memmap, memmaptag->entries);
            
            for(const auto& region : memmap) {
                printk("\tMemmap #%i: Base 0x%x Size 0x%x Type 0x%x\n", i++, region.base, region.length, region.type);
                memsize += region.length;
                if(region.type == STIVALE2_MMAP_USABLE)
                    memsize_usable += region.length;
            }
            break;
        }
        }

        tag = (stivale2_tag*)tag->next;
    }

    printk("%iMB memory detected, %iMB available\n", util::bytesToMb(memsize), util::bytesToMb(memsize_usable));
    printk("%iKB for bitmap\n", util::bytesToKb(memsize / 4096 / 8));

    mem::physmem::initialise(memmap, memsize);
    // mem::virtmem::initialise();

    util::memset(fb, 0x00, fbWidth * fbHeight * 4);
    
    u32 midX = fbWidth / 2, midY = fbHeight / 2;
    u32 midLogo = (midY - 190) * fbWidth + (midX - 190);

    int fi = 0;
    while(1) {
        for(u32 y = 0; y < fbHeight; y++) {
            for(u32 x = 0; x < fbWidth; x++) {
                fbBack[y * fbWidth + x] = hueTable[fi];
            }
        }

        if(++fi >= 16)
            fi = 0;

        for(u32 y = 0; y < 380; y++) {
            for(u32 x = 0; x < 380; x++) {
                u8 p = _xenonLogo[y * 380 + x];
                fbBack[midLogo + y * fbWidth + x] |= (p << 16) | (p << 8) | p;
            }
        }

        util::memcpy(fb, fbBack, 1024 * 768 * sizeof(u32));
    }

    while(1) { __asm("hlt"); }
}
