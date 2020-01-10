# You can override those settings on the command line

PLATFORM ?= device
DEBUG ?= 0

EPSILON_VERSION ?= 12.0.0
EPSILON_CUSTOM_VERSION ?= 1.18.4-0
# USERNAME ?= N/A
# Valid values are "none", "update", "beta"
EPSILON_APPS ?= calculation rpn graph code statistics probability solver atom sequence regression settings external
EPSILON_I18N ?= en fr es de pt hu
EPSILON_GETOPT ?= 0
ESCHER_LOG_EVENTS_BINARY ?= 0
OMEGA_THEME ?= omega_light

include build/defaults.mak
include build/platform.$(PLATFORM).mak

ifndef USE_LIBA
  $(error platform.mak should define USE_LIBA)
endif
ifndef EXE
  $(error platform.mak should define EXE, the extension for executables)
endif
include build/toolchain.$(TOOLCHAIN).mak

SFLAGS += -DDEBUG=$(DEBUG)
ifdef USERNAME
  SFLAGS += -DUSERNAME="$(USERNAME)"
endif
SFLAGS += -DEPSILON_GETOPT=$(EPSILON_GETOPT)
SFLAGS += -DESCHER_LOG_EVENTS_BINARY=$(ESCHER_LOG_EVENTS_BINARY)
