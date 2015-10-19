
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
 * Filename:	$RCSfile: cpu.h,v $
 * Createdate:	2004/06/17 00:40:00
 * FileId:	$Id: cpu.h,v 1.1.1.1 2005/08/09 05:25:23 devin Exp $
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


#ifndef _YIOS_HAL_CPU_H_
#define _YIOS_HAL_CPU_H_

#include <yios/config.h>
#include <yios/types.h>

#include <yios/hal/segset.h>
#include <yios/hal/pageset.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */


#define YI_HAL_CPU_SYSLEVEL	0x0
#define	YI_HAL_CPU_USERLEVEL	0x3

#define YI_HAL_CPU_CALLGATE	0x0c00
#define YI_HAL_CPU_INTGATE	0x0e00
#define YI_HAL_CPU_TRAPGATE	0x0f00

typedef struct _YI_HAL_I386CPU_REGS{
   UINT32	ebp;
   UINT32	edi;
   UINT32	esi;
   UINT32	edx;
   UINT32	ecx;
   UINT32	ebx;
   UINT32	eax;
   UINT32	es;
   UINT32	ds;
   UINT32	int_num;
   UINT32	code;
   UINT32	old_eip;
   UINT32	old_cs;
   UINT32	old_eflags;
   UINT32	old_esp;
   UINT32	old_ss;
}YI_HAL_I386CPU_REGS;

typedef YI_HAL_I386CPU_REGS	YI_HAL_CPU_REGS;

/* Thread scheder use the struct to save cpu status. */
 typedef struct _YI_HAL_I386CPU_CONTEXT_REGS{
	UINT32	esp;
}YI_HAL_I386CPU_CONTEXT_REGS; 

typedef YI_HAL_I386CPU_CONTEXT_REGS	YI_CPU_CONTEXT_REGS;


/*
#define YIHalDisableExtInt()	__asm__ __volatile__("cli");
#define YIHalEnableExtInt()	__asm__	__volatile__("sti");

#define YIHalEnterCritical()	do{ __asm__ __volatile__("cli"); yi_g_nKeEnterCriticalDeepCount++;}while(0)
#define YIHalExitCritical()	do{ if(--yi_g_nKeEnterCriticalDeepCount==0) \
					 __asm__ __volatile__("sti");}while(0)
*/

#define YIHalDisInt()   __asm__ __volatile__("cli")
#define YIHalEnInt()    __asm__ __volatile__("sti")

#define YIHalDisIntSave(__save)  __asm__ __volatile__("pushfl ; popl %0; cli":"=g"(__save): ); 
#define YIHalIntRestore(__save)  __asm__ __volatile__("pushl %0 ; popfl": :"g"(__save):"memory", "cc")


#if CFG_SMP>0
#else /* Not CFG_CMP */
#define _YIHalSpinLock(__lock)
#define _YIHalSpinUnLock(__lock)
#endif /* CFG_SMP */

#define YIHalReSched()		__asm__ __volatile__("int $0x80");



#define YIHalSchedSwitchContext(__nNewThreadId, __pNewThread)	\
	do{							\
		__asm__ __volatile__(				\
			"pushal	\n\t"				\
			"movl	%%esp, %0\n\t"			\
			"movl	%1, %%esp\n\t"			\
			"popal	\n\t"				\
		:"=m"(yi_g_pKeCurrentThread->stCtxRegs.esp)	\
		:"m"((__pNewThread)->stCtxRegs.esp));		\
		yi_g_pKeCurrentThread = (__pNewThread);		\
		yi_g_nKeCurrentThreadId = (__nNewThreadId);	\
	}while(0)
	

#define YIHalPrepareThread(__pThread, __pThreadMain, __pStack, __pData)	\
	do{								\
		/*__pStack-=4*/; (*(PUINT)__pStack) = (UINT)__pData;	\
		__pStack-=4;	/* no use ret eip */			\
		__pStack-=4; (*(PUINT)__pStack) = 0x00000202;/* eflags */		\
		__pStack-=4; (*(PUINT)__pStack) = YI_HAL_KE_CS_SELC;	\
		__pStack-=4; (*(PUINT)__pStack) = ((UINT)(PADDR)__pThreadMain);	\
		__pStack-=8; 	/* error code and interrupt number */	\
		__pStack-=4; (*(PUINT)__pStack) = YI_HAL_KE_DS_SELC;	\
		__pStack-=4; (*(PUINT)__pStack) = YI_HAL_KE_DS_SELC;	\
		__pStack -= (4*7);					\
		(__pThread)->pStack = (PVOID)__pStack;			\
	}while(0)

#define YIHalIdleCall()		__asm__ __volatile__("hlt");

#define YIHLT()     	__asm__("cli\n\thlt")
#define YIDebugPoint()	__asm__("int $3")

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _YIOS_HAL_CPU_H_ */



