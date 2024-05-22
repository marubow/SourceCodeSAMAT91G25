cmd_arch/arm/boot/at91sam9x25ek.dtb := /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/scripts/dtc/dtc -O dtb -o arch/arm/boot/at91sam9x25ek.dtb -b 0  -d arch/arm/boot/.at91sam9x25ek.dtb.d arch/arm/boot/dts/at91sam9x25ek.dts

source_arch/arm/boot/at91sam9x25ek.dtb := arch/arm/boot/dts/at91sam9x25ek.dts

deps_arch/arm/boot/at91sam9x25ek.dtb := \
  arch/arm/boot/dts/at91sam9x25.dtsi \
  arch/arm/boot/dts/at91sam9x5.dtsi \
  arch/arm/boot/dts/skeleton.dtsi \
  arch/arm/boot/dts/at91sam9x5ek.dtsi \
  arch/arm/boot/dts/at91sam9x5cm.dtsi \

arch/arm/boot/at91sam9x25ek.dtb: $(deps_arch/arm/boot/at91sam9x25ek.dtb)

$(deps_arch/arm/boot/at91sam9x25ek.dtb):
