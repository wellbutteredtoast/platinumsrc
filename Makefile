# This makefile is for Linux and OSX targets only.
# For Windows developers, you'll want to use the 'build.cmd' script
# that's in the root of this repo. By default this Makefile uses GCC compilers
# but you are welcome to change this to your preferred compiler!
#
# For all build options, run `make help`
#
# Happy hacking! :^)
#

# ------------------------------ Config ------------------------------

CONFIG 		?= debug

BUILD_DIR	:= build
SRC_DIR		:= src
EXT_DIR		:= external
INC_DIR		:= include

# ------------------------------ Tools ------------------------------

CC := gcc
CXX := g++

# ------------------------------ Flags ------------------------------

COMMON_CFLAGS := \
	-I$(INC_DIR) \
	-Wall -Wextra -Wshadow -Wdouble-promotion -Wsign-conversion \
	-Wimplicit-fallthrough -Wcast-align -Wformat=2 -Wpointer-arith \
	-Werror=return-type -Werror=null-dereference -Werror=undef \
	-fPIC -MMD -MP

ifeq ($(CONFIG),debug)
	FFLAGS := -fno-omit-frame-pointer -fno-optimize-sibling-calls \
		-fno-strict-aliasing -fwrapv -fstack-protector-strong \
		-fsanitize=address,undefined fno-sanitize-recover=all
else
	FFLAGS := -fno-strict-aliasing -fwrapv -fstack-protector-strong \
		-fno-plt -D_FORTIFY_SOURCES=2
endif

CFLAGS := $(COMMON_CFLAGS) $(FFLAGS) -std=c99 \
	-Wimplicit-int -Wstrict-prototypes -Wmissing-prototypes \
	-Werror=implicit-function-declaration

CXXFLAGS := $(COMMON_CFLAGS) $(FFLAGS) -std=c++98 \
	-Woverloaded-virtual -Wextra-semi -Wsuggest-override \
	-Wzero-as-null-pointer-constant -Wnon-virtual-dtor

LDFLAGS := $(FFLAGS)