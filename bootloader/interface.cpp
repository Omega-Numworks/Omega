
#include <assert.h>
#include <ion.h>

#include <bootloader/interface.h>
#include <bootloader/slot.h>
#include <bootloader/boot.h>

#include "computer.h"
#include "cable.h"

namespace Bootloader {

	void Interface::drawMessageBox(const char* line1, const char* line2) {
        KDContext* ctx = KDIonContext::sharedContext();
		//draw a red box at bottom of screen
		ctx->fillRect(KDRect(0, 210, 320, 32), KDColorRed);
		//get lenght of each line
		int line1Length = strlen(line1);
		int line2Length = strlen(line2);
        //draw the message
		ctx->drawString(line1, KDPoint(160-((line1Length*8)/2), 210), KDFont::SmallFont, KDColorWhite, KDColorRed);
		ctx->drawString(line2, KDPoint(160 - ((line2Length*8)/ 2), 228), KDFont::SmallFont, KDColorWhite, KDColorRed);
		
	}
	
void Interface::drawImage(KDContext* ctx, const Image* image, int offset) {
  const uint8_t* data;
  size_t size;
  size_t pixelBufferSize;
 
  if (image != nullptr) {
    data = image->compressedPixelData();
    size = image->compressedPixelDataSize();
    pixelBufferSize = image->width() * image->height();
  } else {
    return;
  }

  KDColor pixelBuffer[4000];
  assert(pixelBufferSize <= 4000);
  assert(Ion::stackSafe()); // That's a VERY big buffer we're allocating on the stack

  Ion::decompress(
    data,
    reinterpret_cast<uint8_t *>(pixelBuffer),
    size,
    pixelBufferSize * sizeof(KDColor)
  );

  KDRect bounds((320 - image->width()) / 2, offset, image->width(), image->height());

  ctx->fillRectWithPixels(bounds, pixelBuffer, nullptr);
}

void Interface::draw(int currentUSB) {
  KDContext * ctx = KDIonContext::sharedContext();
  ctx->fillRect(KDRect(0,0,320,240), KDColorBlack);
  drawImage(ctx, ImageStore::Computer, 70);
  drawImage(ctx, ImageStore::Cable, 132);
  drawMessageBox("Press (1)/(2) to select slot", "Press Power to reset");
  ctx->drawString("Slot A:", KDPoint(0, 0),  KDFont::SmallFont, KDColorWhite, KDColorBlack);
  ctx->drawString("Slot B:", KDPoint(0, 13), KDFont::SmallFont, KDColorWhite, KDColorBlack);
  ctx->drawString("Current:", KDPoint(0, 26),  KDFont::SmallFont, KDColorWhite, KDColorBlack);
  ctx->drawString("Press (6) for STM32 bootloader", KDPoint(0, 39),  KDFont::SmallFont, KDColorWhite, KDColorBlack);

  if (Boot::mode() == BootMode::SlotA) {
    ctx->drawString("Slot A", KDPoint(63, 26),  KDFont::SmallFont, KDColorWhite, KDColorBlack);
  } else if (Boot::mode() == BootMode::SlotB) {
    ctx->drawString("Slot B", KDPoint(63, 26),  KDFont::SmallFont, KDColorWhite, KDColorBlack);
  }

  Slot slots[2] = {Slot::A(), Slot::B()};

  for(uint8_t i = 0; i < 2; i++) {
    Slot slot = slots[i];

    if (slot.kernelHeader()->isValid() && slot.userlandHeader()->isValid()) {
      if (slot.userlandHeader()->isOmega() && slot.userlandHeader()->isUpsilon()) {
        ctx->drawString("Upsilon", KDPoint(56, i*13),  KDFont::SmallFont, KDColorWhite, KDColorBlack);
        ctx->drawString(slot.userlandHeader()->upsilonVersion(), KDPoint(112, i*13),  KDFont::SmallFont, KDColorWhite, KDColorBlack);
      } else if (slot.userlandHeader()->isOmega()) {
        ctx->drawString("Omega", KDPoint(56, i*13),  KDFont::SmallFont, KDColorWhite, KDColorBlack);
        ctx->drawString(slot.userlandHeader()->omegaVersion(), KDPoint(112, i*13),  KDFont::SmallFont, KDColorWhite, KDColorBlack);
      } else {
        ctx->drawString("Epsilon", KDPoint(56, i*13),  KDFont::SmallFont, KDColorWhite, KDColorBlack);
        ctx->drawString(slot.userlandHeader()->version(), KDPoint(112, i*13),  KDFont::SmallFont, KDColorWhite, KDColorBlack);
      }
      ctx->drawString(slot.kernelHeader()->patchLevel(), KDPoint(168, i*13),  KDFont::SmallFont, KDColorWhite, KDColorBlack);
    }
    else {
        if (slot.userland2Header()->isValid()) {
            ctx->drawString("Epsilon", KDPoint(56, i * 13), KDFont::SmallFont, KDColorWhite, KDColorBlack);
            ctx->drawString(slot.userland2Header()->version(), KDPoint(112, i * 13), KDFont::SmallFont, KDColorWhite, KDColorBlack);
            ctx->drawString(slot.kernelHeader()->patchLevel(), KDPoint(168, i * 13), KDFont::SmallFont, KDColorWhite, KDColorBlack);
        }
        else {
            ctx->drawString("Invalid", KDPoint(56, i * 13), KDFont::SmallFont, KDColorWhite, KDColorBlack);
        }
    }
    
  }
  char* USBFlashVerbose = "";
  if (currentUSB == 1) {
      USBFlashVerbose = "Slot A";
  }
  else if (currentUSB == 2) {
      USBFlashVerbose = "Slot B";
  }
  else if (currentUSB == 3) {
      USBFlashVerbose = "Slot A & B";
  }
  //add USBFlashVerbose to USBFlashVerboseStatus
  ctx->drawString("USB Flash Mode: ", KDPoint(0, 180), KDFont::SmallFont, KDColorWhite, KDColorBlack);
  ctx->drawString(USBFlashVerbose, KDPoint(112, 180), KDFont::SmallFont, KDColorWhite, KDColorBlack);
  ctx->drawString("Press (+) to switch mode", KDPoint(0, 193), KDFont::SmallFont, KDColorWhite, KDColorBlack);
  ctx->drawString("2.0.6", KDPoint(260, 180), KDFont::LargeFont, KDColorRed, KDColorBlack);
  if (Bootloader::Slot::A().kernelHeader()->isValid()) {
	  const char* version = Bootloader::Slot::A().kernelHeader()->version();
      bool isExam = Bootloader::ExamMode::SlotsExamMode::FetchSlotExamMode(version, "A") > 0;
      if (isExam) {
          ctx->drawString("E", KDPoint(238, 0), KDFont::SmallFont, KDColorWhite, KDColorBlack);
      }
  }

  if (Bootloader::Slot::B().kernelHeader()->isValid()) {
      const char* version = Bootloader::Slot::B().kernelHeader()->version();
      bool isExam = Bootloader::ExamMode::SlotsExamMode::FetchSlotExamMode(version, "B") > 0;
      if (isExam) {
          ctx->drawString("E", KDPoint(238, 13), KDFont::SmallFont, KDColorWhite, KDColorBlack);
      }
  }

}

}
