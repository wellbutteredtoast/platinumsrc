# This makefile is for Linux and macOS targets only.
# For Windows developers, you'll have to wait a little while
# longer while I cook up a build script for Visual Studio.
#
# Or you can brave the depths and do so yourself!
#
# Run `make help` for options.
# Run `./bootstrap` before building!
#
# Happy hacking! :^)

# ------------------------------ Config ------------------------------

CONFIG      ?= debug
LIB_TYPE    ?= static
LIB_NAME    ?= platinum

BUILD_DIR   := build
SRC_DIR     := src
EXT_DIR     := external
INC_DIR     := include

# ------------------------------ Platform ------------------------------

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    SHARED_EXT   := dylib
    SHARED_FLAGS := -dynamiclib
else
    SHARED_EXT   := so
    SHARED_FLAGS := -shared
endif

STATIC_TARGET := $(BUILD_DIR)/lib$(LIB_NAME).a
SHARED_TARGET := $(BUILD_DIR)/lib$(LIB_NAME).$(SHARED_EXT)

ifeq ($(LIB_TYPE),shared)
    TARGET := $(SHARED_TARGET)
else
    TARGET := $(STATIC_TARGET)
endif

# ------------------------------ Tools ------------------------------

CC  := gcc
CXX := g++

# ------------------------------ Flags ------------------------------

COMMON_CFLAGS := \
    -I$(INC_DIR) -isystem $(EXT_DIR) -isystem $(EXT_DIR)/openal-soft/include \
    -Wall -Wextra -Wshadow -Wdouble-promotion -Wsign-conversion \
    -Wimplicit-fallthrough -Wcast-align -Wformat=2 -Wpointer-arith \
    -Werror=return-type -Werror=null-dereference -Werror=undef \
    -fPIC -MMD -MP

ifeq ($(CONFIG),debug)
    FFLAGS := -fno-omit-frame-pointer -fno-optimize-sibling-calls \
        -fno-strict-aliasing -fwrapv -fstack-protector-strong \
        -fsanitize=address,undefined -fno-sanitize-recover=all
else
    FFLAGS := -fno-strict-aliasing -fwrapv -fstack-protector-strong \
        -fno-plt -D_FORTIFY_SOURCES=2
endif

CFLAGS := $(COMMON_CFLAGS) $(FFLAGS) -std=c99 \
    -Wimplicit-int -Wstrict-prototypes -Wmissing-prototypes \
    -Werror=implicit-function-declaration

CXXFLAGS := $(COMMON_CFLAGS) $(FFLAGS) -std=c++98 \
    -Woverloaded-virtual -Wextra-semi -Wsuggest-override -Wnon-virtual-dtor

ifeq ($(CONFIG),debug)
    LDFLAGS := -fsanitize=address,undefined -fno-sanitize-recover=all
else
    LDFLAGS := -fstack-protector-strong -fno-plt -Wl,-z,relro,-z,now
endif

ifeq ($(UNAME_S),Darwin)
	SYSTEM_LIBS := -framework OpenGL -lm
else
	SYSTEM_LIBS := -lGL -lGLU -lopenal -lm
endif

# ------------------------------ Sources ------------------------------

C_SRCS   := $(shell find $(SRC_DIR) -name '*.c')
CXX_SRCS := $(shell find $(SRC_DIR) -name '*.cpp')

SRCS := $(C_SRCS) $(CXX_SRCS)

OBJS := $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(SRCS))
OBJS := $(OBJS:.c=.o)
OBJS := $(OBJS:.cpp=.o)

DEPS := $(OBJS:.o=.d)

# ------------------------------ Targets ------------------------------

.PHONY: all
all: $(TARGET)

# ------------------------------ Linking / Archiving ------------------------------

$(STATIC_TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	ar rcs $@ $(OBJS)

$(SHARED_TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(SHARED_FLAGS) -o $@ $(OBJS) $(LDFLAGS) $(SYSTEM_LIBS)

# ------------------------------ Compilation ------------------------------

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

# ------------------------------ Utilities ------------------------------

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: rebuild
rebuild: clean all

.PHONY: static
static:
	$(MAKE) LIB_TYPE=static CONFIG=$(CONFIG)

.PHONY: shared
shared:
	$(MAKE) LIB_TYPE=shared CONFIG=$(CONFIG)
# ------------------------------ Help ------------------------------

.PHONY: help
help:
	@echo "PlatinumSrc Make Tool"
	@echo ""
	@echo "Available targets:"
	@echo "  make / make all     Build the library (CONFIG=$(CONFIG), TYPE=$(LIB_TYPE))"
	@echo "  make clean          Remove build artifacts"
	@echo "  make rebuild        Clean and rebuild"
	@echo "  make static         Build static library"
	@echo "  make shared         Build shared library"
	@echo ""
	@echo "Configuration:"
	@echo "  CONFIG=debug        Debug build (default)"
	@echo "  CONFIG=release      Release build"
	@echo "  LIB_TYPE=static     Static library (default)"
	@echo "  LIB_TYPE=shared     Shared library"
	@echo ""
	@echo "Examples:"
	@echo "  make CONFIG=release"
	@echo "  make shared"
	@echo ""
	@echo "* Remember to bootstrap dependencies first!"
	@echo "* ./bootstrap"