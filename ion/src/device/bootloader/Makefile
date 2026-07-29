ion_device_src += $(addprefix ion/src/device/n0110/drivers/, \
  cache.cpp \
  fccid.cpp \
  led.cpp \
  reset.cpp \
  usb.cpp \
)

ion_device_src += $(addprefix ion/src/device/bootloader/drivers/, \
  board.cpp \
  external_flash_tramp.cpp \
  power.cpp \
  trampoline.cpp \
)

ion_device_src += $(addprefix ion/src/device/bootloader/boot/, \
  rt0.cpp \
)

ion_device_src += $(addprefix ion/src/device/bootloader/, \
  platform_info.cpp \
)

LDSCRIPT ?= ion/src/device/bootloader/flash.ld
