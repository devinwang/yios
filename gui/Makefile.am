
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
## Createdate:	2005/01/08 00:12:00
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

EXTRA_LIBRARIES = libgui.a
noinst_LIBRARIES = libgui.a

#if CFG_DRIVER_NKB
#I386_NKB_SRCS = i386/nkb.c
#endif

#libdriver_a_SOURCES = i386/pc_com.c $(I386_NKB_SRCS)



MW_DIR=mw

MW_STDINC_DIR=stdinc
MW_INCLUDE_DIR=include
MW_ENGINE_DIR=engine
MW_DRIVERS_DIR=drivers
MW_NANOX_DIR=nanox
MW_MWIN_DIR=mwin
MW_FONTS_DIR=fonts

MW_ENGINE_SRCS := $(MW_DIR)/$(MW_ENGINE_DIR)/devopen.c \
                $(MW_DIR)/$(MW_ENGINE_DIR)/devdraw.c \
                $(MW_DIR)/$(MW_ENGINE_DIR)/devfont.c \
                $(MW_DIR)/$(MW_ENGINE_DIR)/devmouse.c \
                $(MW_DIR)/$(MW_ENGINE_DIR)/devkbd.c \
                $(MW_DIR)/$(MW_ENGINE_DIR)/devclip.c \
                $(MW_DIR)/$(MW_ENGINE_DIR)/devrgn.c \
                $(MW_DIR)/$(MW_ENGINE_DIR)/devpal1.c \
                $(MW_DIR)/$(MW_ENGINE_DIR)/devpal2.c \
                $(MW_DIR)/$(MW_ENGINE_DIR)/devimage.c \
                $(MW_DIR)/$(MW_ENGINE_DIR)/devimage_stretch.c \
                $(MW_DIR)/$(MW_ENGINE_DIR)/image_bmp.c \
                $(MW_DIR)/$(MW_ENGINE_DIR)/image_gif.c \
                $(MW_DIR)/$(MW_ENGINE_DIR)/image_jpeg.c \
                $(MW_DIR)/$(MW_ENGINE_DIR)/image_png.c \
                $(MW_DIR)/$(MW_ENGINE_DIR)/image_pnm.c \
                $(MW_DIR)/$(MW_ENGINE_DIR)/image_tiff.c \
                $(MW_DIR)/$(MW_ENGINE_DIR)/image_xpm.c \
                $(MW_DIR)/$(MW_ENGINE_DIR)/devlist.c \
                $(MW_DIR)/$(MW_ENGINE_DIR)/selfont.c \
                $(MW_DIR)/$(MW_ENGINE_DIR)/error.c \
                $(MW_DIR)/$(MW_ENGINE_DIR)/devrgn2.c \
                $(MW_DIR)/$(MW_ENGINE_DIR)/devarc.c \
                $(MW_DIR)/$(MW_ENGINE_DIR)/devpoly.c \
                $(MW_DIR)/$(MW_ENGINE_DIR)/devstipple.c \
                $(MW_DIR)/$(MW_ENGINE_DIR)/font_dbcs.c
       
# MW_ENGINE_SRCS += $(MW_DIR)/$(MW_ENGINE_DIR)/font_fnt.c
# MW_ENGINE_SRCS += $(MW_DIR)/$(MW_ENGINE_DIR)/font_hzk.c
                

MW_DRIVERS_SRCS := $(MW_DIR)/$(MW_STDINC_DIR)/stdsup.c \
                $(MW_DIR)/$(MW_DRIVERS_DIR)/scr_svga.c \
                $(MW_DIR)/$(MW_DRIVERS_DIR)/kbd_ps2.c \
                $(MW_DIR)/$(MW_DRIVERS_DIR)/mou_ps2.c \
                $(MW_DIR)/$(MW_DRIVERS_DIR)/genfont.c

MW_FONTS_SRCS := $(MW_DIR)/$(MW_FONTS_DIR)/winFreeSansSerif11x13.c \
                $(MW_DIR)/$(MW_FONTS_DIR)/X6x13.c

