OUTPUT_FORMAT(elf64-x86-64)

KERNEL_LMA = 1M;
KERNEL_VMA = 0xffffffff80200000;

ENTRY(kmain)
 
SECTIONS
{
	_start = .;
    . = KERNEL_VMA;
 
    .stivale2hdr : {
        KEEP(*(.stivale2hdr))
    }
 
    .text : {
        *(.text*)
    }
 
    .rodata : {
        *(.rodata*)
		__CTOR_LIST__ = .;
		*(.ctors)
		__CTOR_END__ = .;
    }
 
    .data : {
        *(.data*)
    }
 
    .bss : {
        *(COMMON)
        *(.bss*)
    }

	_end = .;
}