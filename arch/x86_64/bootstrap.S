.include "arch/x86_64/multiboot.S"

.set KERNEL_VMA, 0xffffffff80000000
.extern kmain
.global bootstrap, init_ptl4, init_ptl3, init_ptl2

.code32
.section .boot
bootstrap:
	# Disable interrupts
	cli

	# Load the GDT table
	lgdt (init_gdt64_ptr - KERNEL_VMA)

	# Save multiboot info for later
	mov %eax, %esi
	mov %ebx, %edi

	# Set the stack pointer to our stack
	movl $(kstack_top - KERNEL_VMA), %esp

	# Reset EFLAGS
	pushl $0
	popf

	# Make sure paging is off
	movl %cr0, %eax
	andl $0x7fffffff, %eax
	movl %eax, %cr0

	# Enable PAE
	movl %cr4, %eax
	orl $0x20, %eax
	movl %eax, %cr4

	# Load the paging directory
	movl $(init_ptl4 - KERNEL_VMA), %eax
	movl %eax, %cr3

	# Enable long mode
	movl $0xc0000080, %ecx
	rdmsr
	orl $0x101, %eax
	wrmsr

	# And finally, enable paging
	movl %cr0, %eax
	orl $0x80010000, %eax
	movl %eax, %cr0

	# Jump to 64-bit code
	ljmp $0x8, $(boot64_trampoline - KERNEL_VMA)

.code64
boot64_trampoline:
	mov $boot64, %rax
	jmp *%rax

boot64:
	call kmain

_stop:
    cli
    hlt
    jmp _stop

# This hardcoded page table *will* become a problem later on.
# Easiest is to map the first 50-100 pages (100-200MB)
# Nothing else is done with this table anyways. When the kernel sets up memory this table will not be used again
.section .data
.align 0x10
gdt64:
	.quad 0x0000000000000000
	.quad 0x0020980000000000
gdt64_end:

.align 0x10
init_gdt64_ptr:
	.word gdt64_end - gdt64 - 1
	.long gdt64 - KERNEL_VMA

.align 0x1000
init_ptl4:
	.quad init_ptl3 - KERNEL_VMA + 0b11
	.fill 510,8,0
	.quad init_ptl3 - KERNEL_VMA + 0b11

.align 0x1000
init_ptl3:
	.quad init_ptl2 - KERNEL_VMA + 0b11
	.fill 509,8,0
	.quad init_ptl2 - KERNEL_VMA + 0b11
	.fill 1,8,0

.align 0x1000
init_ptl2:
	.quad 0x0000000000000083 # 0MB - 2MB
	.quad 0x0000000000200083 # 2MB - 4MB
	.quad 0x0000000000400083 # 4MB - 8MB
	.fill 509,8,0

.section .bss
.align 0x20
kstack:
	.fill 32 * 1024, 0 # 32KB
kstack_top: