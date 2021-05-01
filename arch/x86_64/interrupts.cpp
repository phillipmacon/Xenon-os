#include <cpu/interrupts.h>
#include <cpu/ioport.h>
#include <print.h>
#include <mem.h>
#include "interrupts.h"

__attribute__((aligned(0x1000)))
static IDTEntry idt_entries[256];
static IDTR idtr;

extern "C" void loadIdt(u64 ptr);

void exceptionDefault(struct interrupt_frame* frame);
void exceptionDivideByZero(struct interrupt_frame* frame);
void exceptionDoubleFault(struct interrupt_frame* frame);
void exceptionPageFault(struct interrupt_frame* frame);

namespace cpu::interrupts {
    void setEntry(int i, u64 handler, IdtType type) {
        idt_entries[i].selector = 0x08;
        idt_entries[i].type = type;
        idt_entries[i].zero1 = 0;
        idt_entries[i].zero2 = 0;
        idt_entries[i].setOffset(handler);
    }
    
    void init() {
        disableInterrupts();

        util::memset(idt_entries, 0, sizeof(idt_entries));

        idtr.offset = (u64)&idt_entries;
        idtr.limit = sizeof(idt_entries) - 1;

        // for(int i = 0; i < 32; i++)
        //     setEntry(i, (u64)exceptionDefault, IdtType::InterruptGate);

        setEntry(0x0, (u64)exceptionDivideByZero, IdtType::InterruptGate);
        setEntry(0x8, (u64)exceptionDoubleFault, IdtType::InterruptGate);
        setEntry(0xe, (u64)exceptionPageFault, IdtType::InterruptGate);

        loadIdt((u64)&idtr);
        printk("IDT Loaded, enabling interrupts\n");
        enableInterrupts();
        printk("Interupts enabled!\n");
    }

    void disableInterrupts() {
        asm volatile("cli");
    }

    void enableInterrupts() {
        asm volatile("sti");
    }
}

__attribute__((interrupt))
void exceptionDefault(struct interrupt_frame* frame) {
    printkl(LogLevel::Abort, "AN EXCEPTION HAS OCCURRED\n");
    while(1) { __asm("hlt"); }
}

__attribute__((interrupt))
void exceptionDivideByZero(struct interrupt_frame* frame) {
    printkl(LogLevel::Abort, "DIVISION BY ZERO\n");
    while(1) { __asm("hlt"); }
}

__attribute__((interrupt))
void exceptionPageFault(struct interrupt_frame* frame) {
    printkl(LogLevel::Abort, "PAGE FAULT\n");
    while(1) { __asm("hlt"); }
}

__attribute__((interrupt))
void exceptionDoubleFault(struct interrupt_frame* frame) {
    printkl(LogLevel::Abort, "DOUBLE FAULT\n");
    while(1) { __asm("hlt"); }
}