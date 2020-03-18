#include <io.h>
#include <types.h>

void puts(const char* s) {
    while(*s) {
        io::out8(0x3f8, *s);
        s++;
	}
}

extern "C" void kmain(const char* multiboot_structure, u32 multiboot_magic)
{
	u16* vmem = (u16*)0xb8000;
    for(int _x = 0; _x < 80*25; _x++)
        vmem[_x] = 0x1000;

	puts("\n");
	puts("This build was compiled on " __DATE__ " at " __TIME__ "\n");
    puts("__--== !              Hello from the cool zone               ! ==--__\n");
    puts("__--== ! We are running on the shiny new x86_64 architecture ! ==--__\n");

    while(1) { asm volatile("hlt"); }
}