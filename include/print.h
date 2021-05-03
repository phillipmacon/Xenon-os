#pragma once
#include <types.h>

enum class LogLevel {
    Debug,
    Info,
    Warn,
    Error,
    Abort,
    None
};

void putc(const char c);
void puts(const char* s);
void putn(const char* s, size_t n);

void printkl(LogLevel level, const char* msg, ...);
void printk(const char* msg, ...);