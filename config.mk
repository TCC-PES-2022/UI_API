BIN        = p2
CC         = g++
AR         = ar
CFLAGS     = -Wall -Wextra -O2 -fPIC
COVFLAGS   = -fprofile-arcs -ftest-coverage -g
LDFLAGS    = -lgcrypt -lgpg-error
COVLDFLAGS = -lgcov --coverage
DESTDIR    ?= /tmp
LIBDEST    ?= $(DESTDIR)/lib
INCDEST    ?= $(DESTDIR)/include

HEADER = UI_API

MAIN   = p2

OBJ    = UI_API

LIB    = libuiapi.a
