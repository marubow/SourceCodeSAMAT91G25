cmd_arch/arm/boot/aks-cdu.dtb := /home/boardcon/GD01D2_SW/DocumentData/DocumentProject/Linux/Source/linux-at91/scripts/dtc/dtc -O dtb -o arch/arm/boot/aks-cdu.dtb -b 0  -d arch/arm/boot/.aks-cdu.dtb.d arch/arm/boot/dts/aks-cdu.dts

source_arch/arm/boot/aks-cdu.dtb := arch/arm/boot/dts/aks-cdu.dts

deps_arch/arm/boot/aks-cdu.dtb := \
  arch/arm/boot/dts/ge863-pro3.dtsi \
  arch/arm/boot/dts/at91sam9260.dtsi \
  arch/arm/boot/dts/skeleton.dtsi \

arch/arm/boot/aks-cdu.dtb: $(deps_arch/arm/boot/aks-cdu.dtb)

$(deps_arch/arm/boot/aks-cdu.dtb):
