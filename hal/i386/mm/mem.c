
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
 * Filename:	$RCSfile: mem.c,v $
 * Createdate:	2004/06/09 21:47:00
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

#include <yios/hal/init.h>
#include <yios/hal/e820.h>

#include <yios/rtl/rtl.h>

#include <yios/hal/memory.h>
#include <yios/memory.h>
#include <yios/int.h>
#include <yios/dbg.h>

/* System page memory description array. */
STATIC YM_INIT_MEMDES	yi_g_arMemDes[YM_E820NR_MAX+1];
STATIC UINT yi_g_nMemDesNR = 0;

/* kernel memory highest address. */
EXTERN PADDR	yi_g_nLdAllEnd;


EXTERN VOID YIDrawFillRect(INT x1, INT y1, INT x2, INT y2, UINT32 color);
EXTERN VOID YIDrawPoint(INT x, INT y, UINT32 color);

/* Show blue text. */
EXTERN UINT32 yi_g_nTtyBKColor;
STATIC VOID YIHalMMPageFault(INT nIntNum, YI_HAL_CPU_REGS* pRegs, PVOID pData)
{
    UINT nCr2;

#if CFG_GUI == 1
    YIDrawFillRect(0, 0, 800, 600, 0x000000ff);
    yi_g_nTtyBKColor = 0x000000ff;
    
#endif /* CFG_VBE */

    __asm__(
    "movl %%cr2, %0\n\t"
    :"=r"(nCr2)
    );

    YIRtlKernelMsg("\n      =========================");
    YIRtlKernelMsg("\n            Page Fault!!!");
    YIRtlKernelMsg("\n      =========================");

    YIRtlKernelMsg("\n\
   At 0x%08x when access 0x%08x\n\
   -------------------------------------", pRegs->old_eip, nCr2);

   YIRtlKernelMsg("\n\
   cs:    0x%08x, eflags:0x%08x\n\
   ss:    0x%08x, esp:   0x%08x\n\
   eax:   0x%08x, ebx:   0x%08x\n\
   ecx:   0x%08x, edx:   0x%08x\n\
   esi:   0x%08x, edi:   0x%08x\n", 
   pRegs->old_cs, pRegs->old_eflags, 
				pRegs->old_ss, pRegs->old_esp, pRegs->eax, pRegs->ebx, pRegs->ecx, pRegs->edx,
				pRegs->esi, pRegs->edi);

    __asm__("cli\n\thlt");
}

/* Add raw-ram zone into chain. */
STATIC YI_STATUS YMAddRamZone(NRADDR nStart, NRADDR nSize)
{
    YM_INIT_MEMDES* pDes;

    if(yi_g_nMemDesNR > YM_E820NR_MAX)
    {
        BUG();
        return !E_OK;
    }

    /* Add low  1M memory. */
    pDes = &yi_g_arMemDes[yi_g_nMemDesNR];
    
    pDes->nStartAddr = nStart;
    pDes->nSize = nSize;

    YIListAddTail(&yi_g_ltMemInitDes.ltNext, &pDes->ltNext);

    yi_g_nMemDesNR++;

    return E_OK;

}


