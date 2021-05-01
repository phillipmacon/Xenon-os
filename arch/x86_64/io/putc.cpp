#include <cpu/ioport.h>
#include <fb.h>
#include <print.h>

void putc(const char c) {
    io::out8(0x3f8, c);
    video::fb::putc(c);
}