cmd_arch/arm/boot/tny_a9263.dtb := /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/scripts/dtc/dtc -O dtb -o arch/arm/boot/tny_a9263.dtb -b 0  -d arch/arm/boot/.tny_a9263.dtb.d arch/arm/boot/dts/tny_a9263.dts

source_arch/arm/boot/tny_a9263.dtb := arch/arm/boot/dts/tny_a9263.dts

deps_arch/arm/boot/tny_a9263.dtb := \
  arch/arm/boot/dts/at91sam9263.dtsi \
  arch/arm/boot/dts/skeleton.dtsi \

arch/arm/boot/tny_a9263.dtb: $(deps_arch/arm/boot/tny_a9263.dtb)

$(deps_arch/arm/boot/tny_a9263.dtb):
