
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
 * Filename:	$RCSfile: sched_ts.h,v $
 * Createdate:	2005/06/03 23:12:00
 * FileId:	$Id: sched_ts.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
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


#ifndef _YIOS_SCHED_TS_H_
#define _YIOS_SCHED_TS_H_

#include <yios/config.h>
#include <yios/types.h>

#include <yios/thread.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */



#define CFG_KERNEL_SCHED_SYSTEM_TIME_PRIORITY		0
#define CFG_KERNEL_SCHED_USER_PRIORITY			1

#define CFG_KERNEL_SCHED_IDLE_PRIORITY			(CFG_KERNEL_SCHED_MAX_THREAD - 1)


EXTERN YI_THREAD yi_g_arKeThreadTbl[CFG_KERNEL_SCHED_MAX_THREAD];
EXTERN YI_PTHREAD yi_g_pKeFreeThreadList;

typedef struct _YI_THREAD_MASK_TABLE_STRUCT{
#if CFG_KERNEL_SCHED_MAX_THREAD <= 8
	UINT8 arThreadTbl1D;
#elif CFG_KERNEL_SCHED_MAX_THREAD <= 64
	UINT8 arThreadTbl1D;
	UINT8 arThreadTbl2D[8];
#else/*  CFG_KERNEL_SCHED_MAX_THREAD <= 512 */
	UINT8 arThreadTbl1D;
	UINT8 arThreadTbl2D[8];
	UINT8 arThreadTbl3D[64];
#endif /* CFG_KERNEL_SCHED_MAX_THREAD */

#if CFG_KERNEL_SCHED_MAX_THREAD <= 8
	UINT8 arThreadTblTs1D;
#elif CFG_KERNEL_SCHED_MAX_THREAD <= 64
	UINT8 arThreadTblTs1D;
	UINT8 arThreadTblTs2D[8];
#else/*  CFG_KERNEL_SCHED_MAX_THREAD <= 512 */
	UINT8 arThreadTblTs1D;
	UINT8 arThreadTblTs2D[8];
	UINT8 arThreadTblTs3D[64];
#endif /* CFG_KERNEL_SCHED_MAX_THREAD */


}YI_THREAD_MASK_TABLE_STRUCT, *YI_PTHREAD_MASK_TABLE_STRUCT;

EXTERN YI_THREAD_MASK_TABLE_STRUCT yi_g_arKeReadyThreadTblSt;
/*
#if CFG_KERNEL_SCHED_MAX_THREAD <= 8
	EXTERN UINT8 yi_g_arKeReadyThreadTbl1D;
#elif CFG_KERNEL_SCHED_MAX_THREAD <= 64
	EXTERN UINT8 yi_g_arKeReadyThreadTbl1D;
	EXTERN UINT8 yi_g_arKeReadyThreadTbl2D[8];
#else*/
	/*  CFG_KERNEL_SCHED_MAX_THREAD <= 512 */
	/* EXTERN UINT8 yi_g_arKeReadyThreadTbl1D;
	EXTERN UINT8 yi_g_arKeReadyThreadTbl2D[8];
	EXTERN UINT8 yi_g_arKeReadyThreadTbl3D[64];*/
/* #endif*/ /* CFG_KERNEL_SCHED_MAX_THREAD */


EXTERN YI_STATUS YICALL YIKeSchedRMInit();

EXTERN YI_STATUS YICALL YIKeSRMCreateThread(UINT nThreadPriority,
        YI_PKERNEL_THREAD_FUNCTION pThreadMain, PADDR pStack, PVOID pData, UINT nFlags);

EXTERN VOID YICALL YIKeSRMSched();

EXTERN VOID YICALL YIKeSRMSuspendThread(UINT nThreadPriority);
EXTERN VOID YICALL YIKeSRMResumeThread(UINT nThreadPriority);

EXTERN YI_STATUS YICALL YIKeSRMWait(UINT nThreadPriority, UINT nTickCount, UINT nType);

EXTERN VOID YIKeSchedSelectThread(VOID);


EXTERN UINT YIKeSRMMaskIdGetHighest(YI_PTHREAD_MASK_TABLE_STRUCT pMaskTbl);
EXTERN VOID YIKeSRMMaskIdMask(YI_PTHREAD_MASK_TABLE_STRUCT pMaskTbl, YI_PTHREAD PTh);
EXTERN VOID YIKeSRMMaskIdUnMask(YI_PTHREAD_MASK_TABLE_STRUCT pMaskTbl, YI_PTHREAD PTh);

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _YIOS_SCHED_TS_H_ */



