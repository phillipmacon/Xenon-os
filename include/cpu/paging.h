#pragma once
#include <types.h>

namespace cpu::paging {
    // Set up kernel page table entries
    void initialisePageTable();
    
    // Load the page table and start using it right away
    void loadPageTable();

    addr mapPagesCrap(addr phys, size_t len, u16 l4, u16 l3, u16 l2, u16 l1);
}