
/* ====================================================================
 * The YingOS project.
 * Copyright (C) 2005 Devin Wang

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
 * Filename:	$RCSfile: memory.h,v $
 * Createdate:	2005/08/09 21:29:00
 * FileId:      $Id: memory.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
 * Last update:	$Date: 2005/08/09 05:25:23 $
 * Creator:     Devin Wang <vhd@21cn.com>
 * Version:     $Revision: 1.1.1.1 $
 * Description: 
 *
 * ====================================================================
 */

/* ====================================================================
 *  Revision:
 *
 * ====================================================================
 */


#ifndef _YIOS_MEMORY_H_
#define _YIOS_MEMORY_H_

#include <yios/config.h>
#include <yios/types.h>
#include <yios/list.h>

#include <yios/mmzone.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

#define YM_ALIGN_UP_PAGE(__x__)	ALIGN_UP((__x__), PAGE_SIZE)
#define YM_ALIGN_DN_PAGE(__x__)	ALIGN_DN((__x__), PAGE_SIZE)


#define YM_MEM_RAM      	1
#define YM_MEM_FLASH		2
#define YM_MEM_IOMEM		3

/* Initialize memory description stucture. */
typedef struct _YM_INIT_MEMDES{
	LIST_HEAD	ltNext;
	NRADDR		nStartAddr;
	NRADDR		nSize;
	UINT		nType;		/* Memory type. */
}YM_INIT_MEMDES;

/* HAL set memory infomation to the chain. */
EXTERN YM_INIT_MEMDES	yi_g_ltMemInitDes;

/* Kernel ram memory maps address. */
EXTERN NRADDR           yi_g_nKernelPageMapsAddr;


EXTERN YI_STATUS YMInitialize();


/* Initialize raw phyical memory maps (call by hal). */
EXTERN NRADDR YMInitRawRamMaps(PADDR pMapsAddr);

/* set raw memory maps bit. */
EXTERN VOID YMAllocMemoryMaps(NRADDR nAddr, NRADDR nSize);

/* Clear raw memory maps bit. */
EXTERN VOID YMFreeMemoryMaps(NRADDR nAddr, NRADDR nSize);

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _YIOS_MEMORY_H_ */

