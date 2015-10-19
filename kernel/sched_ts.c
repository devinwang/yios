
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
 * Filename:	$RCSfile: sched_ts.c,v $
 * Createdate:	2005/06/03 22:49:00
 * FileId:	$Id: sched_ts.c,v 1.1.1.1 2005/08/09 05:25:25 devin Exp $
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

/* #include <yios/hal/cpu.h>*/
#include <yios/lock.h>

#include <yios/rtl/rtl.h>
#include <yios/thread.h>
#include <yios/sched.h>

/* allocate a number of thread buffer for all system thread. */
YI_THREAD yi_g_arKeThreadTbl[CFG_KERNEL_SCHED_MAX_THREAD];

/* variable for chain of free thread buffer. */
YI_PTHREAD yi_g_pKeFreeThreadList = NULL;


CONST UINT8 yi_g_arKeMapTbl[8] = 
{0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

CONST UINT8 yi_g_arKeUnMapTbl[256] =
{
	0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
	0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
	0x05, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
	0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
	0x06, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
	0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
	0x05, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
	0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
	0x07, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
	0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
	0x05, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
	0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
	0x06, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
	0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
	0x05, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
	0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00,
};


YI_THREAD_MASK_TABLE_STRUCT yi_g_arKeReadyThreadTblSt;

STATIC UINT yi_g_nKeSRMPhonyThreadStack;
STATIC UINT yi_g_pKeSRMIdleThreadStack[128];
STATIC UINT yi_g_pKeSRMTimerThreadStack[128];

/* System performance count. */
VOLATILE UINT32  yi_g_nCpuPfmUsedTime = 0;
VOLATILE UINT32  yi_g_nCpuPfmFreeTime = 0;

VOLATILE UINT32  yi_g_nCpuPfmUsedTimeCount = 0;
VOLATILE UINT32  yi_g_nCpuPfmFreeTimeCount = 0;

VOLATILE INT yi_g_nCpuPfmIdle = 0;

STATIC VOID GetTsc(PUINT32 pTscLow, PUINT32 pTscHigh)
{   
    __asm__ __volatile__(
        "rdtsc\n\t"
        "movl %%eax, %0\n\t"
        "movl %%edx, %1\n\t"
        :"=m"(*pTscLow), "=m"(*pTscHigh)
        :
    );
}


/* System idle thread main routine */
YI_STATUS YICALL YIKeSRMIdleThread(PVOID pData)
{
	for(;;)
	{
        UINT32 nTscCountLow, nTscCountHigh, nTscCount;

        __asm__ __volatile__("cli");

        GetTsc(&nTscCountLow, &nTscCountHigh);
        nTscCount = nTscCountLow>>16 | nTscCountHigh<<16;

        yi_g_nCpuPfmUsedTime += nTscCount-yi_g_nCpuPfmUsedTimeCount;

        /* YIRtlKernelMsg("%d", yi_g_nCpuPfmFreeTime); */

        yi_g_nCpuPfmFreeTimeCount = nTscCount;
        yi_g_nCpuPfmIdle = 1;

        __asm__ __volatile__("sti");

		YIHalIdleCall(); 
	}
	return E_OK;
}


/* System time thread main routine */
STATIC YI_STATUS YICALL YIKeSRMTimerThread(PVOID pData)
{
	for(;;)
	{
		UINT nCount;
	
		for(nCount = 1; nCount < CFG_KERNEL_SCHED_IDLE_PRIORITY; nCount++)
		{
			FLAGS_T nFlag;
            YIKeSpinLockSave(0, nFlag);
            
			if(yi_g_arKeThreadTbl[nCount].nDelayTime)
			{
				if(--yi_g_arKeThreadTbl[nCount].nDelayTime == 0)
				{	
					YIKeSRMResumeThread(nCount);
				}
			}
			YIKeSpinUnLockSave(0, nFlag);
		}

		YIKeSRMSuspendThread(CFG_KERNEL_SCHED_SYSTEM_TIME_PRIORITY);
		YIHalReSched();

	}
	return E_OK;
}


EXTERN YI_STATUS YICALL YIKeSchedRMInit()
{
	INT	nCount;
	
	/* how deep in current intterupte or trap. */
	yi_g_nKeIntTrapDeepCount = 0;

	/* sched thread when yi_g_nKeDisSchedDeepCount is 0 */
	yi_g_nKeDisSchedDeepCount = 0;

	/* We should kown how deep in current critical call. */
	/* yi_g_nKeEnterCriticalDeepCount = 1;*/

	/* point to current thread and next thread. */
	yi_g_nKeCurrentThreadId = 0;
	yi_g_nKeNextThreadId = 0;
	yi_g_pKeCurrentThread = NULL;
	yi_g_pKeNextThread = NULL;


	YIRtlMemoryZero(yi_g_arKeThreadTbl, sizeof(yi_g_arKeThreadTbl));

    YIRtlMemoryZero(&yi_g_arKeReadyThreadTblSt, sizeof(yi_g_arKeReadyThreadTblSt));

	yi_g_pKeFreeThreadList = &yi_g_arKeThreadTbl[CFG_KERNEL_SCHED_MAX_THREAD-1];

	for(nCount = CFG_KERNEL_SCHED_MAX_THREAD-1; nCount>=0 ; nCount--)
	{
		YI_PTHREAD pThread;
		pThread = yi_g_pKeFreeThreadList;
		yi_g_pKeFreeThreadList = &yi_g_arKeThreadTbl[nCount-1];
		yi_g_pKeFreeThreadList->pNextThread = pThread; 
	}
	
	/* Set 0 thread to temporary active thread for first sched reason. */
	yi_g_pKeCurrentThread = (YI_PTHREAD)&yi_g_nKeSRMPhonyThreadStack;
	yi_g_nKeCurrentThreadId = CFG_KERNEL_SCHED_IDLE_PRIORITY+1;
	
	/* Create system timer thread. */
	YIKeSRMCreateThread(CFG_KERNEL_SCHED_SYSTEM_TIME_PRIORITY, YIKeSRMTimerThread,
				yi_g_pKeSRMTimerThreadStack+128-4, NULL, YI_CREATE_SUSPENDED);
	
	/* Create idle thread */
	YIKeSRMCreateThread(CFG_KERNEL_SCHED_IDLE_PRIORITY, YIKeSRMIdleThread,
				yi_g_pKeSRMIdleThreadStack+128-4, NULL, 0);
	
	

	return E_OK;
}



EXTERN YI_STATUS YICALL YIKeSRMCreateThread(UINT nThreadPriority,
	YI_PKERNEL_THREAD_FUNCTION pThreadMain, PADDR pStack, PVOID pData, UINT nFlags)
{
	YI_PTHREAD	pNewTh;	
	FLAGS_T nFlag;

	/* current priority is used? */
	if(nThreadPriority >= CFG_KERNEL_SCHED_MAX_THREAD)
		return ETHPRNG; 	

    YIKeSpinLockSave(0, nFlag);


	pNewTh = &yi_g_arKeThreadTbl[nThreadPriority];

    YIRtlMemoryZero(pNewTh, sizeof(YI_THREAD));    

    pNewTh->nThreadPriority = nThreadPriority;
    pNewTh->nDelayTime = 0;

#if CFG_KERNEL_SCHED_MAX_THREAD <=8
	pNewTh->nMaskBit1D = yi_g_arKeMapTbl[nThreadPriority];
#elif CFG_KERNEL_SCHED_MAX_THREAD <= 64
	pNewTh->nPosition1D = nThreadPriority>>3;
	pNewTh->nMaskBit1D = yi_g_arKeMapTbl[nThreadPriority>>3];
	pNewTh->nMaskBit2D = yi_g_arKeMapTbl[nThreadPriority & 0x07];
#else
	pNewTh->Position1D = nThreadPriority>>6;
	pNewTh->nMaskBit1D = yi_g_arKeMapTbl[nThreadPriority>>6];
	pNewTh->nPosition2D = nThreadPriority>>3;
	pNewTh->nMaskBit2D = yi_g_arKeMapTbl[(nThreadPriority>>3)&0x07];
	pNewTh->nMaskBit3D = yi_g_arKeMapTbl[nThreadPriority & 0x07];

#endif /* CFG_KERNEL_SCHED_MAX_THREAD */

	YIHalPrepareThread(pNewTh, pThreadMain, pStack, pData);

	/* thread start here. */
	if(!(nFlags & YI_CREATE_SUSPENDED))
		YIKeSRMResumeThread(nThreadPriority);
	
	YIKeSpinUnLockSave(0, nFlag);

	return E_OK;
}


EXTERN VOID YIKeSchedSelectThread(VOID)
{
	FLAGS_T nFlag;

    YIKeSpinLockSave(0, nFlag);

    yi_g_nKeNextThreadId = YIKeSRMMaskIdGetHighest(&yi_g_arKeReadyThreadTblSt);
	
	yi_g_pKeNextThread = &yi_g_arKeThreadTbl[yi_g_nKeNextThreadId];

	YIKeSpinUnLockSave(0, nFlag);
}


EXTERN VOID YICALL YIKeSRMSched()
{

	YIHalReSched();

}

EXTERN VOID YICALL YIKeSRMSuspendThread(UINT nThreadPriority)
{
	YI_PTHREAD pTh;
	FLAGS_T nFlag;

    YIKeSpinLockSave(0, nFlag);

	pTh = &yi_g_arKeThreadTbl[nThreadPriority];
	YIKeSRMMaskIdUnMask(&yi_g_arKeReadyThreadTblSt, pTh);

	YIKeSpinUnLockSave(0, nFlag);
	return; 
}

EXTERN VOID YICALL YIKeSRMResumeThread(UINT nThreadPriority)
{
	YI_PTHREAD pTh;
	FLAGS_T nFlag;

    YIKeSpinLockSave(0, nFlag);

	pTh = &yi_g_arKeThreadTbl[nThreadPriority];
	
    YIKeSRMMaskIdMask(&yi_g_arKeReadyThreadTblSt, pTh);

	YIKeSpinUnLockSave(0, nFlag);
	return; 
}

EXTERN YI_STATUS YICALL YIKeSRMWait(UINT nThreadPriority, UINT nTickCount, UINT nType)
{
	switch(nType)
	{
		case YI_WAIT_WITH_TIMEOUT_UNINTERRUPTED:
			yi_g_arKeThreadTbl[nThreadPriority].nDelayTime=nTickCount;
			do{
				YIKeSRMSuspendThread(nThreadPriority);
				YIKeSRMSched();
			}while(yi_g_arKeThreadTbl[nThreadPriority].nDelayTime > 0);
		break;

		case YI_WAIT_WITH_TIMEOUT_INTERRUPTED:
			yi_g_arKeThreadTbl[nThreadPriority].nDelayTime=nTickCount;
			YIKeSRMSuspendThread(nThreadPriority);
			YIKeSRMSched();
		break;

		case YI_WAIT:
			YIKeSRMSuspendThread(nThreadPriority);
			YIKeSRMSched();
		break;
	}
	return 0;
}

EXTERN UINT YIKeSRMMaskIdGetHighest(YI_PTHREAD_MASK_TABLE_STRUCT pMaskTbl)
{
    UINT nHighestId;
#if CFG_KERNEL_SCHED_MAX_THREAD > 8
	UINT n1D;
#endif /* CFG_KERNEL_SCHED_MAX_THREAD */

#if CFG_KERNEL_SCHED_MAX_THREAD > 64
	UINT n2D;
#endif /* CFG_KERNEL_SCHED_MAX_THREAD */

#if CFG_KERNEL_SCHED_MAX_THREAD <=8
    nHighestId = yi_g_arKeUnMapTbl[pMaskTbl->arThreadTblTs1D];
    /* no ready thread? */
    if(nHighestId == CFG_KERNEL_SCHED_IDLE_PRIORITY)
    {
       pMaskTbl->arThreadTblTs1D = pMaskTbl->arThread1D;
    }
    nHighestId = yi_g_arKeUnMapTbl[pMaskTbl->arThreadTblTs1D];
    if(nHighestId != CFG_KERNEL_SCHED_IDLE_PRIORITY)
    {
	    pMaskTbl->arThreadTblTs1D &= ~yi_g_arKeThreadTbl[nHighestId].nMaskBit1D;
    }
 
#elif CFG_KERNEL_SCHED_MAX_THREAD <= 64
	n1D = yi_g_arKeUnMapTbl[pMaskTbl->arThreadTblTs1D];
	nHighestId = (n1D<<3) + yi_g_arKeUnMapTbl[pMaskTbl->arThreadTblTs2D[n1D]];
    if(nHighestId == CFG_KERNEL_SCHED_IDLE_PRIORITY)
    {
        pMaskTbl->arThreadTblTs1D = pMaskTbl->arThreadTbl1D;
        YIRtlMemoryCopy(pMaskTbl->arThreadTblTs2D, pMaskTbl->arThreadTbl2D, sizeof(pMaskTbl->arThreadTblTs2D));
    }
 	n1D = yi_g_arKeUnMapTbl[pMaskTbl->arThreadTblTs1D];
	nHighestId = (n1D<<3) + yi_g_arKeUnMapTbl[pMaskTbl->arThreadTblTs2D[n1D]];
    if(nHighestId != CFG_KERNEL_SCHED_IDLE_PRIORITY)
    {
	    if((pMaskTbl->arThreadTblTs2D[yi_g_arKeThreadTbl[nHighestId].nPosition1D] &= 
        ~yi_g_arKeThreadTbl[nHighestId].nMaskBit2D) == 0)
            pMaskTbl->arThreadTblTs1D &= ~yi_g_arKeThreadTbl[nHighestId].nMaskBit1D;
    }
 
#else /* !!! */
	n1D = yi_g_arKeUnMapTbl[pMaskTbl->arThreadTblTs1D];
	n2D = yi_g_arKeUnMapTbl[pMaskTbl->arThreadTblTs2D[n1D]];
	nHighestId =  (n1D<<6) + (n2D<<3) + yi_g_arkeUnMapTbl[pMaskTbl->arThreadTblTs3D[(n1D<<3)+n2D]];

    if(nHighestId == CFG_KERNEL_SCHED_IDLE_PRIORITY)
    {
        pMaskTbl->arThreadTblTs1D = pMaskTbl->arThreadTbl1D;
        YIRtlMemoryCopy(pMaskTbl->arThreadTblTs2D, pMaskTbl->arThreadTbl2D, sizeof(pMaskTbl->arThreadTblTs2D));
        YIRtlMemoryCopy(pMaskTbl->arThreadTblTs3D, pMaskTbl->arThreadTbl3D, sizeof(pMaskTbl->arThreadTblTs3D));
    }

 	n1D = yi_g_arKeUnMapTbl[pMaskTbl->arThreadTblTs1D];
	n2D = yi_g_arKeUnMapTbl[pMaskTbl->arThreadTblTs2D[n1D]];
	nHighestId =  (n1D<<6) + (n2D<<3) + yi_g_arkeUnMapTbl[pMaskTbl->arThreadTblTs3D[(n1D<<3)+n2D]];

    if(nHighestId != CFG_KERNEL_SCHED_IDLE_PRIORITY)
    {
	    if((pMaskTbl->arThreadTblTs3D[yi_g_arKeThreadTbl[nHighestId].nPosition1D] &= 
            ~yi_g_arKeThreadTbl[nHighestId].nMaskBit3D) == 0)
	        if(pMaskTbl->arThreadTblTs2D[yi_g_arKeThreadTbl[nHighestId].nPosition1D] &= 
                ~yi_g_arKeThreadTbl[nHighestId].nMaskBit2D)
        	    pMaskTbl->arThreadTblTs1D &= ~yi_g_arKeThreadTbl[nHighestId].nMaskBit1D;
    }	
#endif /* CFG_KERNEL_SCHED_MAX_THREAD */
    return nHighestId;
}

EXTERN VOID YIKeSRMMaskIdMask(YI_PTHREAD_MASK_TABLE_STRUCT pMaskTbl, YI_PTHREAD pTh)
{
#if CFG_KERNEL_SCHED_MAX_THREAD <=8
	pMaskTbl->arThreadTbl1D |= pTh->nMaskBit1D;
#elif CFG_KERNEL_SCHED_MAX_THREAD <= 64
	pMaskTbl->arThreadTbl1D |= pTh->nMaskBit1D;
	pMaskTbl->arThreadTbl2D[pTh->nPosition1D] |= pTh->nMaskBit2D;

#else
	pMaskTbl->arThreadTbl1D |= pTh->nMaskBit1D;
	pMaskTbl->arThreadTbl2D[pTh->nPosition1D] |= pTh->nMaskBit2D;
	pMaskTbl->arThreadTbl3D[pTh->nPosition2D] |= pTh->nMaskBit3D;

#endif /* CFG_KERNEL_SCHED_MAX_THREAD */

#if CFG_KERNEL_SCHED_MAX_THREAD <=8
	pMaskTbl->arThreadTblTs1D |= pTh->nMaskBit1D;
#elif CFG_KERNEL_SCHED_MAX_THREAD <= 64
	pMaskTbl->arThreadTblTs1D |= pTh->nMaskBit1D;
	pMaskTbl->arThreadTblTs2D[pTh->nPosition1D] |= pTh->nMaskBit2D;

#else
	pMaskTbl->arThreadTblTs1D |= pTh->nMaskBit1D;
	pMaskTbl->arThreadTblTs2D[pTh->nPosition1D] |= pTh->nMaskBit2D;
	pMaskTbl->arThreadTblTs3D[pTh->nPosition2D] |= pTh->nMaskBit3D;

#endif /* CFG_KERNEL_SCHED_MAX_THREAD */

}

EXTERN VOID YIKeSRMMaskIdUnMask(YI_PTHREAD_MASK_TABLE_STRUCT pMaskTbl, YI_PTHREAD pTh)
{
#if CFG_KERNEL_SCHED_MAX_THREAD <=8
	pMaskTbl->arThreadTbl1D &= ~pTh->nMaskBit1D;
#elif CFG_KERNEL_SCHED_MAX_THREAD <= 64
	if((pMaskTbl->arThreadTbl2D[pTh->nPosition1D] &= ~pTh->nMaskBit2D)
        == 0)
        pMaskTbl->arThreadTbl1D &= ~pTh->nMaskBit1D;
#else
	if((pMaskTbl->arThreadTbl3D[pTh->nPosition2D] &= ~pTh->nMaskBit3D)
        == 0)
	    if(pMaskTbl->arThreadTbl2D[pTh->nPosition1D] &= ~pTh->nMaskBit2D)
        	pMaskTbl->arThreadTbl1D &= ~pTh->nMaskBit1D;
#endif /* CFG_KERNEL_SCHED_MAX_THREAD */

#if CFG_KERNEL_SCHED_MAX_THREAD <=8
	pMaskTbl->arThreadTblTs1D &= ~pTh->nMaskBit1D;
#elif CFG_KERNEL_SCHED_MAX_THREAD <= 64
    if((pMaskTbl->arThreadTblTs2D[pTh->nPosition1D] &= ~pTh->nMaskBit2D)
        == 0)
        pMaskTbl->arThreadTblTs1D &= ~pTh->nMaskBit1D;
#else
	if((pMaskTbl->arThreadTblTs3D[pTh->nPosition2D] &= ~pTh->nMaskBit3D)
        == 0)
	    if(pMaskTbl->arThreadTblTs2D[pTh->nPosition1D] &= ~pTh->nMaskBit2D)
        	pMaskTbl->arThreadTblTs1D &= ~pTh->nMaskBit1D;
#endif /* CFG_KERNEL_SCHED_MAX_THREAD */

}

