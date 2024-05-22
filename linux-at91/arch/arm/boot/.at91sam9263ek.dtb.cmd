cmd_arch/arm/boot/at91sam9263ek.dtb := /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/scripts/dtc/dtc -O dtb -o arch/arm/boot/at91sam9263ek.dtb -b 0  -d arch/arm/boot/.at91sam9263ek.dtb.d arch/arm/boot/dts/at91sam9263ek.dts

source_arch/arm/boot/at91sam9263ek.dtb := arch/arm/boot/dts/at91sam9263ek.dts

deps_arch/arm/boot/at91sam9263ek.dtb := \
  arch/arm/boot/dts/at91sam9263.dtsi \
  arch/arm/boot/dts/skeleton.dtsi \

arch/arm/boot/at91sam9263ek.dtb: $(deps_arch/arm/boot/at91sam9263ek.dtb)

$(deps_arch/arm/boot/at91sam9263ek.dtb):
