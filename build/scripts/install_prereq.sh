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
        ${osInfo[$f]} install git
        ${osInfo[$f]} install nasm
        ${osInfo[$f]} install xorriso
        ${osInfo[$f]} install grub-pc grub-pc-bin grub-rescue-pc
    fi
done