cmd_arch/arm/boot/sama5d35ek.dtb := /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/scripts/dtc/dtc -O dtb -o arch/arm/boot/sama5d35ek.dtb -b 0  -d arch/arm/boot/.sama5d35ek.dtb.d arch/arm/boot/dts/sama5d35ek.dts

source_arch/arm/boot/sama5d35ek.dtb := arch/arm/boot/dts/sama5d35ek.dts

deps_arch/arm/boot/sama5d35ek.dtb := \
  arch/arm/boot/dts/sama5d3mb.dtsi \
  arch/arm/boot/dts/sama5d3cm.dtsi \
  arch/arm/boot/dts/sama5d3.dtsi \
  arch/arm/boot/dts/skeleton.dtsi \

arch/arm/boot/sama5d35ek.dtb: $(deps_arch/arm/boot/sama5d35ek.dtb)

$(deps_arch/arm/boot/sama5d35ek.dtb):
