#! /bin/bash
declare -A osInfo;
osInfo[/etc/redhat-release]=yum
osInfo[/etc/arch-release]=pacman
osInfo[/etc/gentoo-release]=emerge
osInfo[/etc/SuSE-release]=zypp
osInfo[/etc/debian_version]=apt-get

for f in ${!osInfo[@]}
do
    if [[ -f $f ]];then
        ${osInfo[$f]} install -y git nasm gcc-multilib g++-multilib
        ${osInfo[$f]} install -y xorriso
        ${osInfo[$f]} install -y quilt
        ${osInfo[$f]} install -y minicom
        ${osInfo[$f]} install -y grub-pc grub-pc-bin grub-rescue-pc
    fi
done

echo Downloading toolchain...
git clone https://github.com/rm-hull/barebones-toolchain.git toolchain

echo Now patching openlibm...

cd ../lib/
rm -rf openlibm
git clone https://github.com/JuliaMath/openlibm.git openlibm/
quilt push
