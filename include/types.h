#pragma once
#include <stdint.h>

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using size_t = u64;
using addr = u64;

static_assert(sizeof(u8) == 1,  "Type u8 does not have the right size");
static_assert(sizeof(u16) == 2, "Type u16 does not have the right size");
static_assert(sizeof(u32) == 4, "Type u32 does not have the right size");
static_assert(sizeof(u64) == 8, "Type u64 does not have the right size");