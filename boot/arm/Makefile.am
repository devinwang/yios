
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
## Createdate:	2004/08/15 09:06:00
## FileId:	$Id: Makefile.am,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
## Last update:	$Date: 2005/08/09 05:25:23 $
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
CFLAGS= @CFLAGS@
CCLD= @LD@

EXTRA_PROGRAMS = boot

noinst_PROGRAMS = boot

boot_SOURCES = boot.S

boot_CCASFLAGS = $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)
# boot_LDFLAGS = -Ttext 0x0 -Tbss 0x0 -Tdata 0x0 --oformat binary -e _start
boot_LDFLAGS = -Ttext 0x0 -Tbss 0x0 -Tdata 0x0  -e _start

