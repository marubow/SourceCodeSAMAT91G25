cmd_arch/arm/boot/tny_a9g20.dtb := /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/scripts/dtc/dtc -O dtb -o arch/arm/boot/tny_a9g20.dtb -b 0  -d arch/arm/boot/.tny_a9g20.dtb.d arch/arm/boot/dts/tny_a9g20.dts

source_arch/arm/boot/tny_a9g20.dtb := arch/arm/boot/dts/tny_a9g20.dts

deps_arch/arm/boot/tny_a9g20.dtb := \
  arch/arm/boot/dts/at91sam9g20.dtsi \
  arch/arm/boot/dts/at91sam9260.dtsi \
  arch/arm/boot/dts/skeleton.dtsi \
  arch/arm/boot/dts/tny_a9260_common.dtsi \

arch/arm/boot/tny_a9g20.dtb: $(deps_arch/arm/boot/tny_a9g20.dtb)

$(deps_arch/arm/boot/tny_a9g20.dtb):
