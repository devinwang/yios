
/* ====================================================================
 * The YingOS project.
 * Copyright (C) 2004 Devin Wang

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * ====================================================================
 *
 * Filename:	$RCSfile: list.h,v $
 * Createdate:	2005/01/05 20:41:00
 * FileId:	$Id: list.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
 * Last update:	$Date: 2006/03/19 14:02:00 $
 * Creator:	Devin Wang <vhd@21cn.com>
 * Version:	$Revision: 1.1.1.1 $
 * Description: 
 *
 * ====================================================================
 */

/* ====================================================================
 *  Revision:
 *
 * ====================================================================
 */


#ifndef _YIOS_MW_MWCFG_H_
#define _YIOS_MW_MWCFG_H_

#include <yios/config.h>
#include <yios/types.h>

#include <yios/rtl/rtl.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

typedef SIZE_T size_t;
/* typedef sizt_t ssize_t;*/

#define __YINGOS__  1
/* #define UNIX 1 */

#define MWPIXEL_FORMAT MWPF_TRUECOLOR565


#define MW_CPU_BIG_ENDIAN 0

#define NOSTDPAL4
#define NOSTDPAL8

/*#define HAVE_FNT_SUPPORT    0*/
#define HAVE_HZK_SUPPORT    0
#define HAVE_GB2312_SUPPORT 0

#define HAVE_FNTGZ_SUPPORT  0
#define FNT_FONT_DIR "fonts/bdf"
#define HZK_FONT_DIR "fonts/chinese"

#define NOFONTSORCLIPPING   0

#define FRAMEBUFFER         0
#define HAVETEXTMODE        0

#define THREADSAFE          0

#define SCREEN_WIDTH   800
#define SCREEN_HEIGHT  600

#define NONETWORK           1
#define NANOX               1 

#define UPDATEREGIONS       1

#define HAVE_USLEEP 

/* #define MWPIXEL_FORMAT  MWPF_TRUECOLOR565*/

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _YIOS_MW_MWCFG_H_ */



