#include <cpu/paging.h>
#include <print.h>
#include "mem.h"

extern addr _start;
extern addr _end;

namespace mem::physmem {
    void initialise(u64 memSize) {
        cpu::paging::initialisePageTable();

        // add kernel code to page table
        // FIXME: replace this with something that's actually good and doesn't look like it came directly out of satan's asshole
        cpu::paging::mapPagesCrap(0, &_end - &_start, 0, 0, 0, 0);

        cpu::paging::loadPageTable();
    }
}