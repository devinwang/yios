
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
 * Filename:	$RCSfile: int.c,v $
 * Createdate:	2004/09/14 17:57:00
 * FileId:	$Id: int.c,v 1.1.1.1 2005/08/09 05:25:24 devin Exp $
 * Last update:	$Date: 2005/08/09 05:25:24 $
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

#include <yios/hal/int.h>
#include <yios/hal/init.h>
#include <yios/hal/pageset.h>

#include <yios/hal/time.h>

#include <yios/int.h>
#include <yios/sched.h>

/* #include <yios/syscall.h>*/

#define	YI_HAL_INT_STR_SOMETHING(x)	#x
#define	YI_HAL_INT_GATE_NAME(x)		_YIHalIntGate##x
#define	YI_HAL_INT_GATE_FUN_DECLARE(x)	VOID   YI_HAL_CMPATTR_USED _YIHalIntGate##x(VOID)

#define YI_HAL_INT_GATE(x)				\
	/*VOID __YIHalIntGate##x(VOID);*/			\
	__asm__ /* __volatile__*/ (			\
        ".text                      \n\t"\
		".align 4, 0x90				\n\t"\
        ".global _YIHalIntGate"#x"  \n\t"\
		"_YIHalIntGate"#x":			\n\t"\
		"pushl $0x0				\n\t"\
		"pushl $"#x"				\n\t"\
		"jmp __YIHalIntGateCommon		\n\t"\
)

#define YI_HAL_INT_GATE_WITH_CODE(x)			\
/*	VOID __YIHalIntGate##x(VOID);*/			\
	__asm__ /* __volatile__ */(			\
        ".text                      \n\t"\
		".align 4, 0x90				\n\t"\
        ".global _YIHalIntGate"#x"  \n\t"\
		"_YIHalIntGate"#x":			\n\t"\
		"pushl $"#x"				\n\t"\
		"jmp __YIHalIntGateCommon		\n\t"\
)
/*		YI_HAL_INT_SAVE_ALL_REGS\
		"call YIHalCommonInterruteFun\n\t"\
		YI_HAL_INT_RESTORE_ALL_REGS\
		"addl $0x8, %esp\n\t"\
		"iret\n\t"\*/

__asm__(
".text						\n\t"
".align 4, 0x90					\n\t"
"__YIHalIntGateCommon:				\n\t"
	YI_HAL_INT_SAVE_ALL_REGS
"	call YIHalCommonInterruptFun		\n\t"

/* this is last interupt call? */
"	cli					\n\t"
"	movl yi_g_nKeIntTrapDeepCount, %eax	\n\t"
"	decl %eax				\n\t"
"	movl %eax, yi_g_nKeIntTrapDeepCount	\n\t"
"	jnz __is_deep_call			\n\t"
/* Scheder been disabled? */
"	movl yi_g_nKeDisSchedDeepCount, %edx	\n\t"
"	testl %edx, %edx			\n\t"
"	jnz __sched_disabled			\n\t"

/* Select one nice thread. */
"	call YIKeSchedSelectThread		\n\t"
"	movl yi_g_nKeNextThreadId, %eax		\n\t"
"	cmpl %eax, yi_g_nKeCurrentThreadId	\n\t"
"	jz __same_thread			\n\t"
"	movl %eax, yi_g_nKeCurrentThreadId	\n\t"

/* Switch thread . */
"	movl yi_g_pKeCurrentThread, %ebx	\n\t"
"	movl %esp, (%ebx)			\n\t"
"	movl yi_g_pKeNextThread, %edx		\n\t"
"	movl (%edx), %esp			\n\t"
"	movl %edx, yi_g_pKeCurrentThread	\n\t"
"__is_deep_call:				\n\t"
"__sched_disabled:				\n\t"
"__same_thread:					\n\t"
	YI_HAL_INT_RESTORE_ALL_REGS
"	addl $0x8, %esp				\n\t"
"	iret					\n\t"
);

/* Interrute gates declare */
YI_HAL_INT_GATE_FUN_DECLARE(0x00);
YI_HAL_INT_GATE_FUN_DECLARE(0x01);
YI_HAL_INT_GATE_FUN_DECLARE(0x02);
YI_HAL_INT_GATE_FUN_DECLARE(0x03);
YI_HAL_INT_GATE_FUN_DECLARE(0x04);
YI_HAL_INT_GATE_FUN_DECLARE(0x05);
YI_HAL_INT_GATE_FUN_DECLARE(0x06);
YI_HAL_INT_GATE_FUN_DECLARE(0x07);
YI_HAL_INT_GATE_FUN_DECLARE(0x08);
YI_HAL_INT_GATE_FUN_DECLARE(0x09);
YI_HAL_INT_GATE_FUN_DECLARE(0x0a);
YI_HAL_INT_GATE_FUN_DECLARE(0x0b);
YI_HAL_INT_GATE_FUN_DECLARE(0x0c);
YI_HAL_INT_GATE_FUN_DECLARE(0x0d);
YI_HAL_INT_GATE_FUN_DECLARE(0x0e);
YI_HAL_INT_GATE_FUN_DECLARE(0x10);

