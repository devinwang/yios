
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
 * Filename:	$RCSfile: mmzone.c,v $
 * Createdate:	2005/08/09 10:14:00
 * FileId:	$Id: mmzone.c,v 1.1.1.1 2005/08/09 05:25:25 devin Exp $
 * Last update:	$Date: 2005/08/09 05:25:25 $
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

#include <yios/config.h>
#include <yios/types.h>
#include <yios/errno.h>

#include <yios/rtl/rtl.h>

#include <yios/page.h>
#include <yios/memory.h>
#include <yios/mmzone.h>


YM_RAW_MEMORY   yi_g_stRawRam;

EXTERN YI_STATUS YMZoneInitialize()
{
    PLIST_HEAD		pHead;
    YM_INIT_MEMDES	*pMemDes;
    NRADDR		nTotalMem = 0;
    
    LIST_FOR_EACH(pHead, &yi_g_ltMemInitDes.ltNext)
    {
        pMemDes = LIST_ENTRY(pHead, YM_INIT_MEMDES, ltNext);

        YIRtlKernelMsg("\nRAM: Start: %08x, End: %08x", pMemDes->nStartAddr, pMemDes->nStartAddr + pMemDes->nSize);
        nTotalMem += pMemDes->nSize;
    }

    YIRtlKernelMsg("\nToTalMem:0x%0.8x", nTotalMem);


    return E_OK;
}

