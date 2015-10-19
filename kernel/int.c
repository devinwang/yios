
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
 * Filename:	$RCSfile: int.c,v $
 * Createdate:	2004/09/12 15:45:00
 * FileId:	$Id: int.c,v 1.1.1.1 2005/08/09 05:25:25 devin Exp $
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

#include <yios/hal/int.h>
#include <yios/int.h>

#include <yios/lock.h>
#include <yios/sched.h>

#include <yios/dbg.h>

/* Interruter channel table. */
YI_IRQ_INTERRUPTER yi_g_arInterrupterTbl[CFG_INT_MAX_INTERRUPTER];

/* trap handle function point array. */
STATIC  YI_PTRAP_HANDLE_ROUTINE yi_g_arTrapHandleTbl[YI_HAL_INT_MAX_NUM];
 

EXTERN YI_STATUS YICALL YIIntInitialize(VOID)
{

    YIRtlMemoryZero(yi_g_arInterrupterTbl, sizeof(yi_g_arInterrupterTbl));    
    YIRtlMemoryZero(yi_g_arTrapHandleTbl, sizeof(yi_g_arTrapHandleTbl));

    YIHalIntInitialize();


	return E_OK;
}

/* install handle for one interrupter. */
EXTERN YI_STATUS YIIntInstallHandle(INT nIntNum, YI_PIRQ_HANDLE_STRUCT pHandle)
{
    YI_PIRQ_INTERRUPTER    pInt;
    YI_IRQ_INTERRUPTER      pIntIrq;

    FLAGS_T nFlag;

    pInt = &pIntIrq;

    if(nIntNum >= CFG_INT_MAX_INTERRUPTER)
        return EINTRNG;

    /* can't install a null handle to interrupter channel. */
    if(pHandle == NULL || pHandle->pHandle == NULL)
        return EPARAM;

    pInt = &yi_g_arInterrupterTbl[nIntNum];

    pHandle->pInterrupter = pInt;

    YIKeSpinLockSave(0, nFlag);

    /* Is this first handle in the interrupter channel. */
    if(pInt->pHandle == NULL)
    {
        pInt->pHandle = pHandle;
        pHandle->pNext = NULL;

        /* new handle enable share interruter. */
        if(pHandle->nMask & YI_IRQM_SHARE)
            pInt->nMask &= YI_INT_SHARE;
    }
    else
    {
        /* can we share interrupter? */
        if(!(pInt->nMask & YI_INT_SHARE) || !(pHandle->nMask & YI_IRQM_SHARE))
         {
            YIKeSpinUnLockSave(0, nFlag);
            return ENOINTSHARE;
         }
        pHandle->nStatus = pInt->pHandle->nStatus; 

        /* save old handle. */
        pHandle->pNext = pInt->pHandle;

        /* install handle. */
        pInt->pHandle = pHandle;
    }
    
    YIKeSpinUnLockSave(0, nFlag);
    return E_OK;
}


/* uninstall handle for one interrupter. */
EXTERN YI_STATUS YIIntUnstallHandle(INT nIntNum, YI_PIRQ_HANDLE_STRUCT pHandle)
{
    YI_PIRQ_HANDLE_STRUCT*  pHandlePos;
    FLAGS_T nFlag;

    if(nIntNum >= CFG_INT_MAX_INTERRUPTER)
        return EINTRNG;

    /* can't uninstall a null handle for interrupter channel. */
    if(pHandle == NULL)
        return EPARAM;

    
    YIKeSpinLockSave(0, nFlag);

    pHandlePos = &yi_g_arInterrupterTbl[nIntNum].pHandle;
    while(*pHandlePos != NULL)
    {
        if(*pHandlePos == pHandle)
        {
            /* uninstall handle */
            *pHandlePos = pHandle->pNext;
            YIKeSpinUnLockSave(0, nFlag);
            return E_OK;
        }
        pHandlePos = &((*pHandlePos)->pNext);
    }    

    YIKeSpinUnLockSave(0, nFlag);
    return ENOINTHD;
}

/* Call all installed interrupter handle. */
EXTERN VOID YIIntDoInterrupterHandle(INT nIntNum, YI_HAL_CPU_REGS* pRegs, PVOID pData)
{
    YI_PIRQ_HANDLE_STRUCT pHandleSt;

    pHandleSt = yi_g_arInterrupterTbl[nIntNum].pHandle;
    
    if(pHandleSt == NULL)
        return;

    /* this interrupter channel disabled? */
    if(pHandleSt->nMask & YI_INT_DISABLE)
        return;

    while(pHandleSt)
    {
        if(!(pHandleSt->nMask & YI_IRQM_DISABLE))
            pHandleSt->pHandle(nIntNum, pRegs, pData);
        pHandleSt = pHandleSt->pNext;
    }
}

/* Call all trap handle. */
EXTERN VOID YIIntDoTrapHandle(INT nIntNum, YI_HAL_CPU_REGS* pRegs, PVOID pData)
{
    if(yi_g_arTrapHandleTbl[nIntNum] != NULL)
    {
        yi_g_arTrapHandleTbl[nIntNum](nIntNum, pRegs, pData);
    }else
    {
        BUG();
    }
}

/* set system trap handle. */
EXTERN YI_STATUS YIIntSetTrapHandle(INT nIntNum, YI_PTRAP_HANDLE_ROUTINE pFn, PVOID pData)
{
    if(pFn == NULL)
        return EPARAM;

    if(nIntNum >= YI_HAL_INT_MAX_NUM)
        return EINTRNG;
        
    yi_g_arTrapHandleTbl[nIntNum] = pFn;
    
    return E_OK;
}

