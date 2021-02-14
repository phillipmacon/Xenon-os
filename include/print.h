#pragma once
#include <types.h>

enum class LogLevel {
    Debug,
    Info,
    Warn,
    Error,
    None
};

void putc(const char c);
void puts(const char* s);

void printkl(LogLevel level, const char* msg, ...);
void printk(const char* msg, ...);