
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
 * Filename:	$RCSfile: time.c,v $
 * Createdate:	2004/10/08 08:19:00
 * FileId:	$Id: time.c,v 1.1.1.1 2005/08/09 05:25:24 devin Exp $
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
#include <yios/rtl/rtl.h>

#include <yios/hal/cpu.h>
#include <yios/hal/int.h>
#include <yios/hal/time.h>
#include <yios/time.h>

#include <yios/sched.h>
#include <yios/int.h>


EXTERN UINT CDECLCALL YIRtlKernelMsg(PCSTR fmt, ...);

STATIC YI_IRQ_HANDLE_STRUCT g_stTimeIrqHandleSt;

/* System performance count. */
VOLATILE EXTERN UINT32  yi_g_nCpuPfmUsedTime;
VOLATILE EXTERN UINT32  yi_g_nCpuPfmFreeTime;
VOLATILE EXTERN UINT32  yi_g_nCpuPfmUsedTimeCount;
VOLATILE EXTERN UINT32  yi_g_nCpuPfmFreeTimeCount;

EXTERN UINT g_st_rand_num;

#define F_WIDTH 150
INT yi_g_arCpuPfmGTbl[F_WIDTH];
INT yi_g_nCpuPfmGPos = 0;
VOLATILE EXTERN INT yi_g_nCpuPfmIdle;

EXTERN VOID YIHalSystemTimeIntFun(INT nIntNum, YI_HAL_CPU_REGS* pRegs, PVOID pData)
{
	yi_g_nTimeTickCount++;
	
	YI_HAL_INT_FINISH_INT;

    if(yi_g_nTimeTickCount%50 == 0)
    {
        UINT32 nTotal, nUsed;

        nUsed = yi_g_nCpuPfmUsedTime;
        nTotal = yi_g_nCpuPfmUsedTime+yi_g_nCpuPfmFreeTime;


        /* if(nUsed / (nTotal+1) > 0) 
            yi_g_arCpuPfmGTbl[yi_g_nCpuPfmGPos] = 100;*/
        if(nUsed == nTotal)
            yi_g_arCpuPfmGTbl[yi_g_nCpuPfmGPos] = 100;
        else
            yi_g_arCpuPfmGTbl[yi_g_nCpuPfmGPos] =(nUsed*100/(nTotal+1))%100;
 
       
        yi_g_nCpuPfmGPos++;
        yi_g_nCpuPfmGPos %= F_WIDTH;
       yi_g_nCpuPfmUsedTime = yi_g_nCpuPfmFreeTime = 0;        
    }

	YIKeTimerRoutine(0);

}



/* EXTERN YI_STATUS YICALL YIHalTimeInitialize(VOID) */
EXTERN YI_STATUS YITimeInitialize(VOID)
{

	/* Initialize time 0 for system time interrupt. */
	__asm__ __volatile__(
	"pushf\n\t"
	"cli\n\t"
	"movb $0x36, %%al\n\t"
	"outb %%al, $0x43\n\t"
	"jmp	 1f\n\t"
	"1:\t outb %%al, $0x80\n\t"
	"movw %0, %%ax\n\t"
	"outb %%al, $0x40\n\t"
	"jmp	 1f\n\t"
	"1:\t outb %%al, $0x80\n\t"
	"movb %%ah, %%al\n\t"
	"outb %%al, $0x40\n\t"
	"jmp 1f\n\t"
	"1:\t outb %%al, $0x80\n\t"
	"popf\n\t"
	:
	:"i"(1193180/CFG_KERNEL_SCHED_HZ)
	);

	
    YIRtlMemoryZero(&g_stTimeIrqHandleSt, sizeof(g_stTimeIrqHandleSt));

    g_stTimeIrqHandleSt.pHandle = YIHalSystemTimeIntFun;
    g_stTimeIrqHandleSt.nMask = YI_IRQM_SHARE;

    YIIntInstallHandle(YI_HAL_INT_CHANNEL_TIME, &g_stTimeIrqHandleSt);

    YIRtlMemoryZero(yi_g_arCpuPfmGTbl, sizeof(yi_g_arCpuPfmGTbl));


	return E_OK;
}

