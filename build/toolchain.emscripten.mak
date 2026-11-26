CC = emcc
CXX = em++
LD = em++
CPP = cpp

EMFLAGS = -s PRECISE_F32=1 -s ASYNCIFY

ifeq ($(DEBUG),1)
EMFLAGS += --profiling-funcs
EMFLAGS += -s ASSERTIONS=1
EMFLAGS += -s SAFE_HEAP=1
EMFLAGS += -s STACK_OVERFLOW_CHECK=1
EMFLAGS += -s MAIN_MODULE=1
endif
# No MAIN_MODULE in release: nothing loads side modules on the web, and recent
# Emscripten drops unexported system symbols (SDL, libc++) with MAIN_MODULE=2.

# EMFLAGS += -s ERROR_ON_UNDEFINED_SYMBOLS=0

# Configure EMFLAGS
EMFLAGS += -s WASM=1 -s SINGLE_FILE=1

# Configure LDFLAGS
EMSCRIPTEN_MODULARIZE ?= 1
LDFLAGS += -s MODULARIZE=$(EMSCRIPTEN_MODULARIZE) -s 'EXPORT_NAME="Epsilon"'

SFLAGS += $(EMFLAGS)
LDFLAGS += $(EMFLAGS) -Oz -s EXPORTED_FUNCTIONS='["_main", "_IonSimulatorKeyboardKeyDown", "_IonSimulatorKeyboardKeyUp", "_IonSimulatorEventsPushEvent", "_IonSoftwareVersion", "_IonPatchLevel", "_IonDisplayForceRefresh"]' -s EXPORTED_RUNTIME_METHODS='["UTF8ToString"]'
