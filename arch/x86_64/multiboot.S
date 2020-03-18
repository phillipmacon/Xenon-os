.set ALIGN,    	1 << 0
.set MEMINFO,  	1 << 1
.set VIDINFO,  	1 << 2
.set MAGIC,		0xE85250D6
.set FLAGS, 	(ALIGN | MEMINFO | VIDINFO)
.set CHECKSUM, -(MAGIC + header_end - header_start)

.section .boot
.align 4
header_start:
    .long MAGIC
    .long 0
    .long header_end - header_start
    
    .long CHECKSUM
    
    .word 0
    .word FLAGS
    .long 0
header_end:

