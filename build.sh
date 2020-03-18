GREEN='\033[1;32m'
RED='\033[1;31m'
WHITE='\033[1;37m'
RESET='\033[0m'

mkdir build 2> /dev/null
pushd build
	cmake ..
    make -j xenon.iso
    t1=$?

    stat xenon.iso > /dev/null
    t2=$?

	if [ $t1 -ne 0 ] || [ $t2 -ne 0 ]
    then
        echo -e "${RED}====== Build failed ======${RESET}"
        exit 1
    fi
popd

echo -e "${GREEN}====== Build succeeded ======${RESET}"