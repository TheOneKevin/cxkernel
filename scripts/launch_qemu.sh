#/bin/sh

qemu=""
if grep -qi microsoft /proc/version; then
    qemu=qemu-system-x86_64.exe;
    export QEMU_IP=$(cat /etc/resolv.conf | grep nameserver | awk '{print $2}')
else
    qemu=qemu-system-x86_64;
fi;

debug_args=""
run_async=""

if [ "$1" = "-debug" ]; then
    debug_args="-S -gdb tcp::1234"
    run_async=""
fi;

cd ../build/
$qemu $debug_args -boot d -cdrom boot.iso \
    -chardev stdio,id=char1,logfile=COM1.log,signal=on \
    -serial chardev:char1 -serial file:COM2.log -m 256 \
    -no-reboot
