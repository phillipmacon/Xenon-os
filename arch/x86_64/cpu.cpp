#include <cpu/cpu.h>
#include <cpu/cpuinfo.h>
#include <cpu/interrupts.h>
#include "gdt.h"

namespace cpu {
    void init() {
        cpu::info::printProcessorInfo();
        cpu::gdt::init();
        cpu::interrupts::init();
    }
}