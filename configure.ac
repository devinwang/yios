
# *****************************************************************
# The YingOS project.
# Copyright (C) 2004 Deng Wang

# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#
# ====================================================================
#
# Filename:     $RCSfile: configure.ac,v $
# Createdate:   2004/08/24 22:40:00
# FileId:       $Id: configure.ac,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
# Last update:  $Date: 2005/08/09 05:25:23 $
# Creator:      Devin Wang <vhd@21cn.com>
# Version:      $Revision: 1.1.1.1 $
# Description:
#
# ====================================================================

# ====================================================================
#  Revision:
#
# ====================================================================


AC_REVISION($Revision: 1.1.1.1 $)

AC_PREREQ(2.57)

AC_INIT(yios, 0.0.1, vhd@21cn.com)

AC_CONFIG_AUX_DIR(acauxdir)

AC_CANONICAL_SYSTEM


AM_INIT_AUTOMAKE(yios, 0.0.1)

AM_MAINTAINER_MODE



# AC_SUBST_FILE(CFG_FILE)
# CFG_FILE=$srcdir/config.tp/config.conf

#AC_SUBST(CPPFLAGS, ["-Wall -nostdinc -fno-zero-initialized-in-bss"])
#AC_SUBST(CPPFLAGS, ["-Wall -fno-zero-initialized-in-bss"])
# -fno-omit-frame-pointer"])

# AC_SUBST(CPPFLAGS, ["-Wall"])
#AC_SUBST(CPPFLAGS, ["-Wall -fomit-frame-pointer -nostdinc"])
# AC_SUBST(CCLDFLAGS, [-Wall --enable-stdcall-fixup])
# AC_SUBST(CCASFLAGS, [-Wall])

AC_SUBST(LIBTOOL, [])

case ${host} in
	i386-*-elf)
	AC_SUBST(AR, ${host_alias}-ar)
	AC_SUBST(LD, ${host_alias}-ld)
	break
	;;
	arm-*-elf)
	AC_SUBST(AR, ${host_alias}-ar)
	AC_SUBST(LD, ${host_alias}-ld)
	break
	;;
esac

# Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_CXXCPP
AM_PROG_AS
AC_PROG_RANLIB
AC_LIBOBJ

AC_PROG_AWK
AC_PROG_EGREP
AC_PROG_FGREP
AC_PROG_INSTALL
AC_PROG_LN_S



# AC_PROG_LIBTOOL

AC_SUBST(CPPFLAGS, ["-g -Wall -nostdinc"])
AC_SUBST(CFLAGS, ["-Wall -nostdinc"])
#AC_SUBST(CPPFLAGS, ["-g -Wall -fomit-frame-pointer -nostdinc"])
#AC_SUBST(CFLAGS, ["-Wall -fomit-frame-pointer -nostdinc"])
# AC_SUBST(CCASFLAGS, [-Wall])
AC_SUBST(INCLUDES, ["-I \${top_srcdir}/include -I \${top_builddir}/include"])

# Checks for libraries.

# Checks for header files.

# Checks for some files.
# AC_CHECK_FILE(config.tp/config.conf)

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN
AC_C_CONST
AC_C_INLINE
AC_C_CHAR_UNSIGNED
AC_C_LONG_DOUBLE
AC_C_STRINGIZE
AC_C_VOLATILE
AC_C_PROTOTYPES


dnl AC_CHECK_SIZEOF(char)
dnl AC_CHECK_SIZEOF(short int)
dnl AC_CHECK_SIZEOF(int)
dnl AC_CHECK_SIZEOF(long int)
dnl AC_CHECK_SIZEOF(long long int)

dnl AC_CHECK_SIZEOF(char*)

dnl AC_CHECK_SIZEOF(float)
dnl AC_CHECK_SIZEOF(double)


dnl AC_CHECK_SIZEOF(void)
dnl AC_CHECK_SIZEOF(void*)


# Checks for library functions.
# AC_CYGWIN
AC_EXEEXT
# AC_OBJEXT
# AC_MINGW32

AC_CONFIG_WITH_CFG_FILE


AC_CONFIG_SRCDIR([kernel/thread.c])
AM_CONFIG_HEADER([include/yios/config.h])

AC_CONFIG_FILES([Makefile tools/Makefile \
		boot/Makefile boot/i386/Makefile boot/arm/Makefile \
		kernel/Makefile \
		mm/Makefile \
		driver/Makefile \
        gui/Makefile \
		hal/Makefile \
		hal/i386/Makefile \
		hal/i386/debug/Makefile \
		hal/i386/init/Makefile \
		hal/i386/mm/Makefile \
		hal/i386/process/Makefile])
AC_CONFIG_LINKS(include/yios/hal:${srcdir}/include/yios/hal-i386)
AC_CONFIG_COMMANDS_POST([cp -f ${srcdir}/.gdbinit .gdbinit])
AC_CONFIG_COMMANDS_POST([mkdir -p hal/i386/debug])
AC_CONFIG_COMMANDS_POST([cp -f ${srcdir}/hal/i386/debug/asc16.fnt hal/i386/debug/asc16.fnt])
AC_CONFIG_COMMANDS_POST([cp -f ${srcdir}/hal/i386/debug/hzk16.fnt hal/i386/debug/hzk16.fnt])
AC_CONFIG_COMMANDS_POST([cp -f ${srcdir}/hal/i386/debug/novel.txt hal/i386/debug/novel.txt])
AC_OUTPUT

