#include <cpu/paging.h>
#include <print.h>
#include "mem.h"

namespace util {
    void Bitmap::set(size_t index) {
        bits[index / 64] |= (1 << (index % 64));
    }

    void Bitmap::set(size_t index, size_t count) {
        for(size_t i = 0; i < count; i++)
            set(index+i);
    }

    void Bitmap::unset(size_t index) {
        bits[index / 64] &= ~(1 << (index % 64));
    }

    u8 Bitmap::get(size_t index) {
        return bits[index / 64] & (1 << (index % 64));
    }
    
    void Bitmap::resize(size_t size) {
        data_size = size;
    }

    void Bitmap::clear() {
        memset(bits, 0, data_size * 8);
    }

    size_t Bitmap::getBlock(size_t count) {
       size_t count2 = count;

        for(int i = 0; i < bitcount(); i++) {
            if(get(i) == 0) {
                if(--count2 == 0) {
                    return i - count;
                } else {
                    count2 = count;
                }
            }
        }

        return -1;
    }

    size_t Bitmap::getNextZero() {
        for(size_t i = 0; i < data_size; i++) {
            // Skip block if the entire block is taken
            if(bits[i] == ~0ULL) continue;

            // Iterate through block
            for(size_t ib = 0; ib < 64; ib++) {
                if(get(i * 64 + ib) == 0)
                    return i * 64 + ib;
            }
        }

        return -1;
    }
}


extern addr _start;
extern addr _end;

static util::Bitmap bitmap(&_end, 0);

namespace mem::physmem {
    void initialise(u64 memsize) {
        cpu::paging::initialisePageTable();
        
        bitmap.resize(memsize / constants::pageSize / 64 + 1);
        bitmap.clear();

        // bitmap.set(0, 512 * 3);
        printk("Bitmap size in u64s: %i\n", memsize / constants::pageSize / 64 + 1);
    }

    static void* alloc(size_t pages) {
        // TODO: Spinlocks
        size_t page_base = bitmap.getBlock(pages);
        if(page_base == -1) {
            // TODO: Panic instead of potentially letting the kernel collapse
            return nullptr;
        }

        bitmap.set(page_base, pages);
        return reinterpret_cast<void*>(page_base * constants::pageSize);
    }

    void* allocz(size_t pages) {
        void* p = alloc(pages);
        if(!p)
            return nullptr;

        util::memset(p, 0, pages * constants::pageSize);
        return p;
    }
}