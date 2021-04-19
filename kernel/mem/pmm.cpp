#include <cpu/paging.h>
#include <print.h>
#include "mem.h"

namespace util {
    void Bitmap::set(size_t index) {
        bits[index / 64] |= (1 << (index % 64));
    }

    void Bitmap::unset(size_t index) {
        bits[index / 64] &= ~(1 << (index % 64));
    }

    u8 Bitmap::get(size_t index) {
        return bits[index / 64] & (1 << (index % 64));
    }

    u64 Bitmap::getNextZero() {
        for(size_t i = 0; i < data_size; i++) {
            // Skip block if the entire block is taken
            if(bits[i] == ~0ULL) continue;

            // Iterate through block
            for(size_t ib = 0; ib < 64; ib++) {
                if(get(data_size * 64 + ib) == 0)
                    return data_size * 64 + ib;
            }
        }
    }
}


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