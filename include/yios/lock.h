
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
 * Filename:	$RCSfile: lock.h,v $
 * Createdate:	2005/02/09 21:51:00
 * FileId:	$Id: lock.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
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


#ifndef _YIOS_LOCK_H_
#define _YIOS_LOCK_H_

#include <yios/config.h>
#include <yios/types.h>

#include <yios/hal/cpu.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

#define YIKeSpinLockInit(__lock) \
    do{}while(0)


#define YIKeSpinLockSave(__lock, __save)  \
    do{  YIHalDisIntSave((__save));  _YIHalSpinLock((__lock));}while(0)

#define YIKeSpinLock(__lock)  \
    do{  YIHalDisInt();  _YIHalSpinLock((__lock));}while(0)

#define YIKeSpinUnLockSave(__lock, __save ) \
    do{ _YIHalSpinUnLock((__lock)); YIHalIntRestore((__save));}while(0)

#define YIKeSpinUnLock(__lock ) \
    do{ _YIHalSpinUnLock((__lock)); YIHalEnInt();}while(0)

typedef struct _SPINLOCK_T{
    VOLATILE INT nLock;
}SPINLOCK_T, *PSPINLOCK_T;

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _YIOS_LOCK_H_ */



