#ifndef ION_DEVICE_SHARED_RESET_H
#define ION_DEVICE_SHARED_RESET_H

#include <stdint.h>

namespace Ion {
namespace Device {
namespace Reset {

void core();
void coreWhilePlugged();

/* jump takes as parameter the address of the isr vector to jump to: the data at
 * jumpIsrVectorAddress must the stack_pointer address, and the data at
 * jumpIsrVectorAddress + 1 must be the reset method address. */
void jump(uint32_t jumpIsrVectorAddress);

/* STM32 system bootloader (ROM DFU, USB ID 0483:df11).
 * Only implemented on N0110/N0115 (STM32F730, system memory at 0x1FF00000).
 * jumpToSTBootloader stores STBootloaderMagic in RAM and resets the core; the
 * very beginning of start() (rt0.cpp) then jumps to the ROM while the chip is
 * still in its reset state (clocks, MPU, caches and peripherals untouched). */
constexpr uint32_t STBootloaderMagic = 0x57B007D0;
constexpr uint32_t STBootloaderAddress = 0x1FF00000;
extern volatile uint32_t STBootloaderRequest;
void __attribute__((noreturn)) jumpToSTBootloader();

}
}
}

#endif
