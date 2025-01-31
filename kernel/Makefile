DEFAULT_HOST!=../util/default-host.sh
HOST?=DEFAULT_HOST
HOSTARCH!=../util/target-triplet-to-arch.sh $(HOST)

CFLAGS?=-Ofast
CPPFLAGS?=
LDFLAGS?=
LIBS?=

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/include

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra  -Wpedantic -Werror -Wshadow -Wformat=2 -Wconversion -Wunused-parameter -fstack-protector-all
CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -Iinclude  -Wpedantic -Werror -Wshadow -Wformat=2 -Wconversion -Wunused-parameter -fstack-protector-all
LDFLAGS:=$(LDFLAGS) -fstack-protector-all
LIBS:=$(LIBS) -nostdlib -lk -lgcc

ARCHDIR=arch/$(HOSTARCH)

include $(ARCHDIR)/make.config

CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS) -s
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)

KERNEL_OBJS=\
$(KERNEL_ARCH_OBJS) \
kernel/kernel.o \
kernel/stack.o \
kernel/panic.o \
kernel/assert.o

OBJS=\
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \

LINK_LIST=\
$(LDFLAGS) \
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(LIBS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \

.PHONY: all clean install install-headers install-kernel
.SUFFIXES: .o .c .S

all: tfos.elf

tfos.elf: $(OBJS) $(ARCHDIR)/linker.ld
	$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(LINK_LIST)
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp tfos.elf $(DESTDIR)$(BOOTDIR)
	grub-file --is-x86-multiboot ../sysroot/boot/tfos.elf

$(ARCHDIR)/crtbegin.o $(ARCHDIR)/crtend.o:
	OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

.c.o:
	echo "CC    $<    $@"
	@$(CC) -MD -c $< -o $@ -std=c2x $(CFLAGS) $(CPPFLAGS)

.S.o:
	echo "CC    $<    $@"
	@$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	rm -f tfos.elf
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-kernel

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-kernel: tfos.elf
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp tfos.elf $(DESTDIR)$(BOOTDIR)

-include $(OBJS:.o=.d)
