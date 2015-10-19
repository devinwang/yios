
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
# Filename:     $RCSfile: acinclude.m4,v $
# Createdate:   2004/08/26 21:48:00
# FileId:       $Id: acinclude.m4,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
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





AC_DEFUN([AC_CONFIG_WITH_CFG_FILE],
[
	AC_MSG_NOTICE([disposing system configuration file for compiler])


	case $host in
	i[[34567]]86-*)
		AC_CONFIG_WITH_I386CFG_FILE
		break
		;;
	arm-*)
		AC_CONFIG_WITH_ARMCFG_FILE
		break
		;;
	*)
		AC_MSG_FAILURE(["host $host not support"])
		;;
	esac


])

AC_DEFUN([AC_CONFIG_WITH_I386CFG_FILE],
[
	. ${srcdir}/config.tp/i386.conf
	if test -z "$CFG_PLATFORM"; then
		AC_MSG_FAILURE(failure:"$CFG_PLATFORM")
	fi
	
    AC_SUBST(CFG_PLATFORM)

    AC_DEFINE([CFG_YIOS_KERNEL], [], [yios kernel])
    
	AC_DEFINE_UNQUOTED([CFG_PLATFORM],
				[${CFG_PLATFORM}],[platform])
	
    AC_DEFINE_UNQUOTED([CFG_DEBUG],
				[${CFG_DEBUG}],[debug])
    AM_CONDITIONAL(CFG_DEBUG, test ${CFG_DEBUG} = 1)
	
    AC_DEFINE_UNQUOTED([CFG_DEBUG_METHOD_SERIAL_PORT],
				[${CFG_DEBUG_METHOD_SERIAL_PORT}],[debug method port])
    
    AM_CONDITIONAL(CFG_DEBUG_METHOD_SERIAL_PORT, 
            [test ${CFG_DEBUG_METHOD_SERIAL_PORT} = 1])

    AC_DEFINE_UNQUOTED([CFG_DEBUG_METHOD_MESSAGE],
				[${CFG_DEBUG_METHOD_MESSAGE}],[debug method message])
	
	AC_DEFINE([CFG_KERNEL], [], [kernel])
	
	AC_DEFINE_UNQUOTED([CFG_KERNEL_COMPRESSED],
				[${CFG_KERNEL_COMPRESSED}],[kernel compressed])
	AC_DEFINE_UNQUOTED([CFG_KERNEL_OSKERNELSYS_PADDR],
				[${CFG_KERNEL_OSKERNELSYS_PADDR}],[oskernel.sys paddr])

	AC_DEFINE_UNQUOTED([CFG_KERNEL_IMAGE_PADDR],
				[${CFG_KERNEL_IMAGE_PADDR}], [kernel image paddr])

    AC_DEFINE_UNQUOTED([CFG_INT_MAX_INTERRUPTER],
                [${CFG_INT_MAX_INTERRUPTER}], [max interrupter])

	AC_DEFINE_UNQUOTED([CFG_KERNEL_SCHED_METHOD_RM],
				[${CFG_KERNEL_SCHED_METHOD_RM}],[sched method rate mute])
    AM_CONDITIONAL(CFG_KERNEL_SCHED_METHOD_RM, test ${CFG_KERNEL_SCHED_METHOD_RM} = 1)

	AC_DEFINE_UNQUOTED([CFG_KERNEL_SCHED_METHOD_TIMESLICE],
				[${CFG_KERNEL_SCHED_METHOD_TIMESLICE}],[sched method timeslice])
    AM_CONDITIONAL(CFG_KERNEL_SCHED_METHOD_TIMESLICE, test ${CFG_KERNEL_SCHED_METHOD_TIMESLICE} = 1)

	AC_DEFINE_UNQUOTED([CFG_KERNEL_SCHED_MAX_THREAD],
				[${CFG_KERNEL_SCHED_MAX_THREAD}],[thread maximum])
	AC_DEFINE_UNQUOTED([CFG_KERNEL_SCHED_MAX_PRIORITY],
				[${CFG_KERNEL_SCHED_BITMAP_MAX_PRIORITY}],[priority maximum])

	AC_DEFINE_UNQUOTED([CFG_KERNEL_SCHED_HAVE_THREAD_RELATIONSHIP],
				[${CFG_KERNEL_SCHED_HAVE_THREAD_RELATIONSHIP}],[thread relationship])

	AC_DEFINE_UNQUOTED([CFG_KERNEL_SCHED_HZ],
				[${CFG_KERNEL_SCHED_HZ}],[kernel sched HZ])
# configure memory management
	AC_DEFINE_UNQUOTED([CFG_MM_MM],
				[${CFG_MM_MM}],[memory management])
	AM_CONDITIONAL(CFG_MM_MM, test ${CFG_MM_MM} = 1)
	AC_DEFINE_UNQUOTED([CFG_MM_MMU],
				[${CFG_MM_MMU}],[memory management unit])

	AM_CONDITIONAL(CFG_MM_MMU, test ${CFG_MM_MMU} = 1)

# confingure file system

	AC_DEFINE_UNQUOTED([CFG_FS],
				[${CFG_FS}],[file system support])

    AM_CONDITIONAL(CFG_FS, test ${CFG_FS} = 1)

    AC_DEFINE_UNQUOTED([CFG_FS_TFS],
                [${CFG_FS_TFS}],[tiny file system support])

    AC_DEFINE_UNQUOTED([CFG_FS_TFS_MAX_FILE],
                [${CFG_FS_TFS_MAX_FILE}],[max file number in tiny file system.])

    AC_DEFINE_UNQUOTED([CFG_FS_PROCESS_MAX_FD],
                [${CFG_FS_PROCESS_MAX_FD}], [max fd per process])

# configure GUI
    AC_DEFINE_UNQUOTED([CFG_GUI],
                [${CFG_GUI}], [gui])
    AM_CONDITIONAL(CFG_GUI, test ${CFG_GUI} -ne 0)

    
    AC_DEFINE_UNQUOTED([CFG_VBE],
                [${CFG_VBE}], [vbe])
    AM_CONDITIONAL(CFG_VBE, test ${CFG_VBE} -ne 0)
    
    AC_DEFINE_UNQUOTED([CFG_VBE_MODE],
                [${CFG_VBE_MODE}], [vbe mode])
    
    AC_DEFINE_UNQUOTED([CFG_LANG_GB2312],
                [${CFG_LANG_GB2312}], [gb2312 support])
# configure microwindows support
    AC_DEFINE_UNQUOTED([CFG_MW],
                [${CFG_MW}], [microwindows support])
    AM_CONDITIONAL(CFG_MW, test ${CFG_MW} -ne 0)

# drivers support
    AC_DEFINE_UNQUOTED([CFG_DRIVER_NKB],
                [${CFG_DRIVER_NKB}], [normal keybord])
    AM_CONDITIONAL(CFG_DRIVER_NKB, test ${CFG_DRIVER_NKB} -ne 0)

    AC_DEFINE_UNQUOTED([CFG_DRIVER_PS2MOUSE],
                [${CFG_DRIVER_PS2MOUSE}], [ps2 mouse])
    AM_CONDITIONAL(CFG_DRIVER_PS2MOUSE, test ${CFG_DRIVER_PS2MOUSE} -ne 0)
 
])


AC_DEFUN([AC_CONFIG_WITH_ARMCFG_FILE],
[
	. $srcdir/config.tp/arm.conf
	if test -z "$CFG_PLATFORM"; then
		AC_MSG_FAILURE(failure:"$CFG_PLATFORM")
	fi
    
    AC_SUBST(CFG_PLATFORM)

    AC_DEFINE([CFG_YIOS_KERNEL], [], [yios kernel])
    
	AC_DEFINE_UNQUOTED([CFG_PLATFORM],
				[${CFG_PLATFORM}],[platform])
	
    AC_DEFINE_UNQUOTED([CFG_DEBUG],
				[${CFG_DEBUG}],[debug])
    AM_CONDITIONAL(CFG_DEBUG, test ${CFG_DEBUG} = 1)
	
    AC_DEFINE_UNQUOTED([CFG_DEBUG_METHOD_SERIAL_PORT],
				[${CFG_DEBUG_METHOD_SERIAL_PORT}],[debug method port])
    
    AM_CONDITIONAL(CFG_DEBUG_METHOD_SERIAL_PORT, 
            [test ${CFG_DEBUG_METHOD_SERIAL_PORT} = 1])

    AC_DEFINE_UNQUOTED([CFG_DEBUG_METHOD_MESSAGE],
				[${CFG_DEBUG_METHOD_MESSAGE}],[debug method message])
	
	AC_DEFINE([CFG_KERNEL], [], [kernel])
	
	AC_DEFINE_UNQUOTED([CFG_KERNEL_COMPRESSED],
				[${CFG_KERNEL_COMPRESSED}],[kernel compressed])
	AC_DEFINE_UNQUOTED([CFG_KERNEL_OSKERNELSYS_PADDR],
				[${CFG_KERNEL_OSKERNELSYS_PADDR}],[oskernel.sys paddr])

	AC_DEFINE_UNQUOTED([CFG_KERNEL_IMAGE_PADDR],
				[${CFG_KERNEL_IMAGE_PADDR}], [kernel image paddr])

    AC_DEFINE_UNQUOTED([CFG_INT_MAX_INTERRUPTER],
                [${CFG_INT_MAX_INTERRUPTER}], [max interrupter])


	AC_DEFINE_UNQUOTED([CFG_KERNEL_SCHED_METHOD_RM],
				[${CFG_KERNEL_SCHED_METHOD_RM}],[rate monotonic sched method])


	AC_DEFINE_UNQUOTED([CFG_KERNEL_SCHED_METHOD_TIMESLICE],
				[${CFG_KERNEL_SCHED_METHOD_TIMESLICE}],[sched method timeslice])

	AC_DEFINE_UNQUOTED([CFG_KERNEL_SCHED_MAX_THREAD],
				[${CFG_KERNEL_SCHED_MAX_THREAD}],[thread maximum])
	AC_DEFINE_UNQUOTED([CFG_KERNEL_SCHED_MAX_PRIORITY],
				[${CFG_KERNEL_SCHED_BITMAP_MAX_PRIORITY}],[priority maximum])

	AC_DEFINE_UNQUOTED([CFG_KERNEL_SCHED_HAVE_THREAD_RELATIONSHIP],
				[${CFG_KERNEL_SCHED_HAVE_THREAD_RELATIONSHIP}],[thread relationship])

	AC_DEFINE_UNQUOTED([CFG_KERNEL_SCHED_HZ],
				[${CFG_KERNEL_SCHED_HZ}],[kernel sched HZ])

	AC_DEFINE_UNQUOTED([CFG_FS],
				[${CFG_FS}],[file system support])

    AM_CONDITIONAL(CFG_FS, test ${CFG_FS} = 1)

    AC_DEFINE_UNQUOTED([CFG_FS_TFS],
                [${CFG_FS_TFS}],[tiny file system support])

    AC_DEFINE_UNQUOTED([CFG_FS_TFS_MAX_FILE],
                [${CFG_FS_TFS_MAX_FILE}],[max file number in tiny file system.])

    AC_DEFINE_UNQUOTED([CFG_FS_PROCESS_MAX_FD],
                [${CFG_FS_PROCESS_MAX_FD}], [max fd per process])

# configure GUI
    AC_DEFINE_UNQUOTED([CFG_GUI],
                [${CFG_GUI}], [gui])
    AM_CONDITIONAL(CFG_GUI, test ${CFG_GUI} -ne 0)

    
    AC_DEFINE_UNQUOTED([CFG_VBE],
                [${CFG_VBE}], [vbe])
    AM_CONDITIONAL(CFG_VBE, test ${CFG_VBE} -ne 0)
    
    AC_DEFINE_UNQUOTED([CFG_VBE_MODE],
                [${CFG_VBE_MODE}], [vbe mode])
    
    AC_DEFINE_UNQUOTED([CFG_LANG_GB2312],
                [${CFG_LANG_GB2312}], [gb2312 support])

# drivers support
    AC_DEFINE_UNQUOTED([CFG_DRIVER_NKB],
                [${CFG_DRIVER_NKB}], [normal keybord])
    AM_CONDITIONAL(CFG_DRIVER_NKB, test ${CFG_DRIVER_NKB} -ne 0)

])


