AC_INIT([cxkernel], [0.0.1])
AC_LANG([C])
AC_CONFIG_HEADERS([config.h])

AC_ARG_ENABLE([debug], AS_HELP_STRING([--enable-debug], [Enable debugging support]))

dnl Variables
TARGET=""
FOLDER=""
CXXFLAGS="-Wall -Wextra -ffreestanding -fdiagnostics-color=auto -fno-rtti -fno-exceptions -nostdinc++ "
CCFLAGS=" -Wall -Wextra -ffreestanding -fdiagnostics-color=auto "
ASFLAGS=""
NAFLAGS=""
LDFLAGS="-lgcc -nostdlib -ffreestanding -nodefaultlibs -nostdinc++ "

OBJDIR="build/obj"
BINDIR="build/bin"
INITRD=""
BITS=""

AC_DEFUN([CHECK_GCC], [
    AC_CHECK_PROG(ELFGCC0, [${TARGET}-gcc], found, not found)
    AC_CHECK_PROG(ELFGCC1, [${TARGET}-g++], found, not found)
    AC_CHECK_PROG(ELFGCC2, [${TARGET}-ld],  found, not found)
    AC_CHECK_PROG(ELFGCC3, [${TARGET}-as],  found, not found)
    AC_CHECK_PROG(ELFGCC4, [${TARGET}-objcopy], found, not found)
    if [test x"${ELFGCC0}" == x"found"] && [test x"${ELFGCC1}" == x"found"] && [test x"${ELFGCC2}" == x"found"] && [test x"${ELFGCC3}" == x"found"] && [test x"${ELFGCC4}" == x"found"] ; then
        AC_MSG_NOTICE([Found the ${TARGET} toolchain])
    else
        AC_MSG_ERROR([Cannot find cross compiler toolchain for ${TARGET}.])
    fi
])

AC_DEFUN([RUN_x86_32], [
    TARGET="i686-elf"
    FOLDER="x86"
    BITS="x86_32"
    CXXFLAGS+="-std=c++14 -fno-omit-frame-pointer "
    CCFLAGS+=" -std=gnu99 -fno-omit-frame-pointer "
    ASFLAGS+=""
    NAFLAGS+=""
    LDFLAGS+="-lgcc -nostdlib -ffreestanding"
    AC_DEFINE([ARCH_TYPE], [ARCH_x86_32], [The architecture type])
    AC_DEFINE([TARGET_FOLDER], [x86], [The target folder])
    CHECK_GCC
    AC_CHECK_PROG(NASM, nasm,    found, not found)
    AC_CHECK_PROG(GITT, git,     found, not found)
    AC_CHECK_PROG(XORR, xorriso, found, not found)
    if [test x"${NASM}" == x"not found"] || [test x"${XORR}" == x"not found"] || [test x"${GITT}" == x"not found"] ; then
        AC_MSG_ERROR([Cannot find required packages to build the OS.])
    fi
])

AS_IF([test "x$enable_debug" = "xyes"], [
    AC_MSG_NOTICE([Enabled debug features.])
    CXXFLAGS+="-Og -g -g3 "
    CCFLAGS+="-Og -g -g3 "
    NAFLAGS+="-F dwarf -g "
    ASFLAGS+="-g "
    LDFLAGS+="-g "
])

AC_DEFUN([PUSH_CORE], [
    mv core/ core_bk/
])

AC_DEFUN([POP_CORE], [
    mv core_bk/ core/
])

case $target in
    x86_64 | AMD64 | IA64 )
        AC_MSG_NOTICE([Target architecture: x86_64-elf])
        ;;
    i*86* | x86_32 | IA32 | x86)
        AC_MSG_NOTICE([Target architecture: i686-elf])
        RUN_x86_32
        ;;
    *)
        AC_MSG_NOTICE([Target architecture: i686-elf])
        RUN_x86_32
        ;;
esac

AC_SUBST(TARGET, ["$TARGET"])
AC_SUBST(FOLDER, ["$FOLDER"])
AC_SUBST(CCFLAGS, ["$CCFLAGS"])
AC_SUBST(CXXFLAGS, ["$CXXFLAGS"])
AC_SUBST(ASFLAGS, ["$ASFLAGS"])
AC_SUBST(NAFLAGS, ["$NAFLAGS"])
AC_SUBST(LDFLAGS, ["$LDFLAGS"])
AC_SUBST(OBJDIR, ["$OBJDIR"])
AC_SUBST(BINDIR, ["$BINDIR"])
AC_SUBST(INITRD, ["$INITRD"])
AC_SUBST(LDFOLDER, ["$BITS"])

AC_CONFIG_FILES([../Makefile Makefile ../loader/Makefile ../loader/obj/Makefile])
AC_CONFIG_COMMANDS
AC_OUTPUT
