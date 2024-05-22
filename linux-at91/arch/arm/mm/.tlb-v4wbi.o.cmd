cmd_arch/arm/mm/tlb-v4wbi.o := /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/arm-2011.09/bin/arm-none-linux-gnueabi-gcc -Wp,-MD,arch/arm/mm/.tlb-v4wbi.o.d  -nostdinc -isystem /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/arm-2011.09/bin/../lib/gcc/arm-none-linux-gnueabi/4.6.1/include -I/home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/arch/arm/include -Iarch/arm/include/generated -Iinclude  -include /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/include/linux/kconfig.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-at91/include -D__ASSEMBLY__ -mabi=aapcs-linux -mno-thumb-interwork -funwind-tables  -D__LINUX_ARM_ARCH__=5 -march=armv5te -mtune=arm9tdmi -include asm/unified.h -msoft-float        -c -o arch/arm/mm/tlb-v4wbi.o arch/arm/mm/tlb-v4wbi.S

source_arch/arm/mm/tlb-v4wbi.o := arch/arm/mm/tlb-v4wbi.S

deps_arch/arm/mm/tlb-v4wbi.o := \
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
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/arch/arm/include/asm/types.h \
  include/asm-generic/int-ll64.h \
  arch/arm/include/generated/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/arch/arm/include/asm/asm-offsets.h \
  include/generated/asm-offsets.h \
  /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/arch/arm/include/asm/tlbflush.h \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/smp/on/up.h) \
    $(wildcard include/config/cpu/tlb/v4wt.h) \
    $(wildcard include/config/cpu/tlb/fa.h) \
    $(wildcard include/config/cpu/tlb/v4wbi.h) \
    $(wildcard include/config/cpu/tlb/feroceon.h) \
    $(wildcard include/config/cpu/tlb/v4wb.h) \
    $(wildcard include/config/cpu/tlb/v6.h) \
    $(wildcard include/config/cpu/tlb/v7.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/arm/errata/720789.h) \
  /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/arch/arm/include/asm/glue.h \
  arch/arm/mm/proc-macros.S \
    $(wildcard include/config/arm/lpae.h) \
    $(wildcard include/config/cpu/use/domains.h) \
    $(wildcard include/config/cpu/dcache/writethrough.h) \
    $(wildcard include/config/pm/sleep.h) \
  /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/arch/arm/include/asm/thread_info.h \
    $(wildcard include/config/arm/thumbee.h) \
  /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/arch/arm/include/asm/fpstate.h \
    $(wildcard include/config/vfpv3.h) \
    $(wildcard include/config/iwmmxt.h) \

arch/arm/mm/tlb-v4wbi.o: $(deps_arch/arm/mm/tlb-v4wbi.o)

$(deps_arch/arm/mm/tlb-v4wbi.o):
