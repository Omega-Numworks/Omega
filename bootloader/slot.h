#ifndef BOOTLOADER_SLOT_H
#define BOOTLOADER_SLOT_H

#include <stdint.h>

#include "kernel_header.h"
#include "userland_header.h"
#include "slot_exam_mode.h"

namespace Bootloader {

class Slot {

public:
  Slot(uint32_t address) :
    m_kernelHeader(reinterpret_cast<KernelHeader*>(address)),
    m_userlandHeader(reinterpret_cast<UserlandHeader*>(address + 64 * 1024)),
    m_userland2Header(reinterpret_cast<UserlandHeader*>(address + 128 * 1024)) { }

  const KernelHeader* kernelHeader() const;
  const UserlandHeader* userlandHeader() const;
  const UserlandHeader* userland2Header() const;
  [[ noreturn ]] void boot() const;

  static const Slot A();
  static const Slot B();

private:
  const KernelHeader* m_kernelHeader;
  const UserlandHeader* m_userlandHeader;
  const UserlandHeader* m_userland2Header;

};

}

#endif