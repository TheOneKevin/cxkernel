rm -rf $1/iso
mkdir -p $1/iso/boot/grub
cp $2/grub.cfg $1/iso/boot/grub
cp $1/cxloader.bin $1/iso/boot
cp $1/cxkernel.sys $1/iso/boot
grub-mkrescue -o $1/boot.iso $1/iso
