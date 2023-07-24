#/bin/sh

qemu=""
if grep -qi microsoft /proc/version; then
    qemu=qemu-system-x86_64.exe;
else
    qemu=qemu-system-x86_64;
fi;

cd ../build/
$qemu -boot d -cdrom boot.iso \
    -chardev stdio,id=char1,logfile=COM1.log,signal=on \
    -serial chardev:char1 -serial file:COM2.log -m 8
