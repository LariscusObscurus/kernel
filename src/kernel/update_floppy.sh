#!/bin/bash
if [[ $EUID -ne 0 ]]; then
    echo "Must be run as root!" 1>&2
    exit 1
fi
[[ -d /mnt/floppy ]] || mkdir /mnt/floppy

mount -oloop floppy.img /mnt/floppy
if [ "$?" != "0" ]; then
    exit 1
fi
cp kernel /mnt/floppy/kernel.bin
sync
umount /mnt/floppy


