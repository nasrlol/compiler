CC := clang
BUILD_DIR := build

SRC := source/compiler/compiler.c
TARGET := compiler


CFLAGS := -I. -Isource -D_GNU_SOURCE=1 \
          -fno-threadsafe-statics -nostdinc++ \
          -Wall -Wextra -Wconversion \
          -Wno-sign-conversion -Wno-unused-parameter \
          -Wno-missing-field-initializers \
          -Wno-main-return-type -Wno-unused-variable \
          -Wno-unused-but-set-variable \
          -Wno-switch -Wno-unused-function \
          -fno-omit-frame-pointer \
          -fdiagnostics-absolute-paths \
          -Wno-null-dereference \

# LDFLAGS := -lm -ldl -lpthread

DEBUG ?= 1
RELEASE ?= 0
ASAN ?= 0

ifeq ($(DEBUG),1)
CFLAGS += -g -ggdb3
endif

ifeq ($(RELEASE),1)
CFLAGS += -O3
endif

ifeq ($(ASAN),1)
CFLAGS += -fsanitize=address -fsanitize-trap
endif


all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/$(TARGET): $(SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $@


clean:
	...
