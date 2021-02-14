#include <cpu/cpuinfo.h>
#include <print.h>

void _cpuid(u32 id, u32* eax, u32* ebx, u32* ecx, u32* edx) {
    __asm("cpuid\n\t" : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx) : "0" (id));	
}

namespace cpu::info {
    void getProcessorModel(char* dst) {
        _cpuid(0x80000002, (u32*)dst + 0, (u32*)dst + 1, (u32*)dst + 2,  (u32*)dst + 3);
        _cpuid(0x80000003, (u32*)dst + 4, (u32*)dst + 5, (u32*)dst + 6,  (u32*)dst + 7);
        _cpuid(0x80000004, (u32*)dst + 8, (u32*)dst + 9, (u32*)dst + 10, (u32*)dst + 11);
        dst[48] = '\0';
    }

    void getProcessorVendor(char* dst) {
        u32 eax;
        _cpuid(0x00000000, &eax, (u32*)dst + 0, (u32*)dst + 2,  (u32*)dst + 1);
        dst[18] = '\0';
    }

    void printProcessorInfo() {
        char processorModel[49];
        char processorVendor[19];
        getProcessorModel(processorModel);
        getProcessorVendor(processorVendor);

        printk("[ cpu::info ] CPU Vendor: %s\n", processorVendor);
        printk("[ cpu::info ] CPU Model: %s\n", processorModel);
    }
}