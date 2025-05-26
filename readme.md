# MyOS —— 32 位 x86 裸机内核（初版说明）

这是一个用 **C 语言 + 汇编** 编写的最小化操作系统内核示例，目标平台为 **IA‑32（x86 32 位）**。编译完成后可由 **GRUB 2** 启动，在 QEMU 虚拟机里显示“Hello, kernel!”文字，为后续深入 OS 开发打下基础。

---

## 文件结构

编译前项目根目录只包含源码与配置文件；执行 `make` 后生成的所有产物会直接放在同一目录，便于快速查看。

```
boot.s       # Multiboot 头 + _start 入口（汇编）
kernel.c     # C 语言内核主体
linker.ld    # 链接脚本，定义节布局与入口符号
grub.cfg     # GRUB 菜单配置
Makefile     # 构建 / 运行 / 清理 规则

# ↓ 以下文件由 make 生成 ↓
boot.o       # 汇编对象文件
kernel.o     # C 对象文件
myos.bin     # 链接后的内核二进制
myos.iso     # 可启动 ISO（供 QEMU / 物理机）
```

---

## 环境准备（Ubuntu 22.04 LTS 及以上）

```bash
sudo apt update
sudo apt install build-essential bison flex libgmp-dev libmpfr-dev \
                 libmpc-dev texinfo libisl-dev xorriso grub-pc-bin \
                 nasm qemu-system-i386 curl
```

### 构建 i686‑elf 交叉工具链

```bash
# 设置安装路径
export PREFIX="/usr/local/i686-elf"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

# 下载并编译 binutils
mkdir -p ~/opt/cross
cd ~/opt/cross
curl -O https://ftp.gnu.org/gnu/binutils/binutils-2.40.tar.xz
tar -xf binutils-2.40.tar.xz
mkdir binutils-build
cd binutils-build
../binutils-2.40/configure --target=$TARGET --prefix=$PREFIX --with-sysroot --disable-nls --disable-werror
make -j$(nproc)
sudo make install

# 下载并编译 gcc
cd ~/opt/cross
curl -O https://ftp.gnu.org/gnu/gcc/gcc-13.1.0/gcc-13.1.0.tar.xz
tar -xf gcc-13.1.0.tar.xz
cd gcc-13.1.0
./contrib/download_prerequisites
mkdir ../gcc-build
cd ../gcc-build
../gcc-13.1.0/configure --target=$TARGET --prefix=$PREFIX --disable-nls --enable-languages=c,c++ --without-headers
make -j$(nproc) all-gcc
make -j$(nproc) all-target-libgcc
sudo make install-gcc
sudo make install-target-libgcc
```

验证：

```bash
i686-elf-gcc --version
```

---

## 构建与运行

```bash
make        # 生成 boot.o kernel.o myos.bin myos.iso
make run    # 使用 QEMU 启动 ISO
make clean  # 删除所有产物
```

`make run` 等价于：

```bash
qemu-system-i386 -cdrom myos.iso
```

---

## 工作流程概览

1. **boot.s** 含 Multiboot v1 头；GRUB 加载内核到 2 MiB 并跳转 `_start`。
2. `_start` 建立临时栈后调用 `kernel_main`（C）。
3. `kernel_main` 直接向 VGA 文本缓冲 **0xB8000** 写字符串，然后无限 `hlt`。

---

## 进阶方向

* 重新映射 PIC、编写中断服务程序（ISR）。
* 启用分页并实现物理 / 虚拟内存管理。
* 编写基本设备驱动：PIT 定时器、PS/2 键盘、串口调试等。
* 设计简易文件系统或读取 FAT12，并加载用户程序。
* 切换到  ring‑3，添加系统调用接口。

---

## 许可证

本仓库采用 **MIT License**。根据需要可自由修改。
