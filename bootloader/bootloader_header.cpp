#include <stdint.h>

/* Bootloader version header.
 *
 * Firmwares that ship their own bootloader (e.g. Epsilon) read this header
 * before deciding whether to rewrite the bootloader in internal flash. If
 * BootloaderVersion is > 0, the bootloader is left untouched.
 *
 * The linker places the .header section right after the ISR vector table
 * (0x71 entries * 4 bytes = 0x1C4), i.e. at 0x002001C4 on N0110 and N0115. */

#ifndef BOOTLOADER_VERSION
#define BOOTLOADER_VERSION 0xFFFFFFFF
#endif

namespace Bootloader {

class BootloaderHeader {
public:
  constexpr BootloaderHeader() :
    m_header(Magic),
    m_version(BOOTLOADER_VERSION),
    m_footer(Magic) {}
private:
  constexpr static uint32_t Magic = 0xDEC0B007;
  uint32_t m_header;
  uint32_t m_version;
  uint32_t m_footer;
};

static_assert(sizeof(BootloaderHeader) == 12, "Unexpected bootloader header size");

const BootloaderHeader __attribute__((section(".header"), used)) k_bootloaderHeader;

}
