#include <print.h>
#include <starg.h>

// TODO: Move string functions to their own file
size_t strlen(char* s) {
    size_t res{};
    while(*s) {
        res++;
        s++;
    }

    return res;
}

void reverse(char* s) {
    u32 i, j;

    for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
        char c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

char* itoa(i64 num, char* str, u32 base) {
    i64 i = 0;
    bool neg = false;

    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    if (num < 0 && base == 10) {
        neg = true;
        num = -num;
    }
  
    while (num != 0) {
        i64 rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num / base;
    }

    if (neg) 
        str[i++] = '-';
  
    str[i] = '\0';
    reverse(str);

    return str;
} 

char toLower(char c) {
    if(0x41 >= c && c <= 0x5a)
        c += 0x20;
    
    return c;
}

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

[[gnu::noinline]] void puti(i64 x, u32 base=10) {
    char buf[32];

    itoa(x, buf, base);
    puts(buf);
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
                case 'i':
                case 'd': {
                    i64 value = va_arg(args, i64);
                    puti(value);
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
    printklImpl(LogLevel::Debug, fmt, args);
    va_end(args);
}