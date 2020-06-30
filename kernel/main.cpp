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

    // Reading from a serial port always returns the last character it remembers
    // This ugly setup is just for testing io ports
    u8 last_input{};
    while(1) {
        u8 input = io::in8(0x3f8);
        if(input != last_input) {
            puts("Received character from COM0: ");
            io::out8(0x3f8, input);
            puts("\n");
            last_input = input;
        }
    }

    // while(1) { asm volatile("hlt"); }
}