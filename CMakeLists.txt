cmake_minimum_required(VERSION 3.10)
project(xenon)

set(arch "x86_64")

enable_language(ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/arch/linker.ld")

set(CMAKE_LINKER "ld")
set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_LINKER> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
set(CMAKE_EXE_LINKER_FLAGS "-nostdlib -no-pie --build-id=none -zmax-page-size=0x1000 -n -T${LINKER_SCRIPT}")
set(CMAKE_ASM_FLAGS "-m64")
set(CMAKE_CXX_FLAGS " \
	-std=c++17 -Wall -O3 \
	-fno-exceptions -fno-rtti -fuse-cxa-atexit -fno-threadsafe-statics \
	-m64 -march=native -fno-PIC -pedantic -W -Wall -ffreestanding -mno-red-zone -mcmodel=kernel \
	-mno-mmx -mno-sse -mno-sse2 -mno-sse3 -mno-ssse3 -mno-sse4.1 -mno-sse4.2 -mno-sse4 -mno-sse4a -mno-3dnow -mno-avx -mno-avx2 \
")

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -target ${arch}-elf")
else()
	message(WARNING "Using a non-cross compiler isn't recommended! Here be monsters!")
endif()

# Include the current and include/ directory globally
include_directories(.)
include_directories(include)

# Compile architecture specific code
add_subdirectory(arch)

# Compile the kernel
FILE (GLOB_RECURSE KERNEL_SOURCE_FILES "kernel/*.cpp")
add_executable(xenon.elf ${KERNEL_SOURCE_FILES})

target_link_libraries(xenon.elf cpu)

# Create ISO
# TODO: This command is probably arch-specific, move this later
add_custom_target(xenon.iso DEPENDS xenon.elf
    COMMAND strip xenon.elf

    COMMAND echo    $(pwd)
    COMMAND mkdir   -p iso/boot/grub
    COMMAND cp      xenon.elf ./iso/boot/xenon.elf
    COMMAND echo    'set timeout=0'                 > iso/boot/grub/grub.cfg
    COMMAND echo    'set default=0'                 >> iso/boot/grub/grub.cfg
    COMMAND echo    'menuentry \"xenon\" {' 	    >> iso/boot/grub/grub.cfg
    COMMAND echo    '  multiboot2 /boot/xenon.elf'  >> iso/boot/grub/grub.cfg
    COMMAND echo    '  boot'                        >> iso/boot/grub/grub.cfg
    COMMAND echo    '}'                             >> iso/boot/grub/grub.cfg
    COMMAND grub-mkrescue --output=xenon.iso iso 	2> /dev/null
    COMMAND rm -rf iso
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(qemu DEPENDS xenon.iso
    COMMAND qemu-system-x86_64 -cdrom xenon.iso -device rtl8139 -device intel-hda -serial stdio
)

add_custom_target(qemu_gdb DEPENDS xenon.iso
    COMMAND qemu-system-x86_64 -cdrom xenon.iso -device rtl8139 -device intel-hda -serial stdio -no-reboot -d cpu_reset --gdb tcp::1234 -S
)
