INCLUDE_PATH    := ./src
SOURCE_PATH     := ./src
DEPENDENCY_PATH := ./src/dep
OBJECT_PATH     := ./src/obj

LDLIBS := -lpthread
CFLAGS += -O3

PROGRAM_NAME := netraf_pulse

include makefile.mk
