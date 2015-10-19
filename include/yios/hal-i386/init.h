
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
 * Filename:	$RCSfile: init.h,v $
 * Createdate:	2004/05/31 11:40:00
 * FileId:	$Id: init.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
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

#ifndef _YIOS_HAL_INIT_H_
#define _YIOS_HAL_INIT_H_

#include <yios/config.h>
#include <yios/hal/segset.h>
#include <yios/hal/pageset.h>

/*
  When kernel (no compressed) are ready to work, it use 1M memory like this figure.
	+-------------+	0x100000	1M
	|    BIOS     |
	|     IO      |
	+-------------+	0xA0000		640k
	|  64k RDATA  |
	|             |
	+-------------+	0x90000		576k
	|             |
	|             |
	|   kernel    |
	+-------------+	0x1000		4k
	|   4k BIOS   | 
	|    DATA     |
 	+-------------+	0x0000
 */


/* We reserve 64-4k memory for yios. */
#define YI_KERNEL_RDATA_BASE_PADDR	0x90000
#define YI_KE_BASE_PADDR		YI_KERNEL_RDATA_BASE_PADDR

/* RDATA memory figure.
	+-------------+	0xA0000
	| 4k bios     |
	| extended    |
	| data(EBDA)  |
	+-------------+	0x9f000
	|   12k free  |
	|   memory    |
	|             |
	|             |
 	+-------------+	0x9c000
	|             |
	|             |
	|   8*4k PGT  |
	|             |
	+-------------+	0x94000
	|             |
	|  4k PDT     |
	|             |
	+-------------+	0x93000
	| 4k kernel   |
	| 16bit stub  |
	| code        |
	+-------------+	0x92000
	| 4k system   |
	| parameter   |
	| table       |
	+-------------+	0x91000
	|     2k      |
	|     GDT     |
	+-------------+	0x90800
	|     2k      |
	|     IDT     |
	+-------------+	0x90000
 */

/* 2k for IDT */
#define YI_KE_IDT_PADDR		(YI_KE_BASE_PADDR + 0x0000)
#define YI_KE_IDT_SIZE		0x0800

/* 2k for GDT (256 items, not 8096 items) */
#define YI_KE_GDT_PADDR		(YI_KE_BASE_PADDR + YI_KE_IDT_SIZE)
#define YI_KE_GDT_SIZE		0x0800

/* 4k system parameter table. */
#define YI_KE_SPT_PADDR		(YI_KE_GDT_PADDR + YI_KE_GDT_SIZE)
#define YI_KE_SPT_SIZE		0x1000

/* 4k 16bit stub code for 32bit mode driver. */
#define YI_KE_16SC_PADDR		(YI_KE_SPT_PADDR + YI_KE_SPT_SIZE)
#define YI_KE_16SC_SIZE		0x1000

/* kernel use 4k + 8*4k page talbe memory. */
#define YI_KE_PAGE_TABLE_BASE_PADDR	(YI_KE_16SC_PADDR + YI_KE_16SC_SIZE)
#define YI_KE_PDT_PADDR		YI_KE_PAGE_TABLE_BASE_PADDR

#define YI_KE_PGT_PADDR		YI_KE_PDT_PADDR + PAGE_SIZE
/* 8 PGT pages. */
#define YI_KE_PGT_NUM		8	

#define YI_KE_IDT_SEG		YI_PADDR_TO_16SEG(YI_KE_IDT_PADDR)
#define YI_KE_GDT_SEG		YI_PADDR_TO_16SEG(YI_KE_GDT_PADDR)

#define YI_KE_TEMP_BUFFER_PADDR	(YI_KE_PGT_PADDR + PAGE_SIZE * YI_KE_PGT_NUM)

/* vbe segment */
#define YI_KE_VBE_PADDR		YI_KE_TEMP_BUFFER_PADDR
#define YI_KE_VBE_SEG		YI_PADDR_TO_16SEG(YI_KE_VBE_PADDR)
#define YI_KE_VBE_SEGOFF	0x0000 
#define YI_KE_VBE_VADDR		0xff800000

/* microwindows stack memory. */
#if CFG_MW==1
#define YI_KE_MW_SM_SIZE    0x00400000
/* #define YI_KE_MW_SM_PADDR   (YI_KE_TEMP_BUFFER_PADDR + 0x004000000)*/
#define YI_KE_MW_SM_PADDR   0x00400000
#define YI_KE_MW_SM_VADDR   0xfe000000
#endif /* CFG_MW */

/* e820 memory map table */
#define YI_KE_E820_NR_PADDR	YI_KE_SPT_PADDR + 0
#define YI_KE_E820_NR_SEG	YI_PADDR_TO_16SEG(YI_KE_SPT_PADDR)
#define YI_KE_E820_NR_SEGOFF	YI_KE_E820_NR_PADDR - YI_KE_SPT_PADDR
#define YI_KE_E820_MAPS_PADDR	(YI_KE_SPT_PADDR+ 4)
#define YI_KE_E820_MAPS_SEG	YI_PADDR_TO_16SEG(YI_KE_SPT_PADDR)
#define YI_KE_E820_MAPS_SEGOFF	YI_KE_E820_MAPS_PADDR - YI_KE_SPT_PADDR


/* decom.c bss segment size. */
/* #define YI_INIT32_UNZIP_BSS_SIZE	(0x10000 + 0x800)*/       /* 64k + 2k */

/* 1M memory for init32 unzip */
#define YI_INIT32_UNZIP_MALLOC_PADDR	0x200000
#define YI_INIT32_UNZIP_STACK_PADDR	0x300000


/* map some memory for stack of loader. */
#define YI_KE_STACK_TOP_PADDR	0x00100000
#define YI_KE_STACK_TOP_VADDR	0xfffffff0


#ifndef __ASSEMBLER__ 

	

#endif /* __ASSEMBLER__ */


#endif /* _YIOS_HAL_INIT_H_ */

