# Makefile for OS project
# To build, first `make dep`, them `make`. Everything should be automatic.
# Will compile all *.c and *.S files in the current directory.
#
# macOS: needs `brew install i686-elf-gcc e2fsprogs qemu`, then `make dep && make && make run`
# Linux: needs a gcc that can build 32-bit code (gcc-multilib), then `make dep && sudo make`


# Flags to use when compiling, preprocessing, assembling, and linking
# -std=gnu89 / -fcommon / -fpermissive keep this 2023 code building with modern gcc
# (globals are defined in headers, and old-style "()" function pointer prototypes are used)
CFLAGS+=-Wall -fno-builtin -fno-stack-protector -nostdlib -std=gnu89 -fcommon -fpermissive -fno-pie
ASFLAGS+=
LDFLAGS+=-nostdlib -static

ifeq ($(shell uname -s),Darwin)
CC=i686-elf-gcc
else
CC=gcc
CFLAGS+=-m32
ASFLAGS+=-m32
LDFLAGS+=-m32 -no-pie
endif

#If you have any .h files in another directory, add -I<dir> to this line
CPPFLAGS+=-nostdinc -g

# This generates the list of source files
SRC=$(wildcard *.S) $(wildcard *.c) $(wildcard */*.S) $(wildcard */*.c)

# This generates the list of .o files. The order matters, boot.o must be first
OBJS=boot.o
OBJS+=$(filter-out boot.o,$(patsubst %.S,%.o,$(filter %.S,$(SRC))))
OBJS+=$(patsubst %.c,%.o,$(filter %.c,$(SRC)))

bootimg: Makefile $(OBJS)
	rm -f bootimg
	$(CC) $(LDFLAGS) $(OBJS) -Ttext=0x400000 -o bootimg -lgcc
ifeq ($(shell uname -s),Darwin)
	python3 update_image.py mp3.img bootimg
else
	sudo ./debug.sh
endif

# Boot the disk image in QEMU (add DEBUG=1 to wait for gdb on localhost:1234)
run:
	qemu-system-i386 -drive file=mp3.img,format=raw -m 256 -name "ECE391 OS" $(if $(DEBUG),-s -S,)

dep: Makefile.dep

Makefile.dep: $(SRC)
	$(CC) -MM $(CPPFLAGS) $(SRC) > $@

.PHONY: clean run
clean:
	rm -f *.o */*.o Makefile.dep

ifneq ($(MAKECMDGOALS),dep)
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),run)
include Makefile.dep
endif
endif
endif
