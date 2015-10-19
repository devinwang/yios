
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
 * Filename:	$RCSfile: sem.c,v $
 * Createdate:	2005/02/10 21:52:00
 * FileId:	$Id: sem.c,v 1.1.1.1 2005/08/09 05:25:25 devin Exp $
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
#include <yios/sem.h>
#include <yios/atom.h>
#include <yios/lock.h>
#include <yios/sched.h>

EXTERN YI_STATUS YIKeSemInit(YI_PSEMAPHORE pSem, INT nInitCount)
{
    YIRtlMemoryZero((PVOID)pSem, sizeof(YI_SEMAPHORE));

    YIKeAtomAssign(&pSem->stAtomCount, nInitCount);
    return E_OK;
}

EXTERN VOID YIKeSemLock(YI_PSEMAPHORE pSem)
{
    FLAGS_T nFlag;
    YI_PTHREAD pCurTh;

    YIKeSpinLockSave(pSem->nLock, nFlag);

    YIKeAtomDec(&pSem->stAtomCount);
    
    if(YIKeAtomRead(&pSem->stAtomCount) < 0)
    {
#if CFG_KERNEL_SCHED_METHOD_RM == 1
        pCurTh = YIKeGetCurrentThread();
       
        YIKeSRMMaskIdMask(&(pSem->arWaitThreadTblSt), pCurTh);

        YIKeSRMSuspendThread(YIKeGetCurrentThreadId());

#endif /* CFG_KERNEL_SCHED_METHOD_RM */
#if CFG_KERNEL_SCHED_METHOD_TIMESLICE == 1
        pCurTh = YIKeGetCurrentThread();
       
        YIKeSRMMaskIdMask(&(pSem->arWaitThreadTblSt), pCurTh);

        YIKeSRMSuspendThread(YIKeGetCurrentThreadId());

#endif /* CFG_KERNEL_SCHED_METHOD_TIMESLICE */
        YIKeSpinUnLockSave(pSem->nLock, nFlag);
        YIKeSRMSched(); 
    }else{
        YIKeSpinUnLockSave(pSem->nLock, nFlag);
    }
}

EXTERN VOID YIKeSemUnLock(YI_PSEMAPHORE pSem)
{
    FLAGS_T nFlag;
    YI_PTHREAD pTh;
    PID_T   nThId;
    YIKeSpinLockSave(pSem->nLock, nFlag);

#if CFG_KERNEL_SCHED_METHOD_RM == 1
    /* Some thread wait the semaphore. */
    if(YIKeAtomRead(&pSem->stAtomCount) < 0)
    {
        if(pSem->arWaitThreadTblSt.arThreadTbl1D)
        {
            nThId = YIKeSRMMaskIdGetHighest(&pSem->arWaitThreadTblSt);
            /* Walk up the thread. */
            pTh = &yi_g_arKeThreadTbl[nThId];
            /* Detach thread from wait table. */
            YIKeSRMMaskIdUnMask(&pSem->arWaitThreadTblSt, pTh);
            YIKeSRMResumeThread(nThId);
        }
    }
    YIKeAtomInc(&pSem->stAtomCount);
#endif /* CFG_KERNEL_SCHED_METHOD_RM */
#if CFG_KERNEL_SCHED_METHOD_TIMESLICE == 1
    /* Some thread wait the semaphore. */
    if(YIKeAtomRead(&pSem->stAtomCount) < 0)
    {
        if(pSem->arWaitThreadTblSt.arThreadTbl1D)
        {
            nThId = YIKeSRMMaskIdGetHighest(&pSem->arWaitThreadTblSt);
            /* Walk up the thread. */
            pTh = &yi_g_arKeThreadTbl[nThId];
            /* Detach thread from wait table. */
            YIKeSRMMaskIdUnMask(&pSem->arWaitThreadTblSt, pTh);
            YIKeSRMResumeThread(nThId);
        }
    }
    YIKeAtomInc(&pSem->stAtomCount);
#endif /* CFG_KERNEL_SCHED_METHOD_TIMESLICE */

    YIKeSpinUnLockSave(pSem->nLock, nFlag);
    YIKeSRMSched();
}

/* try to lock semaphore without waiting. */
EXTERN INT YIKeSemTryLock(YI_PSEMAPHORE pSem)
{
    FLAGS_T nFlag;
    /* YI_PTHREAD pCurTh;*/

    YIKeSpinLockSave(pSem->nLock, nFlag);

    /* Can get the semaphore? */    
    if(YIKeAtomRead(&pSem->stAtomCount) > 0)
    {
        YIKeAtomDec(&pSem->stAtomCount);
        YIKeSpinUnLockSave(pSem->nLock, nFlag);
    }else{
        YIKeSpinUnLockSave(pSem->nLock, nFlag);
        return SYSERR(ENOSEM);
    }
    return E_OK;
}
