file(GLOB_RECURSE ARCH_SOURCE "*.cpp" "*.S")

add_library(cpu STATIC ${ARCH_SOURCE})