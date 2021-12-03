project(
    'xenon', 'cpp',
    version : '0.00',
    default_options : [
        'cpp_std=c++17',
        'warning_level=3',
        'b_pie=false',
        'cpp_eh=none',
        'cpp_rtti=false',
        'warning_level=3'
    ]
)

add_project_arguments([
        '-ffreestanding',
        '-fno-pie',
        '-static',
        '-nostdlib',
        '-nostartfiles',
        '-fno-exceptions',
        '-fno-rtti',
        '-fno-stack-protector',
        '-mno-red-zone',
        '-fuse-cxa-atexit',
        '-fno-threadsafe-statics',
        '-mcmodel=kernel',
    ],
    language: 'cpp'
)

add_project_link_arguments([
        '-Wl,--build-id=none',
        '-Wl,-zmax-page-size=0x1000',
        '-Wl,-n',
        '-fno-pie',
        '-static',
        '-nostdlib',
        '-nostartfiles',
        '-nostdlib',
        '-fno-rtti'
    ],
    language: 'cpp'
)

kernel_sources = []
inc = include_directories('include', '.', 'external')

architecture = meson.get_cross_property('architecture')

subdir('arch/' + architecture)
subdir('kernel')

kernel_lib = static_library('kernel',
    kernel_sources,
    include_directories: inc,
    link_args : [
            '-T' + meson.current_source_dir()/'arch'/architecture/'linker.ld',
        ],
)

xenon_bin = executable('xenon.elf',
    boot_sources,
    include_directories: inc,
    link_with: [arch_lib, kernel_lib],
    link_args : [
            '-T' + meson.current_source_dir()/'arch'/architecture/'linker.ld',
        ],
)