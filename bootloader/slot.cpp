#include <bootloader/slot.h>
#include <ion/src/device/shared/drivers/board.h>
#include <ion/src/device/shared/drivers/external_flash.h>
#include <bootloader/interface.h>

extern "C" void jump_to_firmware(const uint32_t* stackPtr, const void(*startPtr)(void));

namespace Bootloader {

const Slot Slot::A() {
  return Slot(0x90000000);
}

const Slot Slot::B() {
  return Slot(0x90400000);
}

const KernelHeader* Slot::kernelHeader() const {
  return m_kernelHeader;
}

const UserlandHeader* Slot::userlandHeader() const {
  return m_userlandHeader;
}

const UserlandHeader* Slot::userland2Header() const {
	return m_userland2Header;
}

[[ noreturn ]] void Slot::boot() const {
	//protection mechanism against bootloader update introduced in E20
	//check if the slot use Userland2Header
	if (m_userland2Header->isValid()) {
		if (kernelHeader() == (void*)0x90000000) {
			//check if 0x90010000 = 0xFFFFFFFF
			if (*(uint32_t*)0x90010000 != 0xFFFFFFFF) {
				Ion::Device::ExternalFlash::EraseSector(9);
			}
		}
		else if (kernelHeader() == (void*)0x90400000) {
			//check if 0x90410000 = 0xFFFFFFFF
			if (*(uint32_t*)0x90410000 != 0xFFFFFFFF) {
				Ion::Device::ExternalFlash::EraseSector(73);
			}
			
		}
		
	}
  // Configure the MPU for the booted firmware
  Ion::Device::Board::bootloaderMPU();

  // Jump
  jump_to_firmware(kernelHeader()->stackPointer(), kernelHeader()->startPointer());
  for(;;);
}

}
