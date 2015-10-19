
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
 * Filename:	$RCSfile: nkb.c,v $
 * Createdate:	2005/01/08 00:18:00
 * FileId:	$Id: nkb.c,v 1.1.1.1 2005/08/09 05:25:25 devin Exp $
 * Last update:	$Date: 2005/08/09 05:25:25 $
 * Creator:	Devin Wang <vhd@21cn.com>
 * Version:	$Revision: 1.1.1.1 $
 * Description: normal keybord drvier for pc
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
#include <yios/driver.h>

#include <yios/int.h>
#include <yios/rtl/rtl.h>

#include <yios/hal/io.h>

#define KBPORT_DATA     0x60
#define KBPORT_CMD      0x64

#define KBDATC_RESET    0xff

EXTERN UINT CDECLCALL YIRtlKernelMsg(PCSTR fmt, ...);

STATIC  YI_IRQ_HANDLE_STRUCT yi_g_stKBIrqHandleSt;
STATIC CONST UINT8 yi_g_arNKBMapTbl[128] =
{
 /*      0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f */
/* 0 */ 0x00, 0x00,  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  '0',  '-',  '=', 0x00, 0x00,
/* 1 */  'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',  'o',  'p',  '[',  ']', '\r', 0x00,  'a',  's',
/* 2 */  'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';',  '\'',  '`', 0x00,  '\\', 'z',  'x',  'c',  'v',
/* 3 */  'b',  'n',  'm',  ',',  '.',  '/', 0x00, 0x00, 0x00,  ' ', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 4 */	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 5 */	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 6 */	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 7 */	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

UINT8 yi_g_NKBBuffer[12];
INT yi_g_NKBBufPosStart = 0;
INT yi_g_NKBBufPosEnd = 0;
INT yi_g_NKBShiftDown = 0;

STATIC VOID YIDriverKBIntHandle(INT nIntNum, YI_HAL_CPU_REGS* pRegs, PVOID pData)
{
    UINT8 nStatus, nCode;
   

    nCode = YIIoReadPortByte(KBPORT_DATA);
    nStatus = YIIoReadPortByte(KBPORT_CMD);
    /* YIRtlKernelMsg("(Key:%d,%c)", nCode, nCode);*/


    YI_HAL_INT_FINISH_INT;
    /* YIRtlKernelMsg("(Key:%2x,%c)", dd, dd);*/
    /* dd = YIIoReadPortByte(KBPORT_CMD);*/


    /*YIRtlKernelMsg("(Stat:%2x,Code:%2x)", nStatus, yi_g_arNKBMapTbl[nCode]);*/
    if(nCode == 0x2a || nCode == 0x36)
        yi_g_NKBShiftDown = 1;
    if(nCode == 0xaa || nCode == 0xb6)
        yi_g_NKBShiftDown = 0;


    if(nCode < 0x80)
    {
        if((yi_g_NKBBufPosStart+1)%12 != yi_g_NKBBufPosEnd)
        {   
            yi_g_NKBBuffer[yi_g_NKBBufPosStart] = yi_g_arNKBMapTbl[nCode];
            yi_g_NKBBufPosStart++;
            yi_g_NKBBufPosStart %= 12;
        }
            YIKeSRMResumeThread(3);
//    YIRtlKernelMsg("%c", yi_g_arNKBMapTbl[nCode]);
//    YIRtlKernelMsg("(%s,%d)", yi_g_NKBBuffer, yi_g_NKBBufPos);
    }

    
    //if(nCode )
    

    /* if(nCode == 0)
        return;
    */
    /* YIKeTimerRoutine(0);*/
}

EXTERN YI_STATUS  YIDriverKBInitialize(VOID)
{
   
    YIRtlMemoryZero(yi_g_NKBBuffer, sizeof(yi_g_NKBBuffer));

   /* Initialize keybord interrupt chanel. */
    YIRtlMemoryZero(&yi_g_stKBIrqHandleSt, sizeof(yi_g_stKBIrqHandleSt));

    yi_g_stKBIrqHandleSt.pHandle = YIDriverKBIntHandle;
    yi_g_stKBIrqHandleSt.nMask = YI_IRQM_SHARE;

    YIIntInstallHandle(YI_HAL_INT_CHANNEL_KEYBD, &yi_g_stKBIrqHandleSt);

    /*nCode = YIIoReadPortByte(KBPORT_DATA);
    YIRtlKernelMsg("(Key:%d,%c)", nCode, nCode);
*/
 
    /* Initialize keybord hardware. */
    //YIIoReadPortByte(0x21)
    //YIIoWritePortByte(KBDATC_RESET, KBPORT_DATA);
    return E_OK;
}

