#/bin/sh

ip=""
if grep -qi microsoft /proc/version; then
    ip=$(cat /etc/resolv.conf | grep nameserver | awk '{print $2}')
fi;

echo target remote $ip:1234 > $1/.gdbinit
