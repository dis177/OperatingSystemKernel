CROSS := i686-elf
AS    := $(CROSS)-as
CC    := $(CROSS)-gcc
LD    := $(CROSS)-ld
CFLAGS:= -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS:= -T linker.ld -nostdlib

all: myos.iso

boot.o: boot.s
	$(AS) $< -o $@
kernel.o: kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

myos.bin: boot.o kernel.o linker.ld
	$(LD) $(LDFLAGS) boot.o kernel.o -o $@

myos.iso: myos.bin grub.cfg
	mkdir -p isodir/boot/grub
	cp $<          isodir/boot/
	cp grub.cfg    isodir/boot/grub/
	grub-mkrescue -o $@ isodir

run: myos.iso
	qemu-system-i386 -cdrom $<

clean:
	rm -rf *.o myos.bin myos.iso isodir

.PHONY: all run clean

