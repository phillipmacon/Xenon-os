#include <cpu/cpuinfo.h>
#include <print.h>
#include <types.h>

#include "mem/mem.h"

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

    while(1) { __asm("hlt"); }
}