/* System call gate */
YI_HAL_INT_GATE_FUN_DECLARE(0x80);

/* Extern interrute gate. */
YI_HAL_INT_GATE_FUN_DECLARE(0x40);
YI_HAL_INT_GATE_FUN_DECLARE(0x41);
YI_HAL_INT_GATE_FUN_DECLARE(0x42);
YI_HAL_INT_GATE_FUN_DECLARE(0x43);
YI_HAL_INT_GATE_FUN_DECLARE(0x44);
YI_HAL_INT_GATE_FUN_DECLARE(0x45);
YI_HAL_INT_GATE_FUN_DECLARE(0x46);
YI_HAL_INT_GATE_FUN_DECLARE(0x47);
YI_HAL_INT_GATE_FUN_DECLARE(0x48);
YI_HAL_INT_GATE_FUN_DECLARE(0x49);
YI_HAL_INT_GATE_FUN_DECLARE(0x4a);
YI_HAL_INT_GATE_FUN_DECLARE(0x4b);
YI_HAL_INT_GATE_FUN_DECLARE(0x4c);
YI_HAL_INT_GATE_FUN_DECLARE(0x4d);
YI_HAL_INT_GATE_FUN_DECLARE(0x4e);
YI_HAL_INT_GATE_FUN_DECLARE(0x4f);


/* interrut count. */
UINT yi_g_nInterruptCount=0;


YI_HAL_INT_GATE(0x00);			/* */
YI_HAL_INT_GATE(0x01);
YI_HAL_INT_GATE(0x02);
YI_HAL_INT_GATE(0x03);
YI_HAL_INT_GATE(0x04);
YI_HAL_INT_GATE(0x05);
YI_HAL_INT_GATE(0x06);
YI_HAL_INT_GATE(0x07);
YI_HAL_INT_GATE_WITH_CODE(0x08);
YI_HAL_INT_GATE(0x09);
YI_HAL_INT_GATE_WITH_CODE(0x0a);
YI_HAL_INT_GATE_WITH_CODE(0x0b);
YI_HAL_INT_GATE_WITH_CODE(0x0c);
YI_HAL_INT_GATE_WITH_CODE(0x0d);
YI_HAL_INT_GATE_WITH_CODE(0x0e);
YI_HAL_INT_GATE(0x10);

/* System call gate */
YI_HAL_INT_GATE(0x80);

/* Extern interrute gate. */
YI_HAL_INT_GATE(0x40);
YI_HAL_INT_GATE(0x41);
YI_HAL_INT_GATE(0x42);
YI_HAL_INT_GATE(0x43);
YI_HAL_INT_GATE(0x44);
YI_HAL_INT_GATE(0x45);
YI_HAL_INT_GATE(0x46);
YI_HAL_INT_GATE(0x47);
YI_HAL_INT_GATE(0x48);
YI_HAL_INT_GATE(0x49);
YI_HAL_INT_GATE(0x4a);
YI_HAL_INT_GATE(0x4b);
YI_HAL_INT_GATE(0x4c);
YI_HAL_INT_GATE(0x4d);
YI_HAL_INT_GATE(0x4e);
YI_HAL_INT_GATE(0x4f);

/* All traps and interrutes common function. */
STATIC VOID CDECLCALL YI_HAL_CMPATTR_USED YIHalCommonInterruptFun(YI_HAL_CPU_REGS sRegs);

/* Null function for initizlize function handle array. */
/* STATIC VOID YIHalSystemNullIntFun(INT nIntNum, YI_HAL_CPU_REGS* pRegs, PVOID pData);*/

