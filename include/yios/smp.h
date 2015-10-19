
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
 * Filename:	$RCSfile: smp.h,v $
 * Createdate:	2005/02/26 15:18:00
 * FileId:	$Id: smp.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
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


#ifndef _YIOS_SMP_H_
#define _YIOS_SMP_H_

#include <yios/config.h>
#include <yios/types.h>

#include <yios/thread.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

/* Cpu current status struct. */
typedef struct _YI_SMP_CPU_STATUS{
    VOLATILE    INT    nIntDeepCount;
    INT         nCurrentThreadId;
    INT         nNextThreadId;
    YI_PTHREAD  pCurrentThread;
    YI_PTHREAD  pNextThread;
}YI_SMP_CPU_STATUS, *YI_PSMP_CPU_STATUS;


/* how deep in current intterupte or trap. */
EXTERN VOLATILE UINT yi_g_nKeIntTrapDeepCount;

/* sched thread when yi_g_nKeNeedReSched is't 0 */
/* EXTERN VOLATILE UINT yi_g_nKeDisSchedDeepCount;*/

/* point to current thread and next thread. */
EXTERN UINT yi_g_nKeCurrentThreadId;
EXTERN UINT yi_g_nKeNextThreadId;
EXTERN YI_PTHREAD yi_g_pKeCurrentThread;
EXTERN YI_PTHREAD yi_g_pKeNextThread;

#define YIKeGetCurrentThreadId()    yi_g_nKeCurrentThreadId
#define YIKeGetCurrentThread()      yi_g_pKeCurrentThread
/* EXTERN VOID YIKeSchedSelectThread(VOID); */

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _YIOS_SMP_H_ */



