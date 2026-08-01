#include <bootloader/boot.h>
#include <bootloader/slot.h>
#include <ion.h>
#include <ion/src/device/shared/drivers/reset.h>
#include <bootloader/interface.h>

#include <assert.h>

namespace Bootloader {

BootMode Boot::mode() {
  // We use the exam mode driver as storage for the boot mode
  uint8_t mode = Ion::ExamMode::FetchExamMode();

  if (mode > 3)
    return Unknown;
  
  return (BootMode) mode;
}

void Boot::setMode(BootMode mode) {
  BootMode currentMode = Boot::mode();
  if (currentMode == mode)
    return;

  assert(mode != BootMode::Unknown);
  int8_t deltaMode = (int8_t)mode - (int8_t)currentMode;
  deltaMode = deltaMode < 0 ? deltaMode + 4 : deltaMode;
  assert(deltaMode > 0);
  Ion::ExamMode::IncrementExamMode(deltaMode);
}

__attribute__((noreturn)) void Boot::boot() {
  assert(mode() != BootMode::Unknown);

  if (!Slot::A().kernelHeader()->isValid() && !Slot::B().kernelHeader()->isValid()) {
    // Bootloader if both invalid
    bootloader();
  } else if (!Slot::A().kernelHeader()->isValid()) {
    // If slot A is invalid and B valid, boot B
    setMode(BootMode::SlotB);
    Slot::B().boot();
  } else if (!Slot::B().kernelHeader()->isValid()) {
    // If slot B is invalid and A valid, boot A
    setMode(BootMode::SlotA);
    Slot::A().boot();
  } else {
      const char* version = Bootloader::Slot::A().kernelHeader()->version();
      bool isExam = Bootloader::ExamMode::SlotsExamMode::FetchSlotExamMode(version, "A") > 0;
      if (isExam) {
          Bootloader::Slot::A().boot();
      }

      version = Bootloader::Slot::B().kernelHeader()->version();
      isExam = Bootloader::ExamMode::SlotsExamMode::FetchSlotExamMode(version, "B") > 0;
      if (isExam) {
          Bootloader::Slot::B().boot();
      }

    // Both valid, boot the selected one
    if (mode() == BootMode::SlotA) {
      Slot::A().boot();
    } else if (mode() == BootMode::SlotB) {
      Slot::B().boot();
    }
  }

  // Achivement unlocked: How Did We Get Here?
  bootloader();
}

__attribute__ ((noreturn)) void Boot::bootloader() {
  for(;;) {
    int modeFlash = 3;
    // Draw the interfaces and infos
    Bootloader::Interface::draw(modeFlash);
    bool abortUSB = false;
    while (1) {
        uint64_t scan = Ion::Keyboard::scan();
        if (scan == Ion::Keyboard::State(Ion::Keyboard::Key::OnOff)) {
                Ion::Device::Reset::core();
        }
        //allow to set bootmode with 1 and 2
        if (scan == Ion::Keyboard::State(Ion::Keyboard::Key::One)) {
            Bootloader::Boot::setMode(Bootloader::BootMode::SlotA);
            Bootloader::Interface::draw(modeFlash);
			Ion::Timing::msleep(100);
        }
        if (scan == Ion::Keyboard::State(Ion::Keyboard::Key::Two)) {
            Bootloader::Boot::setMode(Bootloader::BootMode::SlotB);
            Bootloader::Interface::draw(modeFlash);
            Ion::Timing::msleep(100);
        }
        if (scan == Ion::Keyboard::State(Ion::Keyboard::Key::Plus)) {
			modeFlash = modeFlash + 1;
            if (modeFlash > 3) {
				modeFlash = 1;
			}
			Ion::Timing::msleep(100);
            Bootloader::Interface::draw(modeFlash);
		}
		
		if (Ion::USB::isPlugged() && !abortUSB) {
            // Enable USB
            Ion::USB::enable();

            Bootloader::Interface::drawMessageBox("USB Enabled", "Press Back to disable USB");
            // Launch the DFU stack, allowing to press Back to quit and reset
            Ion::USB::DFU(true, modeFlash);
			//check if usb is plugged
			if (Ion::USB::isPlugged()) {
                abortUSB = true;
			}
			// Disable USB
			Ion::USB::disable();
			Bootloader::Interface::draw(modeFlash);
        }
		
		//Prevent USB from launching after Back press
		if (!Ion::USB::isPlugged() && abortUSB) {
			abortUSB = false;
		}

		Ion::Timing::msleep(100);
		
    }
  }
}

}
