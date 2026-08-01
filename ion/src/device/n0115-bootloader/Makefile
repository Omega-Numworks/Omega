ion_device_src += $(addprefix ion/src/device/n0110/drivers/, \
  cache.cpp \
  led.cpp \
  reset.cpp \
  usb.cpp \
)

ion_device_src += $(addprefix ion/src/device/n0115/drivers/, \
  fccid.cpp \
)

ion_device_src += $(addprefix ion/src/device/n0110-bootloader/drivers/, \
  board.cpp \
  external_flash_tramp.cpp \
  power.cpp \
  trampoline.cpp \
)

ion_device_src += $(addprefix ion/src/device/n0110-bootloader/boot/, \
  rt0.cpp \
)

ion_device_src += $(addprefix ion/src/device/n0110-bootloader/, \
  platform_info.cpp \
)

LDSCRIPT ?= ion/src/device/n0110-bootloader/flash.ld
