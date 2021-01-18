#include <cpu/ioport.h>
#include <cpu/paging.h>
#include <print.h>
#include "paging.h"

namespace cpu::paging {
    PageDirectoryL4 pageTableL4;
    PageDirectoryL3 pageTableL3;
    PageDirectoryL2 pageTableL2;

    PageDirectoryL1 pageTableL1_0;
    PageDirectoryL1 pageTableL1_1;
    PageDirectoryL1 pageTableL1_2;

    // Dirty and ugly, but works for now
    void initialisePageTable() {
        auto& pd4Entry = pageTableL4.entries[0];
        pd4Entry.user = 0;
        pd4Entry.writable = 1;
        pd4Entry.present = 1;
        pd4Entry.address = ((addr)&pageTableL3 - constants::kernelVma) >> constants::pageShiftBits;

        pageTableL4.entries[511] = pd4Entry;

        auto& pd3Entry = pageTableL3.entries[0];
        pd3Entry.user = 0;
        pd3Entry.writable = 1;
        pd3Entry.present = 1;
        pd3Entry.address = ((addr)&pageTableL2 - constants::kernelVma) >> constants::pageShiftBits;

        pageTableL3.entries[510] = pd3Entry;

        auto& pd2Entry = pageTableL2.entries[0]; // 0MiB - 2MiB
        pd2Entry.user = 0;
        pd2Entry.writable = 1;
        pd2Entry.present = 1;
        pd2Entry.address = ((addr)&pageTableL1_0 - constants::kernelVma) >> constants::pageShiftBits;

        auto& pd2Entry_ = pageTableL2.entries[1]; // 2MiB - 4MiB
        pd2Entry_.user = 0;
        pd2Entry_.writable = 1;
        pd2Entry_.present = 1;
        pd2Entry_.address = ((addr)&pageTableL1_1 - constants::kernelVma) >> constants::pageShiftBits;

        auto& pd2Entry__ = pageTableL2.entries[2]; // 4MiB - 8MiB
        pd2Entry__.user = 0;
        pd2Entry__.writable = 1;
        pd2Entry__.present = 1;
        pd2Entry__.address = ((addr)&pageTableL1_2 - constants::kernelVma) >> constants::pageShiftBits;
    }

    u64 align(u64 x, u64 alignment=constants::pageSize) {
        addr r = x % alignment;
        return r ? x + (alignment - r) : x; 
    }

    addr mapPagesCrap(addr phys, size_t len, u16 l4, u16 l3, u16 l2, u16 l1) {
        len = align(len, constants::pageSize2MiB);

        VirtualAddr vaddr{};
        vaddr.l4 = l4;
        vaddr.l3 = l3;
        vaddr.l2 = l2;
        vaddr.l1 = l1;

        // FIXME: Efficiency much?
        for(size_t i = 0; i < len; i += constants::pageSize, vaddr.x += constants::pageSize, phys += constants::pageSize) {
            auto& pd4Entry = pageTableL4.entries[vaddr.l4];
            auto& pd3Entry = reinterpret_cast<PageDirectoryL3*>((pd4Entry.address << constants::pageShiftBits) + constants::kernelVma)->entries[vaddr.l3];
            auto& pd2Entry = reinterpret_cast<PageDirectoryL2*>((pd3Entry.address << constants::pageShiftBits) + constants::kernelVma)->entries[vaddr.l2];
            auto& pd1Entry = reinterpret_cast<PageDirectoryL1*>((pd2Entry.address << constants::pageShiftBits) + constants::kernelVma)->entries[vaddr.l1];
    
            pd1Entry.user = 0;
            pd1Entry.writable = 1;
            pd1Entry.present = 1;
            pd1Entry.address = phys >> constants::pageShiftBits;
        }

        vaddr.l4 = l4;
        vaddr.l3 = l3;
        vaddr.l2 = l2;
        vaddr.l1 = l1;
        return vaddr.x;
    }

    void setCR3(addr v) {
        v -= constants::kernelVma;
        __asm("movq %0, %%cr3": :"r" (v));
    }

    void loadPageTable() { setCR3((addr)&pageTableL4); }
}