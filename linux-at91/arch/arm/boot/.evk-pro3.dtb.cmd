cmd_arch/arm/boot/evk-pro3.dtb := /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/scripts/dtc/dtc -O dtb -o arch/arm/boot/evk-pro3.dtb -b 0  -d arch/arm/boot/.evk-pro3.dtb.d arch/arm/boot/dts/evk-pro3.dts

source_arch/arm/boot/evk-pro3.dtb := arch/arm/boot/dts/evk-pro3.dts

deps_arch/arm/boot/evk-pro3.dtb := \
  arch/arm/boot/dts/ge863-pro3.dtsi \
  arch/arm/boot/dts/at91sam9260.dtsi \
  arch/arm/boot/dts/skeleton.dtsi \

arch/arm/boot/evk-pro3.dtb: $(deps_arch/arm/boot/evk-pro3.dtb)

$(deps_arch/arm/boot/evk-pro3.dtb):
