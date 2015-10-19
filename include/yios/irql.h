
/* ====================================================================
 * The YingOS project.
 * Copyright (C) 2004-2005 Devin Wang

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
 * Filename:	$RCSfile: irql.h,v $
 * Createdate:	2005/02/07 18:11:00
 * FileId:      $Id: irql.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
 * Last update:	$Date: 2005/08/09 05:25:23 $
 * Creator:     Devin Wang <vhd@21cn.com>
 * Version:     $Revision: 1.1.1.1 $
 * Description: kernel irq level
 *
 * ====================================================================
 */

/* ====================================================================
 *  Revision:
 *
 * ====================================================================
 */


#ifndef _YIOS_IRQL_H_
#define _YIOS_IRQL_H_

#include <yios/config.h>
#include <yios/types.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */


/* System irq levlel definition */
/*
    Critical level: 32  0xe0~0xff
    Hard irq level: 128 0x60~0xdf
    Soft irq level: 32  0x40~0x5f
    Sched    level: 64  0x00~0x3f
*/

#define YIOS_IRQL_HIGHEST               0xff
#define YIOS_IRQL_SYSCRASH              0xfc
#define YIOS_IRQL_POWERFUT              0xf8
#define YIOS_IRQL_SMPSYN                0xf0
#define YIOS_IRQL_INTDIS                0xe0

#define YIOS_IRQL_HARDIRQ_HIGHEST       0xdf
#define YIOS_IRQL_HARDIRQ_LOWEST        0x60


#define YIOS_IRQL_SOFTIRQ_HIGHEST       0x5f
#define YIOS_IRQL_SOFTIRQ_LOWEST        0x40

#define YIOS_IRQL_SCHED_HIGHEST         0x3f
#define YIOS_IRQL_SCHED_LOWEST          0x00

#define YIOS_IRQL_LOWEST                0x00

 
#define YIKeLowerIrql()                 YIHalLowerIrql()
#define YIKeCurrentIrql()               YIHalCurrentIrql()

#define YIKeRaiseIrqlToHDI(x)           YIHalRaiseIrqlToHDI(x)
#define YIKeRaiseIrqlToSFI(x)           YIHalRaiseIrqlToSFI(x)

#define YIKeRaiseIrqlTo

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _YIOS_IRQL_H_ */



