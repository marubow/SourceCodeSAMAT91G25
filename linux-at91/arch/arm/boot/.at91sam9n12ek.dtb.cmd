cmd_arch/arm/boot/at91sam9n12ek.dtb := /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/scripts/dtc/dtc -O dtb -o arch/arm/boot/at91sam9n12ek.dtb -b 0  -d arch/arm/boot/.at91sam9n12ek.dtb.d arch/arm/boot/dts/at91sam9n12ek.dts

source_arch/arm/boot/at91sam9n12ek.dtb := arch/arm/boot/dts/at91sam9n12ek.dts

deps_arch/arm/boot/at91sam9n12ek.dtb := \
  arch/arm/boot/dts/at91sam9n12.dtsi \
  arch/arm/boot/dts/skeleton.dtsi \

arch/arm/boot/at91sam9n12ek.dtb: $(deps_arch/arm/boot/at91sam9n12ek.dtb)

$(deps_arch/arm/boot/at91sam9n12ek.dtb):
