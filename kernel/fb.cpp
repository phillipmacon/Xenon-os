#include <fb.h>
#include <mem.h>
#include "xenonfont.binh"

namespace video::fb {
    u32* fb_ptr{};
    size_t fb_width, fb_height;

    size_t fb_cols, fb_rows;
    size_t char_x, char_y;
    
    void init(void* ptr, size_t width, size_t height) {
        fb_ptr = reinterpret_cast<u32*>(ptr);
        fb_width = width;
        fb_height = height;

        fb_cols = fb_width / 8;
        fb_rows = fb_height / 16;
        char_x = 0;
        char_y = 0;
    }

    void putc(char c) {
        if(!fb_ptr)
            return;

        switch(c) {
        case '\n':
            char_x = 0;
            if(++char_y >= fb_rows)
                char_y = 0;
            return;
        case '\r':
            char_x = 0;
            return;
        }

        const u8* character = xenonfont[xenonfont_conversiontable[c]];
        u16 x = char_x * 8, y = char_y * 16;

        for(u8 i = 0; i < 16; i++)
            for(u8 b = 0; b < 8; b++)
                fb_ptr[(y + i) * fb_width + (x + b)] = character[i] & (0x80 >> b) ? 0xffffffff : 0;

        if(++char_x >= fb_cols) {
            char_x = 0;
            if(++char_y >= fb_rows)
                char_y = 0;
        }
    }

    void puts(const char* s) {
        if(!fb_ptr)
            return;

        while(*s) {
            putc(*s);
            s++;
        }
    }

    void clear() {
        char_x = 0; char_y = 0;
        util::memset(fb_ptr, 0, fb_width * fb_height * 4);
    }
}