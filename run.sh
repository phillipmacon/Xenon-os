RED='\033[1;31m'
WHITE='\033[1;37m'
RESET='\033[0m'

stat build/xenon.iso > /dev/null
if [ $? -ne 0 ]
then
    echo -e "${RED}Could not find disk image. You can build the disk image by running the build.sh script${RESET}"
	exit 1
fi

pushd build
	echo -e "${WHITE}======== Xenon Start ========${RESET}"

	# make qemu
	qemu-system-x86_64 	-cdrom xenon.iso \
						-device rtl8139 -device intel-hda \
						-serial stdio

	echo -e "${WHITE}========= Xenon End =========${RESET}"
popd