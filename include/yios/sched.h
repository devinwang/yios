
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
 * Filename:	$RCSfile: sched.h,v $
 * Createdate:	2004/07/25 18:42:00
 * FileId:	$Id: sched.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
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


#ifndef _YIOS_SCHED_H_
#define _YIOS_SCHED_H_

#include <yios/config.h>
#include <yios/types.h>

#include <yios/thread.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */


EXTERN YI_STATUS YICALL YIKeSchedRMInit();

EXTERN YI_STATUS YICALL YIKeSRMCreateThread(UINT nThreadPriority,
        YI_PKERNEL_THREAD_FUNCTION pThreadMain, PADDR pStack, PVOID pData, UINT nFlags);

EXTERN VOID YICALL YIKeSRMSched();

EXTERN VOID YICALL YIKeSRMSuspendThread(UINT nThreadPriority);
EXTERN VOID YICALL YIKeSRMResumeThread(UINT nThreadPriority);

EXTERN YI_STATUS YICALL YIKeSRMWait(UINT nThreadPriority, UINT nTickCount, UINT nType);

EXTERN VOID YIKeSchedSelectThread(VOID);
/* 
typedef struct _YI_OBJ_SCHEDULER* PYI_OBJ_SCHEDULER;

typedef struct _YI_OBJ_SCHEDULER{
	UINT		nSchedulerType;
	PYI_OBJ_SCHEDULER pNextScheduler;
	YI_STATUS YICALL (*pfSchedInit)(VOID);
	YI_STATUS YICALL (*pfSchedFini)(VOID);
	YI_STATUS YICALL (*pfCreateThread)();
	YI_STATUS YICALL (*pfSuspendThread)();
	YI_STATUS YICALL (*pfResumeThread)();
	YI_STATUS YICALL (*pfSelectThread)();
	YI_STATUS YICALL (*pfReSched)();
}YI_OBJ_SCHEDULER;
*/

/* wait method */
#define YI_WAIT_WITH_TIMEOUT_UNINTERRUPTED		1
#define YI_WAIT_WITH_TIMEOUT_INTERRUPTED		2
#define YI_WAIT						3

/* create thread memthod */
#define YI_CREATE_SUSPENDED				0x0001

/* how deep in current intterupte or trap. */
EXTERN VOLATILE UINT yi_g_nKeIntTrapDeepCount;

/* sched thread when yi_g_nKeNeedReSched is't 0 */
EXTERN VOLATILE UINT yi_g_nKeDisSchedDeepCount;

/* We should kown how deep in current critical call. */
/* EXTERN VOLATILE UINT yi_g_nKeEnterCriticalDeepCount;*/


/* point to current thread and next thread. */
EXTERN UINT yi_g_nKeCurrentThreadId;
EXTERN UINT yi_g_nKeNextThreadId;
EXTERN YI_PTHREAD yi_g_pKeCurrentThread;
EXTERN YI_PTHREAD yi_g_pKeNextThread;

#if CFG_KERNEL_SCHED_METHOD_RM == 1

#include <yios/sched_rm.h>

#define YIKeGetCurrentThreadId()    yi_g_nKeCurrentThreadId
#define YIKeGetCurrentThread()      yi_g_pKeCurrentThread
/* EXTERN VOID YIKeSchedSelectThread(VOID); */


#endif /* CFG_KERNEL_SCHED_METHOD_RM */

#if CFG_KERNEL_SCHED_METHOD_TIMESLICE == 1 
#include <yios/sched_ts.h>

#define YIKeGetCurrentThreadId()    yi_g_nKeCurrentThreadId
#define YIKeGetCurrentThread()      yi_g_pKeCurrentThread

#endif /* CFG_KERNEL_SCHED_METHOD */




#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _YIOS_SCHED_H_ */



