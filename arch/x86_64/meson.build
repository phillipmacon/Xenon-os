arch_lib = static_library('arch',
    files(
        'io/port.cpp',
        'io/putc.cpp',

        'mem/paging.cpp',

        'interrupts.cpp',
        'idt.S',
        'gdt.cpp',
        'gdt.S',

        'cpuinfo.cpp',
    ),
    include_directories: inc
)

boot_sources = files('stivale.cpp')