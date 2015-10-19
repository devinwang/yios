
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

 * Please read this project's documents about the detail of myOS's source.
 *
 * ====================================================================
 *
 * Filename:	$RCSfile: pageset.h,v $
 * Createdate:	2004/06/04 09:09:00
 * FileId:	$Id: pageset.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
 * Last update:	$Date: 2005/08/09 05:25:23 $
 * Creator:	Devin Wang <vhd@21cn.com>
 * Version:	$Revision: 1.1.1.1 $
 * Description: page mode set.
 *
 * ====================================================================
 */

/* ====================================================================
 *  Revision:
 *
 * ====================================================================
 */

#ifndef _YIOS_HAL_PAGESET_H_
#define _YIOS_HAL_PAGESET_H_

#define YI_KERNEL_OFFSET	0x80000000	/* 2GB */

#define YI_P2V(x)		(YI_KERNEL_OFFSET + (x))
#define YI_V2P(x)		((x) - YI_KERNEL_OFFSET)

/* 4k page on IA32 */
#define PAGE_SIZE		0x00001000

#define YI_PAGE_ADDR_MASK		0xfffff000

#define YI_PAGE_PROP_PRESENT	0x0001
#define YI_PAGE_PROP_WRITEABLE	0x0002
#define YI_PAGE_PROP_USER		0x0004
#define YI_PAGE_PROP_ACCESSED	0x0020
#define YI_PAGE_PROP_DIRTY		0x0040

#define YI_PAGE_PDT_SHIFT		22
#define YI_PAGE_PDT_MASK		0xFFC00000
#define YI_PAGE_ADDR_TO_PDT(x)	(((x) & YI_PAGE_PDT_MASK) >> YI_PAGE_PDT_SHIFT)

#define YI_PAGE_PGT_SHIFT		12
#define	YI_PAGE_PGT_MASK		0x003FF000
#define YI_PAGE_ADDR_TO_PGT(x)	(((x) & YI_PAGE_PGT_MASK) >> YI_PAGE_PGT_SHIFT)

#define YI_PAGE_PAGEADDR_SHIFT	12
#define YI_PAGE_PAGENUM_TO_ADDR(x)	((x)<<YI_PAGE_PAGEADDR_SHIFT)
#define YI_PAGE_ADDR_TO_PAGENUM(x)	((x)>>YI_PAGE_PAGEADDR_SHIFT)


#define YMP_AR2PNR(x) YI_PAGE_ADDR_TO_PAGENUM((x))
#define YMP_PNR2AR(x) YI_PAGE_PAGENUM_TO_ADDR((x))

#endif /* _YIOS_HAL_PAGSET_H_ */


