cmd_usr/initramfs_data.o := /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/arm-2011.09/bin/arm-none-linux-gnueabi-gcc -Wp,-MD,usr/.initramfs_data.o.d  -nostdinc -isystem /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/arm-2011.09/bin/../lib/gcc/arm-none-linux-gnueabi/4.6.1/include -I/home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/arch/arm/include -Iarch/arm/include/generated -Iinclude  -include /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/include/linux/kconfig.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-at91/include -D__ASSEMBLY__ -mabi=aapcs-linux -mno-thumb-interwork -funwind-tables  -D__LINUX_ARM_ARCH__=5 -march=armv5te -mtune=arm9tdmi -include asm/unified.h -msoft-float     -DINITRAMFS_IMAGE="usr/initramfs_data.cpio"   -c -o usr/initramfs_data.o usr/initramfs_data.S

source_usr/initramfs_data.o := usr/initramfs_data.S

deps_usr/initramfs_data.o := \
    $(wildcard include/config/64bit.h) \
  /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/arch/arm/include/asm/unified.h \
    $(wildcard include/config/arm/asm/unified.h) \
    $(wildcard include/config/thumb2/kernel.h) \
  include/linux/stringify.h \
  include/asm-generic/vmlinux.lds.h \
    $(wildcard include/config/hotplug.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/event/tracing.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/ftrace/syscalls.h) \
    $(wildcard include/config/function/graph/tracer.h) \
    $(wildcard include/config/constructors.h) \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/pm/trace.h) \
    $(wildcard include/config/blk/dev/initrd.h) \

usr/initramfs_data.o: $(deps_usr/initramfs_data.o)

$(deps_usr/initramfs_data.o):
