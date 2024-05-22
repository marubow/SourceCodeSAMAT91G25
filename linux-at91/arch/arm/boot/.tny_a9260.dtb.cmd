cmd_arch/arm/boot/tny_a9260.dtb := /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/scripts/dtc/dtc -O dtb -o arch/arm/boot/tny_a9260.dtb -b 0  -d arch/arm/boot/.tny_a9260.dtb.d arch/arm/boot/dts/tny_a9260.dts

source_arch/arm/boot/tny_a9260.dtb := arch/arm/boot/dts/tny_a9260.dts

deps_arch/arm/boot/tny_a9260.dtb := \
  arch/arm/boot/dts/at91sam9260.dtsi \
  arch/arm/boot/dts/skeleton.dtsi \
  arch/arm/boot/dts/tny_a9260_common.dtsi \

arch/arm/boot/tny_a9260.dtb: $(deps_arch/arm/boot/tny_a9260.dtb)

$(deps_arch/arm/boot/tny_a9260.dtb):
