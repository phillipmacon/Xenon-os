#pragma once
#include <mem.h>
#include <types.h>

// TODO: Move this
struct [[gnu::packed]] BitmapHeader {
    u16 magic;
    u32 size;
    u16 reserved0;
    u16 reserved1;
    u32 offset;
};

struct [[gnu::packed]] BitmapInfoHeader {
    u32 header_size;
    u32 image_width;
    u32 image_height;
};

struct [[gnu::packed]] RGB24 {
    u8 r;
    u8 g;
    u8 b;
};

static_assert(sizeof(RGB24) == 3);

namespace uefi {
    struct [[gnu::packed]] RSDT {
        char    signature[4];
        u32     length;
        u8      revision;
        u8      checksum;
        char    oem_id[6];
        char    oem_table_id[8];
        u32     oem_revision;
        u32     creator_id;
        u32     creator_revision;
        u32*    entries;
    };

    struct [[gnu::packed]] RSDP {
        char    signature[8];
        u8      checksum;
        char    oem_id[6];
        u8      revision;
        u32     rsdt_address;
        u32     length;
        u64     xsdt_address;
        u8      extended_checksum;
        char    reserved[3];
    };

    struct [[gnu::packed]] BGRT {
        char    signature[4];
        u32     length;
        u8      revision;
        u8      checksum;
        char    oem_id[6];
        char    oem_table_id[8];
        u32     oem_revision;
        u32     creator_id;
        u32     creator_revision;
        u16     version;
        u8      status;
        u8      image_type;
        u64     image_address;
        u32     image_offset_x;
        u32     image_offset_y;
    };

    class ACPI {
    private:
        RSDT* rsdt;
        
    public:
        void init(u64 rsdp);
        void printTables();

        void* getTable(const char* sig);

        static ACPI& instance() {
            static ACPI inst;
            return inst;
        }
    };
}