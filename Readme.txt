
(1) Install compiler and tools£º

	# tar xvfj arm-2011.09-70-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2

	# sudo apt-get install build-essential
	# sudo apt-get install libncurses5-dev
	# sudo apt-get install mtd-utils
	# sudo apt-get install u-boot-tools
	
	My compiler directory is:

	CROSS_COMPILE=/home/boardcon/arm-2011.09/bin/arm-none-linux-gnueabi-
		
(2) Compile AT91Bootstrap:

	# tar xvfj at91bootstrap-master.tar.bz2
	# cd at91bootstrap-master
	# make mrproper
	
     if 128M DDR2 RAM:
	# make ARCH=arm CROSS_COMPILE=/home/boardcon/arm-2011.09/bin/arm-none-linux-gnueabi- at91sam9x5eknf_uboot_defconfig

     if 256M DDR2 RAM:
	# make ARCH=arm CROSS_COMPILE=/home/boardcon/arm-2011.09/bin/arm-none-linux-gnueabi- at91sam9x5eknf_uboot_256M_defconfig

	Compile£º
	
	# make -j4 ARCH=arm CROSS_COMPILE=/home/boardcon/arm-2011.09/bin/arm-none-linux-gnueabi-
	
	Get image£º

		binaries/at91sam9x5ek-nandflashboot-uboot-3.5.3.bin


(3) Compile uboot:

	# tar xvfj u-boot-at91-master.tar.bz2
	# cd u-boot-at91-master/
	# make ARCH=arm CROSS_COMPILE=/home/boardcon/arm-2011.09/bin/arm-none-linux-gnueabi- distclean

    if 128M DDR2 RAM:
	# make ARCH=arm CROSS_COMPILE=/home/boardcon/arm-2011.09/bin/arm-none-linux-gnueabi- at91sam9x5ek_nandflash_config

    if 256M DDR2 RAM:

	# make ARCH=arm CROSS_COMPILE=/home/boardcon/arm-2011.09/bin/arm-none-linux-gnueabi- at91sam9x5ek_nandflash_256M_config

	Compile£º
	
	# make ARCH=arm CROSS_COMPILE=/home/boardcon/arm-2011.09/bin/arm-none-linux-gnueabi- -j4 at91sam9x5ek_nandflash

	Get image£º

		u-boot.bin 


(4) Compile kernel£º

	# tar xvfj linux-at91.tar.bz2
	# cd linux-at91
	# make ARCH=arm CROSS_COMPILE=/home/boardcon/arm-2011.09/bin/arm-none-linux-gnueabi- distclean
	# make ARCH=arm CROSS_COMPILE=/home/boardcon/arm-2011.09/bin/arm-none-linux-gnueabi- at91_dt_defconfig

		if you want to create you own default configuration simple copy the new .config file 
		in arch/arm/configs with a name like at91_dt_defconfig.

		the device tree source file (.dts) for your board is in arch/arm/boot/dts/at91sam9g25ek.dts.

		then compile it to generate a device tree blob file (.dtb) by typing:

	Compile device tree£º

	# make ARCH=arm CROSS_COMPILE=/home/boardcon/arm-2011.09/bin/arm-none-linux-gnueabi- dtbs

		This file will be used by the Linux Kernel at startup time to configure the initial state of your hardware.

	# make ARCH=arm menuconfig(if you want to config the kernel)

	# make -j4 ARCH=arm CROSS_COMPILE=/home/boardcon/arm-2011.09/bin/arm-none-linux-gnueabi- uImage

	Get images£º

		arch/arm/boot/dts/at91sam9g25ek.dtb
		arch/arm/boot/uImage


(5) Compile rootfs£º

	# sudo tar xvfj rootfs.tar.bz2
	# cd rootfs
	# sudo ./mkubifsimage

	Get images£º

		ubifs.img
















