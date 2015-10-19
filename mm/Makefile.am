
## ====================================================================
## The YingOS project.
## Copyright (C) 2005 Devin Wang

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
## Createdate:	2006/05/31 22:01:00
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


EXTRA_LIBRARIES = libmm.a
noinst_LIBRARIES = libmm.a

#if CFG_MM_MM
#APP_FILES = mem.c
#endif

if CFG_MM_MMU
APP_FILES = mmu.c
endif

libmm_a_SOURCES =  mem.c mmzone.c page.c $(APP_FILES)