# no use windows api
MW_MWIN_SRCS := $(MW_DIR)/$(MW_MWIN_DIR)/winmain.c \
                $(MW_DIR)/$(MW_MWIN_DIR)/winuser.c \
                $(MW_DIR)/$(MW_MWIN_DIR)/wingdi.c \
                $(MW_DIR)/$(MW_MWIN_DIR)/winexpos.c \
                $(MW_DIR)/$(MW_MWIN_DIR)/winclip.c \
                $(MW_DIR)/$(MW_MWIN_DIR)/winevent.c \
                $(MW_DIR)/$(MW_MWIN_DIR)/windefw.c \
                $(MW_DIR)/$(MW_MWIN_DIR)/winrgn.c \
                $(MW_DIR)/$(MW_MWIN_DIR)/winfont.c \
                $(MW_DIR)/$(MW_MWIN_DIR)/winsbar.c \
                \
                $(MW_DIR)/$(MW_MWIN_DIR)/winlib/draw3d.c \
                $(MW_DIR)/$(MW_MWIN_DIR)/winlib/ptinsid.c \
                $(MW_DIR)/$(MW_MWIN_DIR)/winlib/insetr.c \
                $(MW_DIR)/$(MW_MWIN_DIR)/winlib/fastfill.c \
                $(MW_DIR)/$(MW_MWIN_DIR)/winlib/graph3d.c \
                $(MW_DIR)/$(MW_MWIN_DIR)/winlib/mwuser.c \
                \
                $(MW_DIR)/$(MW_MWIN_DIR)/winlib/button.c \
                $(MW_DIR)/$(MW_MWIN_DIR)/winlib/edit.c \
                $(MW_DIR)/$(MW_MWIN_DIR)/winlib/listbox.c \
                $(MW_DIR)/$(MW_MWIN_DIR)/winlib/progbar.c \
                $(MW_DIR)/$(MW_MWIN_DIR)/winlib/static.c \
                $(MW_DIR)/$(MW_MWIN_DIR)/winlib/combobox.c \
                $(MW_DIR)/$(MW_MWIN_DIR)/winlib/caret.c \
                $(MW_DIR)/$(MW_MWIN_DIR)/winlib/scrlbar.c \
                $(MW_DIR)/$(MW_MWIN_DIR)/winlib/medit.c 


MW_NANOX_SRV_SRCS := $(MW_DIR)/$(MW_NANOX_DIR)/srvmain.c \
                $(MW_DIR)/$(MW_NANOX_DIR)/srvfunc.c \
                $(MW_DIR)/$(MW_NANOX_DIR)/srvutil.c \
                $(MW_DIR)/$(MW_NANOX_DIR)/srvevent.c \
                $(MW_DIR)/$(MW_NANOX_DIR)/srvclip.c
                # for network only (server) 
#                $(MW_DIR)/$(MW_NANOX_DIR)/srvnet.c

MW_NANOX_CLI_SRCS := $(MW_DIR)/$(MW_NANOX_DIR)/nxdraw.c \
                $(MW_DIR)/$(MW_NANOX_DIR)/nxutil.c
#                $(MW_DIR)/$(MW_NANOX_DIR)/nxtransform.c 
                 # for network only (client)
#                $(MW_DIR)/$(MW_NANOX_DIR)/client.c \
#                $(MW_DIR)/$(MW_NANOX_DIR)/nxproto.c \
#                $(MW_DIR)/$(MW_NANOX_DIR)/error.c 


MW_NANOX_SRCS := $(MW_NANOX_SRV_SRCS) $(MW_NANOX_CLI_SRCS)

#MW_DEMOS_SRCS := $(MW_DIR)/demos/mwin/mine.c
MW_DEMOS_SRCS = $(MW_DIR)/demos/$(MW_NANOX_DIR)/ntetris.c

MW_SRCS := $(MW_ENGINE_SRCS) $(MW_DRIVERS_SRCS) $(MW_NANOX_SRCS) $(MW_FONTS_SRCS) $(MW_DEMOS_SRCS)


libgui_a_CFLAGS = -I@top_srcdir@/gui/$(MW_DIR)/$(MW_STDINC_DIR) -I@top_srcdir@/gui/$(MW_DIR)/$(MW_INCLUDE_DIR)
libgui_a_SOURCES = $(MW_SRCS)