EXTERN VOID YIHalSetIntTblItem(INT iIntNum, PVOID pFn, UINT nType, UINT nDpl)
{
   PADDR pIdtAddress;

   UINT16 wPDplTypeMask;

   if(iIntNum > 0xff)
      return ;
 
   pIdtAddress   =  (PADDR)(YI_P2V(YI_KE_IDT_PADDR)+(iIntNum*8));
  
   if(nDpl == YI_HAL_CPU_SYSLEVEL)
      wPDplTypeMask = 0x8000;
   else
      wPDplTypeMask = 0xe000;

   switch(nType)
     {
     case YI_HAL_CPU_INTGATE:
       wPDplTypeMask |= 0x0e00;
     break;

     case YI_HAL_CPU_TRAPGATE:
       wPDplTypeMask |= 0x0f00;
     break;

     case YI_HAL_CPU_CALLGATE:
       wPDplTypeMask |= 0x0c00;
     break;
     }
      
   __asm__ __volatile__(
   "movw %%ax, %%bx\n\t"
   "movw %%cx, %%ax\n\t"
   "movl %%ebx, (%%edx)\n\t"
   "movl %%eax, 4(%%edx)\n\t"
   :
   :"d"(pIdtAddress), "b"(YI_HAL_KE_CS_SELC<<16), "a"(pFn), "c"(wPDplTypeMask)
   );
   
   return ;
}
/*
STATIC VOID YIHalSystemNullIntFun(INT nIntNum, YI_HAL_CPU_REGS* pRegs, PVOID pData)
{
   YI_HAL_INT_FINISH_INT; 
	YIRtlKernelMsg("intr %x ", pRegs->int_num);
}
*/


VOID YIHalMMPageFault(INT nIntNum, YI_HAL_CPU_REGS* pRegs, PVOID pData);

/* system call function. */
static VOID YISysCall(INT nInt, YI_HAL_CPU_REGS* pRegs, PVOID pData)
{

}


EXTERN YI_STATUS  YIHalIntInitialize(VOID)
{
	/*INT iCount = 0;*/

   	YIHalDisInt();
   	/* YIHalDisableExtInt();*/

    /*
	for(iCount = 0; iCount < YI_HAL_INT_MAX_INT_NUM; iCount++)
		yi_g_arHalIntFunTbl[iCount] = YIHalSystemNullIntFun;
	*/

	/* normal exceptions and interrupts. */
	YIHalSetIntTblItem(0x00, YI_HAL_INT_GATE_NAME(0x00),
				YI_HAL_CPU_INTGATE,YI_HAL_CPU_SYSLEVEL);	/* Divide error */
	/* YIHalSetIntTableItem(0x01, YI_HAL_INT_GATE_NAME(0x01),
					YI_HAL_CPU_SYSLEVEL);*/	/* RESERVED */
	YIHalSetIntTblItem(0x02, YI_HAL_INT_GATE_NAME(0x02),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);	/* NMI interrupt */
	/* YIHalSetIntTableItem(0x03, YI_HAL_INT_GATE_NAME(0x03),
				YI_HAL_CPU_USERLEVEL);*/	/* Break point */	
	YIHalSetIntTblItem(0x04, YI_HAL_INT_GATE_NAME(0x04),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);	/* Overflow */
	YIHalSetIntTblItem(0x05, YI_HAL_INT_GATE_NAME(0x05),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);	/* BOUND range exceeded*/
	YIHalSetIntTblItem(0x06, YI_HAL_INT_GATE_NAME(0x06),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);	/* Invalid Opcode */
	YIHalSetIntTblItem(0x07, YI_HAL_INT_GATE_NAME(0x07),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);	/* Device not available*/
	YIHalSetIntTblItem(0x08, YI_HAL_INT_GATE_NAME(0x08),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);	/* Double Fault */
	YIHalSetIntTblItem(0x09, YI_HAL_INT_GATE_NAME(0x09),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);	/* Coprocessor segment overrun */
	YIHalSetIntTblItem(0x0a, YI_HAL_INT_GATE_NAME(0x0a),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);	/* Invalid TSS */
	YIHalSetIntTblItem(0x0b, YI_HAL_INT_GATE_NAME(0x0b),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);	/* Segment not present */
	YIHalSetIntTblItem(0x0c, YI_HAL_INT_GATE_NAME(0x0c),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);	/* Stack segment fault */
	YIHalSetIntTblItem(0x0d, YI_HAL_INT_GATE_NAME(0x0d),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);	/* General protection */
	YIHalSetIntTblItem(0x0e, YI_HAL_INT_GATE_NAME(0x0e),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);	/* Page fault */

	/* x87 FPU floating-point error. */
	YIHalSetIntTblItem(0x10, YI_HAL_INT_GATE_NAME(0x10),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);


	/* System call gate */
	YIHalSetIntTblItem(YI_HAL_INT_VECTOR_SYSCALL, YI_HAL_INT_GATE_NAME(0x80),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_USERLEVEL);

	/* Extern interrute gate. */
	YIHalSetIntTblItem(0x40, YI_HAL_INT_GATE_NAME(0x40),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);
	YIHalSetIntTblItem(0x41, YI_HAL_INT_GATE_NAME(0x41),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);
	YIHalSetIntTblItem(0x42, YI_HAL_INT_GATE_NAME(0x42),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);
	YIHalSetIntTblItem(0x43, YI_HAL_INT_GATE_NAME(0x43),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);
	YIHalSetIntTblItem(0x44, YI_HAL_INT_GATE_NAME(0x44),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);
	YIHalSetIntTblItem(0x45, YI_HAL_INT_GATE_NAME(0x45),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);
	YIHalSetIntTblItem(0x46, YI_HAL_INT_GATE_NAME(0x46),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);
	YIHalSetIntTblItem(0x47, YI_HAL_INT_GATE_NAME(0x47),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);
	YIHalSetIntTblItem(0x48, YI_HAL_INT_GATE_NAME(0x48),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);
	YIHalSetIntTblItem(0x49, YI_HAL_INT_GATE_NAME(0x49),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);
	YIHalSetIntTblItem(0x4a, YI_HAL_INT_GATE_NAME(0x4a),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);
	YIHalSetIntTblItem(0x4b, YI_HAL_INT_GATE_NAME(0x4b),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);
	YIHalSetIntTblItem(0x4c, YI_HAL_INT_GATE_NAME(0x4c),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);
	YIHalSetIntTblItem(0x4d, YI_HAL_INT_GATE_NAME(0x4d),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);
	YIHalSetIntTblItem(0x4e, YI_HAL_INT_GATE_NAME(0x4e),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);
	YIHalSetIntTblItem(0x4f, YI_HAL_INT_GATE_NAME(0x4f),
				YI_HAL_CPU_INTGATE, YI_HAL_CPU_SYSLEVEL);

    YIIntSetTrapHandle(YI_HAL_INT_VECTOR_SYSCALL, YISysCall, NULL);
/*
	yi_g_arHalIntFunTbl[0x2e] = YIHalSysCall;
	yi_g_arHalIntFunTbl[0x40] = YIHalSystemTimeIntFun;
	yi_g_arHalIntFunTbl[0x0e] = YIHalMMPageFault;

	yi_g_arHalIntFunTbl[0x42] = YIHalCom2IntFun;
	yi_g_arHalIntFunTbl[0x43] = YIHalCom2IntFun;
  	yi_g_arHalIntFunTbl[0x44] = YIHalCom2IntFun; 
*/
	return E_OK;
}

