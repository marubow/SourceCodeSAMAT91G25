cmd_arch/arm/boot/sama5d33ek_pda.dtb := /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/scripts/dtc/dtc -O dtb -o arch/arm/boot/sama5d33ek_pda.dtb -b 0  -d arch/arm/boot/.sama5d33ek_pda.dtb.d arch/arm/boot/dts/sama5d33ek_pda.dts

source_arch/arm/boot/sama5d33ek_pda.dtb := arch/arm/boot/dts/sama5d33ek_pda.dts

deps_arch/arm/boot/sama5d33ek_pda.dtb := \
  arch/arm/boot/dts/sama5d3mb.dtsi \
  arch/arm/boot/dts/sama5d3cm.dtsi \
  arch/arm/boot/dts/sama5d3.dtsi \
  arch/arm/boot/dts/skeleton.dtsi \
  arch/arm/boot/dts/sama5d3pda.dtsi \

arch/arm/boot/sama5d33ek_pda.dtb: $(deps_arch/arm/boot/sama5d33ek_pda.dtb)

$(deps_arch/arm/boot/sama5d33ek_pda.dtb):
