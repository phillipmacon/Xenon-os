#include <cpu/ioport.h>
#include <print.h>

[[gnu::noinline]] void puts(const char* s) {
    while(*s) {
        io::out8(0x3f8, *s);
        s++;
	}
}

[[gnu::noinline]] void putx(u64 x) {
    char buf[17];

    for(int i = 0; i < 16; i++) {
        buf[15-i] = "0123456789abcdef"[x & 0xf];
        x >>= 4;
    }

    buf[16] = '\0';
    puts(buf);
}