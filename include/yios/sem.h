
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
 * Filename:	$RCSfile: sem.h,v $
 * Createdate:	2005/01/13 22:38:00
 * FileId:      $Id: sem.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
 * Last update:	$Date: 2005/08/09 05:25:23 $
 * Creator:     Devin Wang <vhd@21cn.com>
 * Version:     $Revision: 1.1.1.1 $
 * Description: kernel semaphore routine
 *
 * ====================================================================
 */

/* ====================================================================
 *  Revision:
 *
 * ====================================================================
 */


#ifndef _YIOS_SEM_H_
#define _YIOS_SEM_H_

#include <yios/config.h>
#include <yios/types.h>
#include <yios/list.h>
#include <yios/lock.h>
#include <yios/atom.h>
#include <yios/sched.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

typedef struct _YI_SEMAPHORE{
#if CFG_KERNEL_SCHED_METHOD_RM == 1

    YI_THREAD_MASK_TABLE_STRUCT arWaitThreadTblSt;
#endif /* CFG_KERNEL_SCHED_METHOD_RM */
#if CFG_KERNEL_SCHED_METHOD_TIMESLICE == 1

    YI_THREAD_MASK_TABLE_STRUCT arWaitThreadTblSt;
#endif /* CFG_KERNEL_SCHED_METHOD_TIMESLICE */


   /*  YI_LIST_HEAD	stSemThreadChain;*/

   SPINLOCK_T      nLock;
    YI_ATOM          stAtomCount;
}YI_SEMAPHORE, *YI_PSEMAPHORE;

EXTERN YI_STATUS YIKeSemInit(YI_PSEMAPHORE pSem, INT nInitCount);

EXTERN VOID YIKeSemLock(YI_PSEMAPHORE pSem);

EXTERN VOID YIKeSemUnLock(YI_PSEMAPHORE pSem);

/* try to lock semaphore without waiting. */
EXTERN INT YIKeSemTryLock(YI_PSEMAPHORE pSem);

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _YIOS_SEM_H_ */



