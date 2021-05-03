#include <acpi.h>
#include <mem.h>
#include <print.h>
#include <string.h>

namespace uefi {
    void ACPI::init(u64 rsdp_addr) {
        RSDP* rsdp = reinterpret_cast<RSDP*>(rsdp_addr + mem::constants::kernelBase);
        printk("OEM ID: "); putn(rsdp->oem_id, 6); putc('\n');

        if(strncmp(rsdp->signature, "RSD PTR ", 8) != 0) {
            printkl(LogLevel::Error, "ACPI pointer not found\n");
            rsdt = nullptr;
            return;
        }

        rsdt = reinterpret_cast<RSDT*>(rsdp->rsdt_address + mem::constants::kernelBase);
        printk("OEM Table ID: "); putn(rsdt->oem_table_id, 8); putc('\n');

        printTables();
    }

    void ACPI::printTables() {
        int n = (rsdt->length - sizeof(RSDT)) / 4;
        for(int i = 0; i < n; i++) {
            char* desc = reinterpret_cast<char*>(rsdt->entries[i] + mem::constants::kernelBase);
            printk("rsdt desc %i: ", i); putn(desc, 4); putc('\n');
        }
    }
    
    void* ACPI::getTable(const char* sig) {
        if(!rsdt)
            return nullptr;
    
        int n = (rsdt->length - sizeof(RSDT)) / 4;
        for(int i = 0; i < n; i++) {
            char* desc = reinterpret_cast<char*>(rsdt->entries[i] + mem::constants::kernelBase);
            if(strncmp(desc, sig, 4) == 0)
                return desc;
        }

        return nullptr;
    }
}