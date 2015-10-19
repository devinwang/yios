
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
 * Filename:	$RCSfile: time.h,v $
 * Createdate:	2004/07/25 18:42:00
 * FileId:	$Id: time.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
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


#ifndef _YIOS_TIME_H_
#define _YIOS_TIME_H_

#include <yios/config.h>
#include <yios/types.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

EXTERN VOLATILE UINT yi_g_nTimeTickCount;


EXTERN VOID YICALL YIKeTimerRoutine(UINT nTimerNum);

#if CFG_KERNEL_SCHED_METHOD == CFG_KERNEL_SCHED_METHOD_RM

#if CFG_KERNEL_SCHED_MAX_THREAD <= 8
#elif CFG_KERNEL_SCHED_MAX_THREAD <= 64
#else
#endif /* CFG_KERNEL_SCHED_MAX_THREAD */

#endif /* CFG_KERNEL_SCHED_METHOD */

#if CFG_KERNEL_SCHED_METHOD == CFG_KERNEL_SCHED_METHOD_TIMESLICE

#endif /* CFG_KERNEL_SCHED_METHOD */

EXTERN YI_STATUS YITimeInitialize(VOID);


#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _YIOS_TIME_H_ */



