cmd_arch/arm/mm/pabort-legacy.o := /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/arm-2011.09/bin/arm-none-linux-gnueabi-gcc -Wp,-MD,arch/arm/mm/.pabort-legacy.o.d  -nostdinc -isystem /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/arm-2011.09/bin/../lib/gcc/arm-none-linux-gnueabi/4.6.1/include -I/home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/arch/arm/include -Iarch/arm/include/generated -Iinclude  -include /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/include/linux/kconfig.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-at91/include -D__ASSEMBLY__ -mabi=aapcs-linux -mno-thumb-interwork -funwind-tables  -D__LINUX_ARM_ARCH__=5 -march=armv5te -mtune=arm9tdmi -include asm/unified.h -msoft-float        -c -o arch/arm/mm/pabort-legacy.o arch/arm/mm/pabort-legacy.S

source_arch/arm/mm/pabort-legacy.o := arch/arm/mm/pabort-legacy.S

deps_arch/arm/mm/pabort-legacy.o := \
  /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/arch/arm/include/asm/unified.h \
    $(wildcard include/config/arm/asm/unified.h) \
    $(wildcard include/config/thumb2/kernel.h) \
  include/linux/linkage.h \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/arch/arm/include/asm/linkage.h \
  /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/arch/arm/include/asm/assembler.h \
    $(wildcard include/config/cpu/feroceon.h) \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/cpu/use/domains.h) \
  /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/arch/arm/include/asm/ptrace.h \
    $(wildcard include/config/cpu/endian/be8.h) \
    $(wildcard include/config/arm/thumb.h) \
  /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/arch/arm/include/asm/hwcap.h \
  /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/arch/arm/include/asm/domain.h \
    $(wildcard include/config/io/36.h) \

arch/arm/mm/pabort-legacy.o: $(deps_arch/arm/mm/pabort-legacy.o)

$(deps_arch/arm/mm/pabort-legacy.o):
