#!/bin/sh
rm -rf bin/isodir/
mkdir -p bin/isodir/boot/grub
tar -cvf bin/ramfs.tar `find -path "./grub/ramfs/*" -print`
echo ""
cp grub/grub.cfg  bin/isodir/boot/grub/grub.cfg
cp grub/themes/   bin/isodir/boot/grub/themes -r
cp bin/cxkrnl.bin bin/isodir/boot/cxkrnl.bin
cp bin/ramfs.tar  bin/isodir/boot/ramfs.tar
cp ../loader/obj/cxlodr.bin bin/isodir/boot/cxlodr.bin
grub-mkrescue -o bin/LiquiDOS.iso bin/isodir