#include <drivers/external_flash.h>
#include <drivers/cache.h>
#include <drivers/config/external_flash.h>
#include <drivers/config/clocks.h>
#include <ion/timing.h>

namespace Ion {
namespace Device {
namespace ExternalFlash {

using namespace Regs;

void initGPIO() {
  for(const AFGPIOPin & p : Config::Pins) {
    p.init();
  }
}

void shutdownGPIO() {
  for(const AFGPIOPin & p : Config::Pins) {
    p.group().OSPEEDR()->setOutputSpeed(p.pin(), GPIO::OSPEEDR::OutputSpeed::Low);
    p.group().MODER()->setMode(p.pin(), GPIO::MODER::Mode::Analog);
    p.group().PUPDR()->setPull(p.pin(), GPIO::PUPDR::Pull::None);
  }
}

}
}
}
