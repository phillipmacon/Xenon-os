GREEN='\033[1;32m'
RED='\033[1;31m'
YELLOW='\033[1;33m'
WHITE='\033[1;37m'
RESET='\033[0m'

if [ ! -f "./external/stivale/stivale2.h" ]; then
    echo -e "${YELLOW}Submodules don't seem to be downloaded${RESET}"
    echo    "Downloading submodules..."

    if [ ! -f "$(which git)" ]; then
        echo -e "${RED}Failed to download submodules, git isn't installed!"
        exit -1
    fi

    git submodule init
    git submodule update
fi

meson build --cross-file=arch/x86_64.ini
pushd build
    # Compile the kernel
    echo -e "${GREEN}Building kernel...${RESET}"
    ninja

	if [ $? -ne 0 ]
    then
        echo -e "${RED}====== Failed while building kernel ======${RESET}"
        exit -1
    fi

    echo -e "${GREEN}Building ISO${RESET}"
    mkdir   -p iso_master/boot
    mkdir   -p iso_master/EFI/BOOT

    cp xenon.elf                                        ./iso_master/boot/xenon.elf
    cp ../external/limine-bin/limine.sys                ./iso_master/boot/limine.sys
    cp ../external/limine-bin/limine-cd.bin             ./iso_master/boot/limine-cd.bin
    cp ../external/limine-bin/limine-eltorito-efi.bin   ./iso_master/boot/limine-efi.bin
    cp ../external/limine-bin/BOOTX64.EFI               ./iso_master/EFI/BOOT/BOOTX64.EFI

    cat >iso_master/boot/limine.cfg <<EOL
:xenon
TIMEOUT=0
PROTOCOL=stivale2
KERNEL_PATH=boot:///boot/xenon.elf

GRAPHICS=yes
THEME_BACKGROUND=13001d
THEME_FOREGROUND=ffffff
EOL

    xorriso -as mkisofs -b boot/limine-cd.bin \
            -no-emul-boot -boot-load-size 4 -boot-info-table \
            -eltorito-alt-boot -e boot/limine-efi.bin \
            -no-emul-boot iso_master -o xenon.iso

    stat xenon.iso > /dev/null

	if [ $? -ne 0 ] 
    then
        echo -e "${RED}====== Failed while building ISO ======${RESET}"
        exit -1
    fi
popd

echo -e "${GREEN}====== Successfully built kernel! ======${RESET}"
