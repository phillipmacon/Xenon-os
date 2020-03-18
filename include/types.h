#pragma once
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

static_assert(sizeof(u32) == 4, "Xenon only supports 64-bit systems/compilers");
static_assert(sizeof(u64) == 8, "Xenon only supports 64-bit systems/compilers");