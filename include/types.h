#pragma once
using u8  = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

using i8  = char;
using i16 = short;
using i32 = int;
using i64 = long long;

using size_t = u64;
using addr = u64;

static_assert(sizeof(u8) == 1,  "Type u8 does not have the right size");
static_assert(sizeof(u16) == 2, "Type u16 does not have the right size");
static_assert(sizeof(u32) == 4, "Type u32 does not have the right size");
static_assert(sizeof(u64) == 8, "Type u64 does not have the right size");