/* System performance count. */
VOLATILE EXTERN UINT32  yi_g_nCpuPfmUsedTime;
VOLATILE EXTERN UINT32  yi_g_nCpuPfmFreeTime;

VOLATILE EXTERN UINT32  yi_g_nCpuPfmUsedTimeCount;
VOLATILE EXTERN UINT32  yi_g_nCpuPfmFreeTimeCount;
/*
static UINT64  yi_g_nCpuPfmUsedTime = 0;
static UINT64  yi_g_nCpuPfmFreeTime = 0;
static UINT64  yi_g_nCpuPfmUsedTimeCount = 0;
static UINT64  yi_g_nCpuPfmFreeTimeCount = 0;
*/

EXTERN INT yi_g_nCpuPfmIdle;
STATIC VOID GetTsc(PUINT32 pTscLow, PUINT32 pTscHigh)
{
   __asm__ __volatile__(
        "rdtsc\n\t"
        "movl %%eax, %0\n\t"
        "movl %%edx, %1\n\t"
        :"=m"(*pTscLow), "=m"(*pTscHigh)
        :
        :"eax", "edx"
    );
}


/* All traps and interrutes common function. */
STATIC VOID CDECLCALL YI_HAL_CMPATTR_USED YIHalCommonInterruptFun(YI_HAL_CPU_REGS sRegs)
{
  	/* Enter critical. */
	/* yi_g_nKeEnterCriticalDeepCount++; */
	/* YIHalEnterCritical();*/
    INT nIntNum;


    nIntNum = sRegs.int_num;

	/* increase interrupte deep count. */
	yi_g_nKeIntTrapDeepCount++;

	/* increate interrupte count. */
	yi_g_nInterruptCount++;

    if(nIntNum >= YI_HAL_INT_CHANNEL_START && nIntNum <= YI_HAL_INT_CHANNEL_END)
    {
      if(yi_g_nCpuPfmIdle)
        {

            UINT32 nTscCountLow, nTscCountHigh, nTscCount;

            GetTsc(&nTscCountLow, &nTscCountHigh);
            nTscCount = nTscCountLow>>16 | nTscCountHigh<<16;

            yi_g_nCpuPfmFreeTime += nTscCount-yi_g_nCpuPfmFreeTimeCount;

            yi_g_nCpuPfmUsedTimeCount = nTscCount;
            yi_g_nCpuPfmIdle = 0;
        }

        /* Call interrupter function. */
        YIIntDoInterrupterHandle(nIntNum-YI_HAL_INT_CHANNEL_START, &sRegs, NULL);
    }else{
   	    /* Call trap function. */
        YIIntDoTrapHandle(nIntNum, &sRegs, NULL);
   }
   	return;
}

