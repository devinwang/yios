
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
 * Filename:	$RCSfile: sched.c,v $
 * Createdate:	2004/11/29 22:16:00
 * FileId:	$Id: sched.c,v 1.1.1.1 2005/08/09 05:25:25 devin Exp $
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

#include <yios/sched.h>


/* how deep in current intterupte or trap. */
VOLATILE UINT yi_g_nKeIntTrapDeepCount = 0;

/* sched thread when yi_g_nKeDisSchedDeepCount is 0 */
VOLATILE UINT yi_g_nKeDisSchedDeepCount = 0;

/* We should kown how deep in current critical call. */
/* VOLATILE UINT yi_g_nKeEnterCriticalDeepCount;*/

/* point to current thread and next thread. */
UINT yi_g_nKeCurrentThreadId = 0;
UINT yi_g_nKeNextThreadId = 0;
YI_PTHREAD yi_g_pKeCurrentThread = NULL;
YI_PTHREAD yi_g_pKeNextThread = NULL;

