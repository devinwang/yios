
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
 * Filename:	$RCSfile: thread.h,v $
 * Createdate:	2004/05/31 15:32:00
 * FileId:	$Id: thread.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
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


#ifndef _YIOS_THREAD_H_
#define _YIOS_THREAD_H_

#include <yios/config.h>
#include <yios/types.h>
#include <yios/hal/cpu.h>


#if CFG_FS == 1
#include <yios/file.h>
#endif /* CFG_FS */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */


  typedef struct _YI_THREAD_* YI_PTHREAD;

#if CFG_KERNEL_SCHED_METHOD_TIMESLICE == 1
#if 0
  typedef struct _YI_PROCESS_* _YI_PPROCESS;

  /* All user task is process. */
  typedef struct _YI_PROCESS_{
    PID_T nPID; /* Process id. */

    YI_PPROCESS pParentProcess;

    YI_PPROCESS pBrotherProcessNext;
    YI_PPROCESS pBrotherProcessPrev;

    YI_PTHREAD pChildThreadNext;
    YI_PTHREAD pChildThreadPrev;


  }YI_PROCESS;
#endif /* 0 */
#endif /* CFG_KERNEL_SCHED_METHOD_TIMESLICE */

  /* All kernel task is thread, user task (prcess) have a number of threads. */
  typedef struct _YI_THREAD_ {
    PADDR	pStack;
    /* TID_T nTID; */ /* Thread id. */
    /* UINT nThreadPriority;*/
    UINT	nThreadPriority;

    YI_PTHREAD pNextThread;

#if CFG_KERNEL_SCHED_METHOD_RM == 1
#if CFG_KERNEL_SCHED_MAX_THREAD <= 8
    UINT8	nMaskBit1D;
/*    UINT8	nPosition1D;*/
#elif CFG_KERNEL_SCHED_MAX_THREAD <= 64    
    UINT8	nMaskBit1D;
    UINT8	nPosition1D;	
    UINT8	nMaskBit2D;
/*    UINT8	nPosition2D;	*/
#else
    UINT8	nMaskBit1D;
    UINT8	nPosition1D;	
    UINT8	nMaskBit2D;
    UINT8	nPosition2D;	
    UINT8	nMaskBit3D;
/*    UINT8	nPosition3D;	*/
#endif /* CFG_KERNEL_SCHED_MAX THREAD */

    INT		nDelayTime;

#endif /* CFG_KERNEL_SCHED_METHOD_RM */

#if CFG_KERNEL_SCHED_METHOD_TIMESLICE == 1
#if CFG_KERNEL_SCHED_MAX_THREAD <= 8
    UINT8	nMaskBit1D;
/*    UINT8	nPosition1D;*/
#elif CFG_KERNEL_SCHED_MAX_THREAD <= 64    
    UINT8	nMaskBit1D;
    UINT8	nPosition1D;	
    UINT8	nMaskBit2D;
/*    UINT8	nPosition2D;	*/
#else
    UINT8	nMaskBit1D;
    UINT8	nPosition1D;	
    UINT8	nMaskBit2D;
    UINT8	nPosition2D;	
    UINT8	nMaskBit3D;
/*    UINT8	nPosition3D;	*/
#endif /* CFG_KERNEL_SCHED_MAX THREAD */

    INT		nDelayTime;

#endif /* CFG_KERNEL_SCHED_METHOD_TIMESLICE */


    YI_CPU_CONTEXT_REGS	stCtxRegs;

#if CFG_KERNEL_SCHED_HAVE_THREAD_RELATIONSHIP != 0
    TID_T nPTID; /* Parent thread id*/

    YI_PTHREAD pParentThread;

    YI_PTHREAD pBrotherThreadNext;
    YI_PTHREAD pBrotherThreadPrev;

    YI_PTHREAD pChildThreadNext;
    YI_PTHREAD pChildThreadPrev;
#endif /* CFG_KERNEL_SCHED_HAVE_THREAD_RELATIONSHIP */

#if CFG_KERNEL_SCHED_METHOD_TIMESLICE == 1
    PID_T nPID; /* mother process id. */
/*    YI_PPROCESS pMotherProcess;*/
#endif /* CFG_KERNEL_SCHED_METHOD_TIMESLICE */

/* file system support */
#if CFG_FS == 1
    YI_PFS_FILE_HANDLE	pFileHandle;
#endif /* CFG_FS */

  }YI_THREAD;

/* thread main roution type.  */
typedef YI_STATUS (YICALL *YI_PKERNEL_THREAD_FUNCTION)(PVOID pData);


#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _YIOS_THREAD_H_ */



