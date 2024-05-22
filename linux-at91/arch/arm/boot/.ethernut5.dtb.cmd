cmd_arch/arm/boot/ethernut5.dtb := /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/scripts/dtc/dtc -O dtb -o arch/arm/boot/ethernut5.dtb -b 0  -d arch/arm/boot/.ethernut5.dtb.d arch/arm/boot/dts/ethernut5.dts

source_arch/arm/boot/ethernut5.dtb := arch/arm/boot/dts/ethernut5.dts

deps_arch/arm/boot/ethernut5.dtb := \
  arch/arm/boot/dts/at91sam9260.dtsi \
  arch/arm/boot/dts/skeleton.dtsi \

arch/arm/boot/ethernut5.dtb: $(deps_arch/arm/boot/ethernut5.dtb)

$(deps_arch/arm/boot/ethernut5.dtb):
