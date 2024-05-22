cmd_arch/arm/boot/at91sam9g35ek.dtb := /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/scripts/dtc/dtc -O dtb -o arch/arm/boot/at91sam9g35ek.dtb -b 0  -d arch/arm/boot/.at91sam9g35ek.dtb.d arch/arm/boot/dts/at91sam9g35ek.dts

source_arch/arm/boot/at91sam9g35ek.dtb := arch/arm/boot/dts/at91sam9g35ek.dts

deps_arch/arm/boot/at91sam9g35ek.dtb := \
  arch/arm/boot/dts/at91sam9g35.dtsi \
  arch/arm/boot/dts/at91sam9x5.dtsi \
  arch/arm/boot/dts/skeleton.dtsi \
  arch/arm/boot/dts/at91sam9x5ek.dtsi \
  arch/arm/boot/dts/at91sam9x5cm.dtsi \

arch/arm/boot/at91sam9g35ek.dtb: $(deps_arch/arm/boot/at91sam9g35ek.dtb)

$(deps_arch/arm/boot/at91sam9g35ek.dtb):
