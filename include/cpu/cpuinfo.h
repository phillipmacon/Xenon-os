#pragma once
#include "include/types.h"

namespace cpu::info {
    // Gets the processor model name (eg. Intel(R) Core(TM) i7-10750H CPU @ 2.60GHz)
    // Name is written to dst (48 bytes)
    void getProcessorModel(char* dst);

    // Gets the processor vendor name (eg. GenuineIntel, AuthenticAMD)
    // Name is written to dst (12 bytes)
    void getProcessorVendor(char* dst);

    void printProcessorInfo();
}