#include <print.h>
#include <starg.h>

[[gnu::noinline]] void puts(const char* s) {
    while(*s) {
        putc(*s);
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

    u32 start_index = 0;
    while(buf[start_index] == '0' && start_index < 15) {
        start_index++;
    }

    puts(&buf[start_index]);
}

char toLower(char c) {
    if(0x41 >= c && c <= 0x5a)
        c += 0x20;
    
    return c;
}

void printklImpl(LogLevel level, const char* fmt, va_list args) {
    switch(level) {
        case LogLevel::Debug:   puts("[debug] "); break;
        case LogLevel::Info:    puts("[info] "); break;
        case LogLevel::Warn:    puts("[WARN] "); break;
        case LogLevel::Error:   puts("[ERR] "); break;
        default: break;
    }

    // TODO: Replace this with a libc printf
    char* s = (char*)fmt;
    bool in_fmt = false;
    while(*s) {
        const char c = *s;

        if(in_fmt) {
            switch(toLower(c)) {
                case 'x': {
                    u64 value = va_arg(args, u64);
                    putx(value);
                    in_fmt = false;
                    break;
                }
                case 's': {
                    char* string_pointer = va_arg(args, char*);
                    puts(string_pointer);
                    in_fmt = false;
                    break;
                }
                case 'c': {
                    char value = va_arg(args, char);
                    putc(value);
                    in_fmt = false;
                    break;
                }
                case '%': {
                    putc('%');
                    in_fmt = false;
                    break;
                }
                default: {
                    // printk("Unknown format tag %%%s\n");
                    putc('%');
                    putc(c);
                    in_fmt = false;
                    break;
                }
            }
        }
        else {
            switch(c) {
                case '%':
                    in_fmt = true;
                    break;
                default:
                    putc(c);
                    break;
            }
        }

        s++;
	}
}

void printkl(LogLevel level, const char* fmt, ...) {
    va_list args;

    va_start(args, fmt);
    printklImpl(level, fmt, args);
    va_end(args);
}

void printk(const char* fmt, ...) {
    va_list args;

    va_start(args, fmt);
    printklImpl(LogLevel::None, fmt, args);
    va_end(args);
}