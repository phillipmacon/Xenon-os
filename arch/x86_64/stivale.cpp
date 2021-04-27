#include <stivale/stivale2.h>
#include <types.h>

static u8 kernel_stack[32768] = { 0 };

// framebuffer request tag for bootloader
static stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        .next = 0
    },
    
    .framebuffer_width  = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp    = 0
};

__attribute__((section(".stivale2hdr"), used))
static stivale2_header stivale_hdr = {
    .entry_point = 0,
    .stack = (uintptr_t)kernel_stack + sizeof(kernel_stack),
    .flags = 0,
    .tags = (uintptr_t)&framebuffer_hdr_tag
};