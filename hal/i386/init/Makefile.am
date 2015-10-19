
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
## Createdate:	2004/09/10 17:40:00
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

# INCLUDES = -I @top_srcdir@/include

CCLD = @LD@
CFLAGS=
CPPFLAGS=

if CFG_GUI
if CFG_VBE
APP_FILES = vesa16.S
endif
endif

# EXTRA_LIBRARIES = libinit.a
# noinst_LIBRARIES = libinit.a

# libinit_a_SOURCES = main.c

EXTRA_PROGRAMS = init16 init32
noinst_PROGRAMS = init16 init32

init16_SOURCES = init16.S stub16.S $(APP_FILES)
init16_CCASFLAGS =  $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)
init16_LDFLAGS = -Ttext 0x0 -Tbss 0x0 -Tdata 0x0 --oformat binary -e _start


init32_CCASFLAGS =  $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)

# init32_SOURCES = init32.S decom.c

# start32_o_SOURCES = start32.S
# start32_o_CCASFLAGS = $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)

init32.$(OBJEXT): init32.S
	$(CCAS) $(init32_CCASFLAGS) -c -o init32.o `test -f 'init32.S' || echo '$(srcdir)/'`init32.S

init32$(EXTEXT): init32.$(OBJEXT) decom.$(OBJEXT)
	../../../tools/binit32 $(LD) $(srcdir)/init32.lds init32.o decom.o


