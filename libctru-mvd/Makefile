DEVKITARM ?= /opt/devkitpro/devkitARM
LIBCTRU ?= /opt/devkitpro/libctru
CC := $(DEVKITARM)/bin/arm-none-eabi-gcc
AR := $(DEVKITARM)/bin/arm-none-eabi-ar
CPPFLAGS += -Iinclude -I$(LIBCTRU)/include
CFLAGS += -std=gnu11 -O2 -Wall -Wextra -Werror -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft
SOURCES := $(wildcard source/services/*.c)
OBJECTS := $(patsubst source/services/%.c,build/%.o,$(SOURCES))

.PHONY: all check clean
all: build/libmvd-re.a

build/libmvd-re.a: $(OBJECTS)
	$(AR) rcs $@ $^

build/%.o: source/services/%.c $(wildcard include/3ds/services/*.h)
	@mkdir -p build
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

check:
	DEVKITARM="$(DEVKITARM)" LIBCTRU="$(LIBCTRU)" python3 tests/run.py

clean:
	rm -rf build
