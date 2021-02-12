#pragma once
using u8  = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

using size_t = u64;
using addr = u64;

static_assert(sizeof(u32) == 4, "Xenon only supports 64-bit systems/compilers");
static_assert(sizeof(u64) == 8, "Xenon only supports 64-bit systems/compilers");