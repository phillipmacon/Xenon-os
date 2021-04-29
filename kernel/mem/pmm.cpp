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

    void Bitmap::clear(bool zero) {
        memset(bits, zero ? 0x00 : 0xff, data_size * 8);
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

static util::Bitmap bitmap;

// TODO: Don't use macros, replace this with an inline function
#define PAGECOUNT(length) ((length / mem::constants::pageSize) + ((length % mem::constants::pageSize) > 0 ? 1 : 0))

namespace mem::physmem {
    void free(addr base, size_t length) {
        // printk("Free-ing %i pages @ 0x%x\n", PAGECOUNT(length), base);
        bitmap.set(base / constants::pageSize, PAGECOUNT(length));
    }

    void initialise(util::Array<stivale2_mmap_entry>& mmap, u64 memSize) {
        void* bitmapPtr{};
        for(const auto& region : mmap) {
            if(region.length >= util::kbToBytes(64) && region.type == STIVALE2_MMAP_USABLE && !bitmapPtr) {
                bitmapPtr = (void*)(region.base + mem::constants::kernelBase);
                printk("Found space for bitmap @ 0x%x, putting it there (0x%x)\n", region.base, bitmapPtr);
            }
        }


        if(!bitmapPtr) {
            // panic("Could not find space for bitmap!");
            printk("Could not find space for bitmap!\n");
            return;
        }

        bitmap = util::Bitmap(reinterpret_cast<u64*>(bitmapPtr), memSize / constants::pageSize / 64 + 1);
        
        // Set all memory as used
        bitmap.clear(false);

        printk("Bitmap size in u64s: %i\n", memSize / constants::pageSize / 64 + 1);

        // Unset regions that are available
        for(const auto& region : mmap) {
            if(region.type == STIVALE2_MMAP_USABLE)
                free(region.base, region.length);
        }
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