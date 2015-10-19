
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
 * Filename:	$RCSfile: int.h,v $
 * Createdate:	2004/09/14 11:55:00
 * FileId:	$Id: int.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
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


#ifndef _YIOS_INT_H_
#define _YIOS_INT_H_

#include <yios/config.h>
#include <yios/hal/int.h>
#include <yios/types.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

/* Interrupt mask */
#define YI_INT_DISABLE  0x0001
#define YI_INT_SHARE    0x0002

/* interrupt request handle mask. */
#define YI_IRQM_DISABLE  0x0001
#define YI_IRQM_SHARE    0x0002

/* interrupt request handle status. */
#define YI_IRQS_PENDING  0x0001
#define YI_IRQS_DONE     0x0002


typedef struct _YI_IRQ_HANDLE_STRUCT *YI_PIRQ_HANDLE_STRUCT;
typedef struct _YI_IRQ_INTERRUPTER *YI_PIRQ_INTERRUPTER;

/* Interrute handle function type. */
typedef VOID (__INT_HANDLE_ROUTINE)(INT, YI_HAL_CPU_REGS* /* cpu registers in stack */,
                                    PVOID );

/* function handle */
typedef __INT_HANDLE_ROUTINE       *YI_PIRQ_HANDLE_ROUTINE;
typedef YI_PIRQ_HANDLE_ROUTINE      YI_PTRAP_HANDLE_ROUTINE;

/* description structure of interrupt control. */
typedef struct _YI_IRQ_INTERRUPTER{
    VOID (*pDevInit)(INT nIrqNum);
    VOID (*pDevFini)(INT nIrqNum);
    VOID (*pIntEnable)(INT nIrqNum);
    VOID (*pIntDisable)(INT nIrqNum);
    VOID (*pIntAck)(INT nIrqNum);
    VOID (*pIntEnd)(INT nIrqNum);
    INT                     nMask;
    YI_PIRQ_HANDLE_STRUCT   pHandle;
}YI_IRQ_INTERRUPTER;

/* Irq routine handle */
typedef struct _YI_IRQ_HANDLE_STRUCT{
    INT     nCount;             /* handle count. */
    INT     nMask;
    INT     nStatus;
    YI_PIRQ_HANDLE_ROUTINE  pHandle;
    YI_PIRQ_INTERRUPTER     pInterrupter;
    YI_PIRQ_HANDLE_STRUCT   pNext;
}YI_IRQ_HANDLE_STRUCT;



EXTERN UINT yi_g_nInterruptCount;

EXTERN YI_STATUS YICALL YIIntInitialize(VOID);

/* Interruter channel table. */
EXTERN  YI_IRQ_INTERRUPTER yi_g_arInterrupterTbl[CFG_INT_MAX_INTERRUPTER];

/* install handle for one interrupter. */
EXTERN YI_STATUS YIIntInstallHandle(INT nIntNum, YI_PIRQ_HANDLE_STRUCT pHandle);

/* uninstall handle for one interrupter. */
EXTERN YI_STATUS YIIntUnstallHandle(INT nIntNum, YI_PIRQ_HANDLE_STRUCT pHandle);

/* Call all installed interrupter handle. */
EXTERN VOID INLINE YIIntDoInterrupterHandle(INT nIntNum, YI_HAL_CPU_REGS* pRegs, PVOID pData);

/* Call all trap handle. */
EXTERN VOID INLINE YIIntDoTrapHandle(INT nIntNum, YI_HAL_CPU_REGS* pRegs, PVOID pData);

/* set system trap handle. */
EXTERN YI_STATUS YIIntSetTrapHandle(INT nIntNum, YI_PTRAP_HANDLE_ROUTINE pFn, PVOID pData);


#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _YIOS_INT_H_ */

