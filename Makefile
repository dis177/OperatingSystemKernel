# ======= 工具链 ============================================================
CROSS  = i686-elf
AS     = $(CROSS)-as
CC     = $(CROSS)-gcc
LD     = $(CROSS)-ld

CFLAGS  = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -T linker.ld -nostdlib

# ======= 对象文件 =========================================================
OBJS = boot.o kernel.o gdt.o idt.o tables.o pic.o pit.o isr.o isr_stub.o

# ======= 默认目标 =========================================================
all: myos.iso

# ---------- 单文件 → 对象文件 ----------
boot.o: boot.s
	$(AS) $< -o $@

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

gdt.o: gdt.c
	$(CC) $(CFLAGS) -c $< -o $@

idt.o: idt.c
	$(CC) $(CFLAGS) -c $< -o $@

tables.o: tables.c
	$(CC) $(CFLAGS) -c $< -o $@

isr_stub.o: isr_stub.s
	$(AS) $< -o $@

pic.o: pic.c
	$(CC) $(CFLAGS) -c $< -o $@

pit.o: pit.c
	$(CC) $(CFLAGS) -c $< -o $@

isr.o: isr.c
	$(CC) $(CFLAGS) -c $< -o $@
	
# ---------- 链接 → 内核二进制 ----------
myos.bin: $(OBJS) linker.ld
	$(LD) $(LDFLAGS) $(OBJS) -o $@

# ---------- 打包 ISO ----------
myos.iso: myos.bin grub.cfg
	mkdir -p isodir/boot/grub
	cp myos.bin   isodir/boot/
	cp grub.cfg   isodir/boot/grub/
	grub-mkrescue -o $@ isodir

# ---------- 运行 & 清理 ----------
run: myos.iso
	qemu-system-i386 -cdrom $<

clean:
	rm -rf *.o myos.bin myos.iso isodir

.PHONY: all run clean

