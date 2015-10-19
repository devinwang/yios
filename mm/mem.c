
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
 * Filename:	$RCSfile: mem.c,v $
 * Createdate:	2005/08/09 10:13:00
 * FileId:	$Id: mem.c,v 1.1.1.1 2005/08/09 05:25:24 devin Exp $
 * Last update:	$Date: 2005/08/09 05:25:24 $
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

#include <yios/page.h>
#include <yios/memory.h>
#include <yios/mmzone.h>

#include <yios/hal/memory.h>
#include <yios/hal/bitopt.h>

#include <yios/rtl/rtl.h>
#include <yios/dbg.h>

/* Memory Initialize */
YM_INIT_MEMDES   yi_g_ltMemInitDes = {
	LIST_HEAD_INIT(yi_g_ltMemInitDes.ltNext),
	0, 0, 0
};

/* Kernel ram memory maps address. */
NRADDR           yi_g_nKernelPageMapsAddr;
STATIC NRADDR    yi_g_nRawMemHighest;
STATIC NRADDR    yi_g_nRawMemLowest;

/* Initialize memory system. */
EXTERN YI_STATUS YMInitialize()
{
    YMHalInitialize();
    YMZoneInitialize();

    return E_OK;
}

/* Initialize raw phyical memory maps (call by hal). */
EXTERN NRADDR YMInitRawRamMaps(PADDR pMapsAddr)
{
    PLIST_HEAD      pHead;
    YM_INIT_MEMDES  *pMemDes;
    NRADDR      nEnd, nMapSize;
    NRADDR      nMemLowest = ~0, nMemHighest = 0;

    /* Find lowest and highest memory. */
    LIST_FOR_EACH(pHead, &yi_g_ltMemInitDes.ltNext)
    {   
        pMemDes = LIST_ENTRY(pHead, YM_INIT_MEMDES, ltNext);
        nEnd = pMemDes->nStartAddr + pMemDes->nSize;

        if(pMemDes->nStartAddr < nMemLowest)
            nMemLowest = pMemDes->nStartAddr;

        if(nEnd > nMemHighest)
            nMemHighest = nEnd;
    }
    yi_g_nRawMemLowest = nMemLowest;
    yi_g_nRawMemHighest = nMemHighest;
    
    nMapSize = YMP_AR2PNR(nMemHighest - nMemLowest);

    nMapSize = (nMapSize + 7)/8;

    YIRtlMemorySet(pMapsAddr, 0xff, nMapSize);

    YIRtlKernelMsg("\nnMapSize:%d,Lowest:0x%08x,Highest:0x%08x",
                    nMapSize, nMemLowest, nMemHighest);    
    return nMapSize;
}

/* set raw memory maps bit. */
EXTERN VOID YMAllocMemoryMaps(NRADDR nAddr, NRADDR nSize)
{
    NRADDR nStart, nEnd;
    NRADDR nCount;

    if(nAddr < yi_g_nRawMemLowest)
        BUG();

    if(nAddr + nSize > yi_g_nRawMemHighest)
        BUG();

   
    nStart = nAddr /PAGE_SIZE;
    nEnd = (nAddr + nSize)/PAGE_SIZE;

    if(nStart >nEnd)
        BUG();

   for(nCount = nStart; nCount < nEnd; nCount++)
    {
        if(YITestSetBit((NRADDR*)yi_g_nKernelPageMapsAddr, nCount))
        {
            /* alloc page twice. */
            BUG();
        }
    }
    /* YIRtlKernelMsg("\n<AllocMem>:0x%08x, 0x%08x", nAddr, nSize);*/
}

/* Clear raw memory maps bit. */
EXTERN VOID YMFreeMemoryMaps(NRADDR nAddr, NRADDR nSize)
{
    NRADDR nStart, nEnd;
    NRADDR nCount;

    if(nAddr < yi_g_nRawMemLowest)
        BUG();

    if(nAddr + nSize > yi_g_nRawMemHighest)
        BUG();

   
    nStart = nAddr /PAGE_SIZE;
    nEnd = (nAddr + nSize)/PAGE_SIZE;
 
    if(nStart >nEnd)
        BUG();

    for(nCount = nStart; nCount < nEnd; nCount++)
    {
        if(!YITestClearBit((NRADDR*)yi_g_nKernelPageMapsAddr, nCount))
        {
            /* free page twice. */
            BUG();
        }
        //YIRtlKernelMsg("\n<Test>:0x%08x, 0x%08x", yi_g_nKernelPageMapsAddr, nCount);
    }
    return ;
}

