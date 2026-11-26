#include <drivers/reset.h>
#include <drivers/cache.h>

namespace Ion {
namespace Device {
namespace Reset {

/* Lives in .bss: start() reads it before zeroing .bss, and RAM is preserved
 * across a core reset. */
volatile uint32_t STBootloaderRequest;

void coreWhilePlugged() {
  core();
}

void jumpToSTBootloader() {
  STBootloaderRequest = STBootloaderMagic;
  // Make sure the request reaches RAM before resetting
  Cache::cleanDCache();
  Cache::dsb();
  core();
  __builtin_unreachable();
}

}
}
}
