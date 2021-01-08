.set MB2_HEADER_MAGIC,      0xe85250d6
.set MB2_ARCHITECTURE_I386, 0
.set MB2_CHECKSUM,          -(MB2_HEADER_MAGIC + __multiboot_header_end - __multiboot_header_start)

.set MB2_HEADER_TAG_END,            0
.set MB2_HEADER_TAG_OPTIONAL,       1
.set MB2_HEADER_TAG_FRAMEBUFFER,    5

.section .multiboot
.align 4
__multiboot_header_start:
    .long MB2_HEADER_MAGIC
    .long MB2_ARCHITECTURE_I386
    .long __multiboot_header_end - __multiboot_header_start
    .long MB2_CHECKSUM

.align 8
    .short MB2_HEADER_TAG_FRAMEBUFFER
    .short MB2_HEADER_TAG_OPTIONAL
    .long 20
    .long 0 # width
    .long 0 # height    
    .long 32 # depth

.align 8
    .short MB2_HEADER_TAG_END
    .short 0
    .long 8

__multiboot_header_end:
