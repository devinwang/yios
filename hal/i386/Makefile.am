
## ====================================================================
## The YingOS project.
## Copyright (C) 2004 Devin Wang

## This program is free software; you can redistribute it and/or
## modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2
## of the License, or (at your option) any later version.

## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.

## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
##
## ====================================================================
##
## Filename:	$RCSfile: Makefile.am,v $
## Createdate:	2004/09/10 00:40:00
## FileId:	$Id: Makefile.am,v 1.1.1.1 2005/08/09 05:25:24 devin Exp $
## Last update:	$Date: 2005/08/09 05:25:24 $
## Creator:	Devin Wang <vhd@21cn.com>
## Version:	$Revision: 1.1.1.1 $
## Description: 
##
## ====================================================================


## ====================================================================
##  Revision:
##
## ====================================================================



AUTOMAKE_OPTIONS = foreign


if CFG_DEBUG
APP_DIRS = debug
APP_FILES = debug/libdebug.a
endif


OSKERNEL = oskernel.exe

SUBDIRS = init mm process $(APP_DIRS)

# EXTRA_PROGRAMS = oskernel.sys$(EXEEXT)
noinst_DATA = oskernel.gz
noinst_PROGRAMS = $(OSKERNEL) oskernel.gz oskernel.sys

LDFLAGS=
CFLAGS=
CCLD = $(LD)

oskernel_exe_CCASFLAGS = $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(CPPFLAGS)
#$(CPPFLAGS) $(CFLAGS)

oskernel_exe_LDADD = ../../kernel/libkernel.a ../../mm/libmm.a \
                    ../../driver/libdriver.a ../../gui/libgui.a \
			mm/libmm.a process/libprocess.a $(APP_FILES)
oskernel_exe_SOURCES = init/start32.S
oskernel_exe_LDFLAGS = --whole-archive -T ${srcdir}/oskernel.lds
oskernel_exe_DEPENDENCIES = ../../kernel/libkernel.a ../../mm/libmm.a \
                    ../../driver/libdriver.a ../../gui/libgui.a \
			mm/libmm.a process/libprocess.a $(APP_FILES)

oskernel.sys$(EXEEXT): init/init16 init/init32 oskernel.gz$(EXEEXT)
	../../tools/bkernel init/init16 init/init32 oskernel.gz

oskernel.gz$(EXEEXT): $(OSKERNEL)
	gzip -9 -c oskernel.exe >oskernel.gz