/* Scan e820 table, get memory information. */
STATIC YI_STATUS YMInitE820Map()
{
    INT nE820Num;
    INT nCount;
    YM_E820AREA*    pMaps;

    nE820Num = (INT)*(PUINT8)YI_P2V(YI_KE_E820_NR_PADDR);
   
    if(nE820Num > YM_E820NR_MAX || nE820Num <= 0)
    {
        BUG();
        return !E_OK;
    }

    pMaps = (YM_E820AREA*) YI_P2V(YI_KE_E820_MAPS_PADDR);


    for(nCount = 0; nCount < nE820Num; nCount++)
    {
        NRADDR nStart, nSize;

        switch(pMaps->nType)
        {
            case YM_E820T_RAM:
                nStart = pMaps->nStart;
                nSize = pMaps->nSize;

                nStart = YM_ALIGN_UP_PAGE(nStart);
                nSize = YM_ALIGN_DN_PAGE(nSize);
                if(nSize == 0)
                    break;

                if(nStart < YI_KERNEL_RDATA_BASE_PADDR && nStart+nSize > YI_KERNEL_RDATA_BASE_PADDR)
                {
                    YMAddRamZone(nStart, YI_KERNEL_RDATA_BASE_PADDR - nStart);
                    YIRtlKernelMsg("\nStart:%016Lx, End:%016Lx, Type:%s", pMaps->nStart, pMaps->nStart + pMaps->nSize, "RAM");
                    break;
                }

                if(nStart >= YI_KERNEL_RDATA_BASE_PADDR && nStart < YM_HIGHMEM_PADDR)
                {
                    if(nSize <= YM_HIGHMEM_PADDR - YI_KERNEL_RDATA_BASE_PADDR)
                        break;

                    YMAddRamZone(YM_HIGHMEM_PADDR, nSize+nStart-YM_HIGHMEM_PADDR);
                    YIRtlKernelMsg("\nStart:%016Lx, End:%016Lx, Type:%s", pMaps->nStart, pMaps->nStart + pMaps->nSize, "RAM");
                    break;
                }
                YMAddRamZone(YM_ALIGN_UP_PAGE(pMaps->nStart), YM_ALIGN_DN_PAGE(pMaps->nSize));
                YIRtlKernelMsg("\nStart:%016Lx, End:%016Lx, Type:%s", pMaps->nStart, pMaps->nStart + pMaps->nSize, "RAM");
            break;

            case YM_E820T_RESERVED:
                YIRtlKernelMsg("\nStart:%016Lx, End:%016Lx, Type:%s", pMaps->nStart, pMaps->nStart + pMaps->nSize, "RESERVED");
            break;

            case YM_E820T_ACPI:
                YIRtlKernelMsg("\nStart:%016Lx, End:%016Lx, Type:%s", pMaps->nStart, pMaps->nStart + pMaps->nSize, "ACPI");
            break;

            case YM_E820T_ROM:
                YIRtlKernelMsg("\nStart:%016Lx, End:%016Lx, Type:%s", pMaps->nStart, pMaps->nStart + pMaps->nSize, "ROM");
            break;
        }
        pMaps++;
    }
    
    return E_OK;
}


STATIC YI_STATUS YMHalInitMemoryMaps()
{

    PLIST_HEAD      pHead;
    YM_INIT_MEMDES  *pMemDes;
    NRADDR          nMapSize;

    /* get kernel end address. */
    NRADDR pKernelEnd;
    pKernelEnd = YM_ALIGN_UP_PAGE((NRADDR)&yi_g_nLdAllEnd);

    yi_g_nKernelPageMapsAddr = pKernelEnd;


    /* Init system raw memory maps. */
    nMapSize = YMInitRawRamMaps((PADDR)yi_g_nKernelPageMapsAddr);

    /* Free system raw memory */
    LIST_FOR_EACH(pHead, &yi_g_ltMemInitDes.ltNext)
    {   
        pMemDes = LIST_ENTRY(pHead, YM_INIT_MEMDES, ltNext);
        
        YMFreeMemoryMaps(pMemDes->nStartAddr, pMemDes->nSize);
    }

    /* reserve first page. */
    YMAllocMemoryMaps(0, PAGE_SIZE);
    
    /* reserve kernel memory. */
    YMAllocMemoryMaps(CFG_KERNEL_IMAGE_PADDR, 
            YI_V2P(pKernelEnd) - CFG_KERNEL_IMAGE_PADDR);

    /* reserve raw maps self */
    YMAllocMemoryMaps(YM_ALIGN_UP_PAGE(YI_V2P(yi_g_nKernelPageMapsAddr) + nMapSize),
                        YM_ALIGN_UP_PAGE(nMapSize));

    return E_OK;
}


EXTERN YI_STATUS YMHalInitialize(VOID)
{
    /* Set page fault handle. */
    YIIntSetTrapHandle(YI_HAL_INT_VECTOR_PGFUT, YIHalMMPageFault, NULL);

    /* Initialize memory map table. */
    YMInitE820Map();

    YMHalInitMemoryMaps();

    return E_OK;
}


