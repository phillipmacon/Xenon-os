#pragma once

namespace cpu::interrupts {
    void init();

    void disableInterrupts();
    void enableInterrupts();
}