GREEN='\033[1;32m'
RED='\033[1;31m'
WHITE='\033[1;37m'
RESET='\033[0m'

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

    # Build the ISO using grub-mkrescue or grub2-mkrescue (can't wait to get rid of this)
    echo -e "${GREEN}Building ISO${RESET}"
    mkdir   -p iso/boot/grub
    cp      xenon.elf ./iso/boot/xenon.elf
    echo    'set timeout=0'                 > iso/boot/grub/grub.cfg
    echo    'set default=0'                 >> iso/boot/grub/grub.cfg
    echo    'menuentry \"xenon\" {'         >> iso/boot/grub/grub.cfg
    echo    '  multiboot2 /boot/xenon.elf'  >> iso/boot/grub/grub.cfg
    echo    '  boot'                        >> iso/boot/grub/grub.cfg
    echo    '}'                             >> iso/boot/grub/grub.cfg
    grub-mkrescue --output=xenon.iso iso   2> /dev/null
    rm -rf iso

    stat xenon.iso > /dev/null

	if [ $? -ne 0 ] 
    then
        echo -e "${RED}====== Failed while building ISO ======${RESET}"
        exit -1
    fi
popd

echo -e "${GREEN}====== Successfully built kernel! ======${RESET}"
