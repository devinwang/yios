
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
 * Filename:	$RCSfile: mmzone.h,v $
 * Createdate:	2005/07/28 17:53:00
 * FileId:	$Id: mmzone.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
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


#ifndef _YIOS_MMZONE_H_
#define _YIOS_MMZONE_H_

#include <yios/config.h>
#include <yios/types.h>

#include <yios/hal/pageset.h>

#include <yios/lock.h>
#include <yios/page.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

#define YM_ZONE_MAX_ORDER	10

/* memory type. */
#define MEM_KERNEL
#define MEM_USER
#define MEM_

typedef struct _YM_MEMORY_ZONE *YM_PMEMORY_ZONE;


typedef struct _YM_FREE_AREA{
	LIST_HEAD		ltPages;
	UINT			nOrder;
	YM_PMEMORY_ZONE		pZone;
	UINT32			arMap[0];
}YM_FREE_AREA;


typedef struct _YM_MEMORY_ZONE{
	LIST_HEAD	ltNext;
	SPINLOCK_T	stSpinLock;
	NRADDR		nStartAddr;
	NRADDR		nEndAddr;
	YM_FREE_AREA	arFreeArea[YM_ZONE_MAX_ORDER];
}YM_MEMORY_ZONE; 

typedef struct _YM_RAW_MEMORY{
    NRADDR      nStart;
    NRADDR      nEnd;
    NRADDR      nSize;
    INT         nType;
    PVOID       pMaps;

    YM_PMEMORY_ZONE pMemZone;
}YM_RAW_MEMORY, YM_PRAW_MEMORY;

/* System raw physical memory.  */
EXTERN YM_RAW_MEMORY   yi_g_stRawRam;

EXTERN YI_STATUS YMZoneInitialize();

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _YIOS_MMZONE_H_ */



