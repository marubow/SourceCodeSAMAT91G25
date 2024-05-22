cmd_arch/arm/boot/kizbox.dtb := /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/scripts/dtc/dtc -O dtb -o arch/arm/boot/kizbox.dtb -b 0  -d arch/arm/boot/.kizbox.dtb.d arch/arm/boot/dts/kizbox.dts

source_arch/arm/boot/kizbox.dtb := arch/arm/boot/dts/kizbox.dts

deps_arch/arm/boot/kizbox.dtb := \
  arch/arm/boot/dts/at91sam9g20.dtsi \
  arch/arm/boot/dts/at91sam9260.dtsi \
  arch/arm/boot/dts/skeleton.dtsi \

arch/arm/boot/kizbox.dtb: $(deps_arch/arm/boot/kizbox.dtb)

$(deps_arch/arm/boot/kizbox.dtb):
