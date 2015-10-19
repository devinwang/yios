
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
## Createdate:	2004/09/10 00:43:00
## FileId:	$Id: Makefile.am,v 1.1.1.1 2005/08/09 05:25:25 devin Exp $
## Last update:	$Date: 2005/08/09 05:25:25 $
## Creator:	Devin Wang <vhd@21cn.com>
## Version:	$Revision: 1.1.1.1 $
## Description: 
##
## ====================================================================


## ====================================================================
##  Revision:
##
## ====================================================================



# INCLUDES = -I @top_srcdir@/include


EXTRA_LIBRARIES = libkernel.a
noinst_LIBRARIES = libkernel.a


if CFG_FS
FS_FILES = fs.c
endif

if CFG_KERNEL_SCHED_METHOD_RM
SCHED_RM_FILES = sched_rm.c
endif

if CFG_KERNEL_SCHED_METHOD_TIMESLICE
SCHED_TS_FILES = sched_ts.c
endif

APP_FILES = $(FS_FILES) $(SCHED_RM_FILES) $(SCHED_TS_FILES)

libkernel_a_SOURCES = main.c thread.c sched.c \
            sem.c timer.c int.c \
			rtl/string.c rtl/printk.c $(APP_FILES)


