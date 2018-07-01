AC_INIT(myconfig, 1.0)

AC_LANG(C)

dnl autoconf.h variables
TARGET_PLATFORM_FOLDER=""
ARCH_TYPE=0

dnl Makefile variables
target_prefix=""
gcc_flags=""
as_flags=""
ld_flags=""
n_flags=""

AC_ARG_ENABLE([debug], AS_HELP_STRING([--enable-debug], [Enable debugging support]))

AC_DEFUN(GET_TOOLCHAIN, [
AC_MSG_NOTICE([Downloading toolchain...])
git clone https://github.com/rm-hull/barebones-toolchain.git ~/barebones-toolchain
I686_TOOLS=~/barebones-toolchain/cross/`uname -m`/bin
echo export PATH='$PATH':${I686_TOOLS} >> ~/.bashrc
source ~/.bashrc
])

AC_DEFUN(CHECK_OTHER, [
AC_CHECK_PROG(NASM, nasm,    found, not found)
AC_CHECK_PROG(GITT, git,     found, not found)
AC_CHECK_PROG(XORR, xorriso, found, not found)
if [test x"${NASM}" == x"not found"] ; then
    sudo apt-get install -y nasm
fi
if [test x"${XORR}" == x"not found"] ; then
	sudo apt-get install -y xorriso
fi
if [test x"${GITT}" == x"not found"] ; then
	sudo apt-get install -y git
fi
])

AC_DEFUN(PLATFORM_x86, [
TARGET_PLATFORM_FOLDER="x86"
ARCH_TYPE=0
CHECK_OTHER
AC_CHECK_PROG(ELFGCC0, i686-elf-gcc, found, not found)
AC_CHECK_PROG(ELFGCC1, i686-elf-g++, found, not found)
AC_CHECK_PROG(ELFGCC2, i686-elf-ld,  found, not found)
AC_CHECK_PROG(ELFGCC3, i686-elf-as,  found, not found)
if [test x"${ELFGCC0}" == x"found"] && [test x"${ELFGCC1}" == x"found"] && [test x"${ELFGCC2}" == x"found"] && [test x"${ELFGCC3}" == x"found"] ; then
	AC_MSG_NOTICE([Found i686-elf toolchain])
	target_prefix="i686-elf-"
	gcc_flags="-ffreestanding -O3 -Wall -Wextra -Iinclude -fdiagnostics-color=auto -std=gnu99 -fno-omit-frame-pointer"
	ld_flags="-lgcc -nostdlib -ffreestanding"
	rm -rf kernelsrc/include/autoconf.h
	echo -e "#pragma once\n#define ARCH_TYPE "${ARCH_TYPE}"\n#define TARGET_FOLDER" ${TARGET_PLATFORM_FOLDER} > kernelsrc/include/autoconf.h
	
	AS_IF([test "x$enable_debug" != "xno"], [
		AC_MSG_NOTICE([Enabled debug features!])
		echo -e "\n#define DEBUG\n" >> kernelsrc/include/autoconf.h
		gcc_flags="-ffreestanding -Og -g -g3 -Wall -Wextra -Iinclude -fdiagnostics-color=auto -std=gnu99 -fno-omit-frame-pointer"
		ld_flags="-lgcc -nostdlib -ffreestanding -g"
		n_flags="-g"
		a_flags="-g"
	])
else
	AC_MSG_NOTICE([Did not find the i686-elf toolchain])
	GET_TOOLCHAIN
fi
])

AC_DEFUN(PLATFORM_raspi, [
TARGET_PLATFORM_FOLDER="raspi"
ARCH_TYPE=1
CHECK_OTHER
AC_CHECK_PROG(ELFGCC0, arm-none-eabi-gcc, found, not found)
AC_CHECK_PROG(ELFGCC1, arm-none-eabi-g++, found, not found)
AC_CHECK_PROG(ELFGCC2, arm-none-eabi-ld,  found, not found)
AC_CHECK_PROG(ELFGCC3, arm-none-eabi-as,  found, not found)
AC_CHECK_PROG(ELFGCC4, arm-none-eabi-objcopy,  found, not found)
if [test x"${ELFGCC0}" == x"found"] && [test x"${ELFGCC1}" == x"found"] && [test x"${ELFGCC2}" == x"found"] && [test x"${ELFGCC3}" == x"found"] && [test x"${ELFGCC4}" == x"found"] ; then
	AC_MSG_NOTICE([Found the arm-none-eabi toolchain])
	target_prefix="arm-none-eabi-"
	gcc_flags="-mcpu=arm1176jzf-s -fpic -ffreestanding -Iinclude -std=gnu99"
	as_flags="-mcpu=arm1176jzf-s"
	rm -rf kernelsrc/include/autoconf.h
	echo -e "#pragma once\n#define ARCH_TYPE "${ARCH_TYPE}"\n#define TARGET_FOLDER" ${TARGET_PLATFORM_FOLDER} > kernelsrc/include/autoconf.h
else
	AC_MSG_NOTICE([Did not find the arm-none-eabi toolchain])
	GET_TOOLCHAIN
fi
])

mkdir -p bin
case $target in
	raspi*)
	  AC_MSG_NOTICE([Selected raspi target platform (arm-none-eabi).])
	  PLATFORM_raspi
	  ;;
	i*86* | x86 | IA32)
	  AC_MSG_NOTICE([Selected x86 target platform (i686-elf).])
	  PLATFORM_x86
	  ;;
	*)
	  AC_MSG_NOTICE([No target platform specified. Going with x86...])
	  PLATFORM_x86
	  ;;
esac

AC_SUBST(PLATFORM_PREFIX, ["$target_prefix"])
AC_SUBST(PLATFORM_FLAGS, ["$gcc_flags"])
AC_SUBST(PLATFORM_SFLAGS, ["$as_flags"])
AC_SUBST(PLATFORM_AFLAGS, ["$ld_flags"])
AC_SUBST(PLATFORM_NFLAGS, ["$n_flags"])
AC_SUBST(PLATFORM_ID, ["$TARGET_PLATFORM_FOLDER"])

AC_CONFIG_FILES([Makefile kernelsrc/Makefile obj/Makefile grub/Makefile])
AC_CONFIG_COMMANDS
AC_OUTPUT
