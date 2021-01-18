#include <cpu/cpuinfo.h>
#include <cpu/paging.h>
#include <print.h>
#include <types.h>

#include "mem/mem.h"
#include "xenonlogo.binh"

extern "C" void kmain(void* multiboot_structure, u32 multiboot_magic)
{
    if(multiboot_magic != 0x36d76289) {
        puts("Multiboot magic invalid! Halting.\n");
        while(1) { __asm("hlt"); }
    }

    puts("Initialising kernel\n");
    
    cpu::info::printProcessorInfo();

    // Leave memory size at 0 until we start using UEFI
    mem::physmem::initialise(0);

    // Find the framebuffer tag
    u32* mb = (u32*)multiboot_structure;
    mb += 2;

    addr fbAddr{};
    u32 fbWidth, fbHeight, fbPitch;
    u32 type = *(u32*)mb;
    while(type != 0) {
        puts("Found tag: ");
        putx(type);
        puts("\n");

        if(type == 8) {
            u32* mbSave = mb + 2;
            fbAddr = *(u64*)mbSave;
            fbPitch = mbSave[2];
            fbWidth = mbSave[3];
            fbHeight = mbSave[4];
        }

        u32 mbTagSize = *(mb + 1);
        mbTagSize = (mbTagSize + (8 - 1)) & -8;
        mb = (u32*)((addr)mb + mbTagSize);

        type = *mb;
    }

    if(fbAddr == 0) {
        puts("Framebuffer not found! Halting.\n");
        while(1) { __asm("hlt"); }
    }

    puts("Found fbaddr 0x");
    putx(fbAddr);
    puts("\n");

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
