cmd_arch/arm/boot/at91sam9g20ek_2mmc.dtb := /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/scripts/dtc/dtc -O dtb -o arch/arm/boot/at91sam9g20ek_2mmc.dtb -b 0  -d arch/arm/boot/.at91sam9g20ek_2mmc.dtb.d arch/arm/boot/dts/at91sam9g20ek_2mmc.dts

source_arch/arm/boot/at91sam9g20ek_2mmc.dtb := arch/arm/boot/dts/at91sam9g20ek_2mmc.dts

deps_arch/arm/boot/at91sam9g20ek_2mmc.dtb := \
  arch/arm/boot/dts/at91sam9g20ek_common.dtsi \
  arch/arm/boot/dts/at91sam9g20.dtsi \
  arch/arm/boot/dts/at91sam9260.dtsi \
  arch/arm/boot/dts/skeleton.dtsi \

arch/arm/boot/at91sam9g20ek_2mmc.dtb: $(deps_arch/arm/boot/at91sam9g20ek_2mmc.dtb)

$(deps_arch/arm/boot/at91sam9g20ek_2mmc.dtb):
