
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
 * Filename:	$RCSfile: boot.h,v $
 * Createdate:	2004/04/29 05:58:00
 * FileId:	$Id: boot.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
 * Last update:	$Date: 2005/08/09 05:25:23 $
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

#ifndef _YIOS_HAL_BOOT_H_
#define _YIOS_HAL_BOOT_H_

#include <yios/config.h>

#define YI_PADDR_TO_16SEG(paddr)	(paddr>>4)

#define YI_BOOT_PADDR	0x07c00
#define YI_BOOTHI_PADDR	0x90000

/* "config file" tell me  which we load oskernel.sys to? */
/* 4k for IDT16 and BIOS data. */
#define YI_OSKERNELSYS_PADDR	CFG_KERNEL_OSKERNELSYS_PADDR

/* 16bit mode segment. */
#define YI_BOOTSEG		YI_PADDR_TO_16SEG(YI_BOOT_PADDR)
#define YI_BOOTHISEG		YI_PADDR_TO_16SEG(YI_BOOTHI_PADDR)

#define YI_OSKERNELSYSSEG	YI_PADDR_TO_16SEG(YI_OSKERNELSYS_PADDR)

/* Some bios use 4k memory on main 640k memory for EBDA (Extended BIOS DATA) */
#define YI_BOOTHISTKOF	0xeff0		/* 64k - 4k */


#endif /* _YIOS_HAL_BOOT_H_ */


