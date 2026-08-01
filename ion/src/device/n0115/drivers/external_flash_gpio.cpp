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
  // Turn on the power to the external flash
  Config::PowerPin.group().MODER()->setMode(Config::PowerPin.pin(), GPIO::MODER::Mode::Output);
  Config::PowerPin.group().ODR()->set(Config::PowerPin.pin(), true);

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

  // Turn off the power to the external flash
  Config::PowerPin.group().ODR()->set(Config::PowerPin.pin(), false);
  Config::PowerPin.group().MODER()->setMode(Config::PowerPin.pin(), GPIO::MODER::Mode::Analog);
  Config::PowerPin.group().PUPDR()->setPull(Config::PowerPin.pin(), GPIO::PUPDR::Pull::None);
}

}
}
}
