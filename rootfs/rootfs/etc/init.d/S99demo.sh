#!/bin/sh

if [ -e /dev/sda1 ];then
if [ ! -e /media/usbhd-sda1/ ];then
mkdir -p /media/usbhd-sda1/
mount -t vfat /dev/sda1 /media/usbhd-sda1
sleep 2
fi
fi
if [ -x /media/usbhd-sda1/ ]; then
sleep 2
if [ -f /media/usbhd-sda1/BootUserDemo.sh ]; then
/bin/sh /media/usbhd-sda1/BootUserDemo.sh /media/usbhd-sda1/
fi
fi
